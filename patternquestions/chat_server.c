#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

typedef struct {
    int socket;
    struct sockaddr_in address;
    int uid;
    char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int client_count = 0;

void add_client(client_t *cl) {
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = cl;
            client_count++;
            break;
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int uid) {
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid == uid) {
            clients[i] = NULL;
            client_count--;
            break;
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_to_all(char *message, int uid) {
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->uid != uid) {
            if (send(clients[i]->socket, message, strlen(message), 0) < 0) {
                perror("Send failed");
            }
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    client_t *cli = (client_t *)arg;
    char buffer[BUFFER_SIZE];
    int read_size;
    
    // Get client name
    if ((read_size = recv(cli->socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        strcpy(cli->name, buffer);
        
        char welcome_message[BUFFER_SIZE];
        snprintf(welcome_message, sizeof(welcome_message), "%s has joined the chat!\n", cli->name);
        send_message_to_all(welcome_message, cli->uid);
        
        printf("%s connected from %s:%d\n", 
               cli->name, 
               inet_ntoa(cli->address.sin_addr), 
               ntohs(cli->address.sin_port));
    }
    
    // Handle messages
    while ((read_size = recv(cli->socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        
        if (strcmp(buffer, "/quit") == 0) {
            break;
        }
        
        char formatted_message[BUFFER_SIZE + 50];
        snprintf(formatted_message, sizeof(formatted_message), "%s: %s", cli->name, buffer);
        send_message_to_all(formatted_message, cli->uid);
        
        printf("Message from %s: %s", cli->name, buffer);
    }
    
    // Client disconnected
    if (read_size == 0) {
        printf("%s disconnected\n", cli->name);
    } else if (read_size == -1) {
        perror("Recv failed");
    }
    
    // Notify other clients and clean up
    char leave_message[BUFFER_SIZE];
    snprintf(leave_message, sizeof(leave_message), "%s has left the chat\n", cli->name);
    send_message_to_all(leave_message, cli->uid);
    
    remove_client(cli->uid);
    close(cli->socket);
    free(cli);
    
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Chat server started on port %d\n", PORT);
    printf("Waiting for connections...\n");
    
    // Accept connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Check if server is full
        if (client_count >= MAX_CLIENTS) {
            char *msg = "Server is full. Try again later.\n";
            send(client_socket, msg, strlen(msg), 0);
            close(client_socket);
            continue;
        }
        
        // Create client structure
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->socket = client_socket;
        cli->address = client_addr;
        cli->uid = rand() % 1000;
        strcpy(cli->name, "Anonymous");
        
        // Add client to list and create thread
        add_client(cli);
        
        if (pthread_create(&thread_id, NULL, handle_client, (void *)cli) != 0) {
            perror("Thread creation failed");
            free(cli);
            continue;
        }
        
        pthread_detach(thread_id);
        
        printf("Client connected from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));
    }
    
    close(server_socket);
    return 0;
}