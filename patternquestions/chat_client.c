#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int client_socket;
char name[32];

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    int read_size;
    
    while ((read_size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    
    if (read_size == 0) {
        printf("\nServer disconnected\n");
    } else if (read_size == -1) {
        perror("Recv failed");
    }
    
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    pthread_t thread_id;
    char buffer[BUFFER_SIZE];
    
    // Get user name
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    
    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    // Send name to server
    send(client_socket, name, strlen(name), 0);
    
    printf("Connected to chat server. Type '/quit' to exit.\n");
    
    // Create thread for receiving messages
    if (pthread_create(&thread_id, NULL, receive_messages, NULL) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    
    pthread_detach(thread_id);
    
    // Send messages
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "/quit") == 0) {
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }
        
        send(client_socket, buffer, strlen(buffer), 0);
    }
    
    close(client_socket);
    printf("Disconnected from server\n");
    
    return 0;
}