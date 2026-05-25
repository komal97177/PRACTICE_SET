#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Structure to store algorithm results
typedef struct {
    char name[20];
    int total_seek_time;
    double average_seek_time;
    int *sequence;
    int sequence_size;
} AlgorithmResult;

// Function prototypes
int compare(const void *a, const void *b);
AlgorithmResult sstf(int *requests, int n, int initial_pos);
AlgorithmResult cscan(int *requests, int n, int initial_pos, int disk_size);
void print_results(AlgorithmResult result);

int main() {
    int disk_size, initial_pos, n;
    
    printf("Enter disk size: ");
    scanf("%d", &disk_size);
    
    printf("Enter initial head position: ");
    scanf("%d", &initial_pos);
    
    printf("Enter number of requests: ");
    scanf("%d", &n);
    
    int *requests = (int *)malloc(n * sizeof(int));
    
    printf("Enter the requests: ");
    for(int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }
    
    printf("\n--- Simulation Results ---\n");
    
    // Run SSTF algorithm
    AlgorithmResult sstf_result = sstf(requests, n, initial_pos);
    print_results(sstf_result);
    
    // Run C-SCAN algorithm
    AlgorithmResult cscan_result = cscan(requests, n, initial_pos, disk_size);
    print_results(cscan_result);
    
    // Compare results
    printf("\n--- Comparison ---\n");
    printf("SSTF total seek time: %d\n", sstf_result.total_seek_time);
    printf("C-SCAN total seek time: %d\n", cscan_result.total_seek_time);
    
    if(sstf_result.total_seek_time < cscan_result.total_seek_time) {
        printf("SSTF performs better by %d units\n", 
               cscan_result.total_seek_time - sstf_result.total_seek_time);
    } else {
        printf("C-SCAN performs better by %d units\n", 
               sstf_result.total_seek_time - cscan_result.total_seek_time);
    }
    
    // Free allocated memory
    free(requests);
    free(sstf_result.sequence);
    free(cscan_result.sequence);
    
    return 0;
}

// SSTF algorithm implementation
AlgorithmResult sstf(int *requests, int n, int initial_pos) {
    AlgorithmResult result;
    strcpy(result.name, "SSTF");
    result.total_seek_time = 0;
    result.sequence = (int *)malloc((n + 1) * sizeof(int));
    result.sequence_size = n + 1;
    
    int *temp_requests = (int *)malloc(n * sizeof(int));
    int *visited = (int *)calloc(n, sizeof(int));
    
    // Copy requests
    for(int i = 0; i < n; i++) {
        temp_requests[i] = requests[i];
    }
    
    int current_pos = initial_pos;
    result.sequence[0] = current_pos;
    
    for(int i = 0; i < n; i++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        
        // Find the closest request
        for(int j = 0; j < n; j++) {
            if(!visited[j]) {
                int distance = abs(temp_requests[j] - current_pos);
                if(distance < min_distance) {
                    min_distance = distance;
                    min_index = j;
                }
            }
        }
        
        visited[min_index] = 1;
        result.total_seek_time += min_distance;
        current_pos = temp_requests[min_index];
        result.sequence[i + 1] = current_pos;
    }
    
    result.average_seek_time = (double)result.total_seek_time / n;
    
    free(temp_requests);
    free(visited);
    
    return result;
}

// C-SCAN algorithm implementation
AlgorithmResult cscan(int *requests, int n, int initial_pos, int disk_size) {
    AlgorithmResult result;
    strcpy(result.name, "C-SCAN");
    result.total_seek_time = 0;
    result.sequence = (int *)malloc((n + 3) * sizeof(int)); // Extra for endpoints
    result.sequence_size = 0;
    
    int *temp_requests = (int *)malloc((n + 2) * sizeof(int));
    
    // Copy requests and add endpoints
    for(int i = 0; i < n; i++) {
        temp_requests[i] = requests[i];
    }
    
    // Sort the requests
    qsort(temp_requests, n, sizeof(int), compare);
    
    // Find the position where initial head is located
    int i = 0;
    while(i < n && temp_requests[i] < initial_pos) {
        i++;
    }
    
    int sequence_index = 0;
    result.sequence[sequence_index++] = initial_pos;
    
    // Move towards the end
    for(int j = i; j < n; j++) {
        result.total_seek_time += abs(temp_requests[j] - initial_pos);
        initial_pos = temp_requests[j];
        result.sequence[sequence_index++] = initial_pos;
    }
    
    // Jump to the beginning and add disk end if needed
    if(initial_pos != disk_size - 1) {
        result.total_seek_time += abs((disk_size - 1) - initial_pos);
        initial_pos = disk_size - 1;
        result.sequence[sequence_index++] = initial_pos;
    }
    
    // Jump to start
    result.total_seek_time += abs(0 - initial_pos);
    initial_pos = 0;
    result.sequence[sequence_index++] = initial_pos;
    
    // Process requests from the beginning
    for(int j = 0; j < i; j++) {
        result.total_seek_time += abs(temp_requests[j] - initial_pos);
        initial_pos = temp_requests[j];
        result.sequence[sequence_index++] = initial_pos;
    }
    
    result.sequence_size = sequence_index;
    result.average_seek_time = (double)result.total_seek_time / n;
    
    free(temp_requests);
    
    return result;
}

// Helper function for qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Print results
void print_results(AlgorithmResult result) {
    printf("\n%s Algorithm:\n", result.name);
    printf("Seek sequence: ");
    for(int i = 0; i < result.sequence_size; i++) {
        printf("%d", result.sequence[i]);
        if(i < result.sequence_size - 1) {
            printf(" -> ");
        }
    }
    printf("\nTotal seek time: %d\n", result.total_seek_time);
    printf("Average seek time: %.2f\n", result.average_seek_time);
}