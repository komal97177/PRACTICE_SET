#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Constants
#define PHYSICAL_MEMORY_SIZE 4    // Number of frames in physical memory
#define PAGE_SIZE 1               // Size of each page in KB
#define DISK_SIZE 16              // Number of pages in disk
#define MAX_PROCESSES 3
#define MAX_VIRTUAL_PAGES 10

// Page table entry structure
typedef struct {
    int physical_frame;  // Physical frame number (-1 if not loaded)
    bool loaded;         // Whether the page is loaded in memory
    bool dirty;          // Whether the page has been modified
    int disk_location;   // Where the page is stored on disk
} PageTableEntry;

// Process control block structure
typedef struct {
    int process_id;
    PageTableEntry *page_table;  // Array of page table entries
    int num_pages;               // Number of virtual pages
} ProcessCB;

// Global variables
char physical_memory[PHYSICAL_MEMORY_SIZE][PAGE_SIZE * 1024];  // Physical memory
char disk[DISK_SIZE][PAGE_SIZE * 1024];                        // Disk storage
int fifo_queue[PHYSICAL_MEMORY_SIZE];                          // FIFO queue for page replacement
int front = -1, rear = -1;                                     // Queue pointers

ProcessCB processes[MAX_PROCESSES];
int process_count = 0;

// Statistics
int page_faults = 0;
int disk_operations = 0;

// Function prototypes
void initialize_memory();
int create_process(int num_pages);
bool allocate_memory(int process_id, int virtual_page);
bool access_memory(int process_id, int virtual_page, char operation);
int find_free_frame();
int replace_page();
void load_page_from_disk(int process_id, int virtual_page, int frame_index);
void write_page_to_disk(int process_id, int virtual_page, int frame_index);
void display_memory_status();
void enqueue(int frame_index);
int dequeue();
bool is_queue_empty();
void simulate_process(int process_id, int *pages_to_access, int num_accesses);

int main() {
    srand(time(0));  // Seed for random number generation
    
    printf("Virtual OS Memory Manager with FIFO Page Replacement\n");
    printf("====================================================\n\n");
    
    // Initialize memory and disk
    initialize_memory();
    
    // Create processes with virtual memory
    int p1 = create_process(5);  // Process 0 with 5 virtual pages
    int p2 = create_process(4);  // Process 1 with 4 virtual pages
    int p3 = create_process(6);  // Process 2 with 6 virtual pages
    
    // Define memory access patterns for each process
    int access_pattern1[] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    int access_pattern2[] = {2, 3, 4, 2, 3, 5, 2, 3, 4, 5};
    int access_pattern3[] = {1, 2, 1, 3, 1, 4, 1, 5, 1, 6};
    
    // Simulate process memory accesses
    simulate_process(p1, access_pattern1, sizeof(access_pattern1)/sizeof(access_pattern1[0]));
    simulate_process(p2, access_pattern2, sizeof(access_pattern2)/sizeof(access_pattern2[0]));
    simulate_process(p3, access_pattern3, sizeof(access_pattern3)/sizeof(access_pattern3[0]));
    
    // Display final memory status
    display_memory_status();
    
    // Clean up
    for (int i = 0; i < process_count; i++) {
        free(processes[i].page_table);
    }
    
    return 0;
}

void initialize_memory() {
    // Initialize physical memory
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        sprintf(physical_memory[i], "FRAME_%d_DATA", i);
    }
    
    // Initialize disk
    for (int i = 0; i < DISK_SIZE; i++) {
        sprintf(disk[i], "DISK_DATA_%d", i);
    }
    
    printf("Memory initialized with %d frames (%d KB each)\n", 
           PHYSICAL_MEMORY_SIZE, PAGE_SIZE);
    printf("Disk initialized with %d pages\n\n", DISK_SIZE);
}

int create_process(int num_pages) {
    if (process_count >= MAX_PROCESSES) {
        printf("Cannot create more processes. Maximum limit reached.\n");
        return -1;
    }
    
    if (num_pages > MAX_VIRTUAL_PAGES) {
        printf("Process cannot have more than %d virtual pages.\n", MAX_VIRTUAL_PAGES);
        return -1;
    }
    
    ProcessCB *p = &processes[process_count];
    p->process_id = process_count;
    p->num_pages = num_pages;
    p->page_table = (PageTableEntry*)malloc(num_pages * sizeof(PageTableEntry));
    
    // Initialize page table
    for (int i = 0; i < num_pages; i++) {
        p->page_table[i].physical_frame = -1;  // Not loaded
        p->page_table[i].loaded = false;
        p->page_table[i].dirty = false;
        p->page_table[i].disk_location = (p->process_id * MAX_VIRTUAL_PAGES + i) % DISK_SIZE;
    }
    
    printf("Process %d created with %d virtual pages\n", p->process_id, num_pages);
    return process_count++;
}

bool allocate_memory(int process_id, int virtual_page) {
    if (process_id >= process_count || process_id < 0) {
        printf("Invalid process ID: %d\n", process_id);
        return false;
    }
    
    ProcessCB *p = &processes[process_id];
    
    if (virtual_page >= p->num_pages || virtual_page < 0) {
        printf("Invalid virtual page: %d for process %d\n", virtual_page, process_id);
        return false;
    }
    
    // Check if already allocated
    if (p->page_table[virtual_page].loaded) {
        printf("Page %d already allocated for process %d\n", virtual_page, process_id);
        return true;
    }
    
    // Find a free frame or replace one
    int frame_index = find_free_frame();
    if (frame_index == -1) {
        frame_index = replace_page();
    }
    
    // Load the page from disk
    load_page_from_disk(process_id, virtual_page, frame_index);
    
    // Update page table
    p->page_table[virtual_page].physical_frame = frame_index;
    p->page_table[virtual_page].loaded = true;
    p->page_table[virtual_page].dirty = false;
    
    // Add to FIFO queue
    enqueue(frame_index);
    
    printf("Allocated page %d for process %d in frame %d\n", 
           virtual_page, process_id, frame_index);
    
    return true;
}

bool access_memory(int process_id, int virtual_page, char operation) {
    if (process_id >= process_count || process_id < 0) {
        printf("Invalid process ID: %d\n", process_id);
        return false;
    }
    
    ProcessCB *p = &processes[process_id];
    
    if (virtual_page >= p->num_pages || virtual_page < 0) {
        printf("Invalid virtual page: %d for process %d\n", virtual_page, process_id);
        return false;
    }
    
    PageTableEntry *entry = &p->page_table[virtual_page];
    
    // If page is loaded in physical memory
    if (entry->loaded) {
        printf("Process %d accessed page %d in frame %d (%c)\n", 
               process_id, virtual_page, entry->physical_frame, operation);
        
        if (operation == 'W') {
            entry->dirty = true;
            // Simulate writing to memory
            sprintf(physical_memory[entry->physical_frame], "PROC_%d_PAGE_%d_MODIFIED", 
                    process_id, virtual_page);
        }
        
        return true;
    }
    
    // Page fault handling
    page_faults++;
    printf("Page fault occurred for process %d on page %d\n", process_id, virtual_page);
    
    // Find a free frame or replace one
    int frame_index = find_free_frame();
    if (frame_index == -1) {
        frame_index = replace_page();
    }
    
    // Load the page from disk
    load_page_from_disk(process_id, virtual_page, frame_index);
    
    // Update page table
    entry->physical_frame = frame_index;
    entry->loaded = true;
    if (operation == 'W') {
        entry->dirty = true;
        // Simulate writing to memory
        sprintf(physical_memory[frame_index], "PROC_%d_PAGE_%d_MODIFIED", 
                process_id, virtual_page);
    }
    
    // Add to FIFO queue
    enqueue(frame_index);
    
    printf("Process %d accessed page %d now loaded in frame %d (%c)\n", 
           process_id, virtual_page, frame_index, operation);
    
    return true;
}

int find_free_frame() {
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        bool frame_in_use = false;
        
        // Check if this frame is used by any process
        for (int p = 0; p < process_count; p++) {
            for (int j = 0; j < processes[p].num_pages; j++) {
                if (processes[p].page_table[j].loaded && 
                    processes[p].page_table[j].physical_frame == i) {
                    frame_in_use = true;
                    break;
                }
            }
            if (frame_in_use) break;
        }
        
        if (!frame_in_use) {
            return i;
        }
    }
    return -1;  // No free frame found
}

int replace_page() {
    if (is_queue_empty()) {
        return 0;  // Fallback if queue is empty
    }
    
    // Get the oldest frame from the FIFO queue
    int frame_to_replace = dequeue();
    
    // Find which process and page is using this frame
    for (int p = 0; p < process_count; p++) {
        for (int i = 0; i < processes[p].num_pages; i++) {
            PageTableEntry *entry = &processes[p].page_table[i];
            if (entry->loaded && entry->physical_frame == frame_to_replace) {
                // If the page is dirty, write it back to disk
                if (entry->dirty) {
                    write_page_to_disk(p, i, frame_to_replace);
                }
                
                // Update the page table for the replaced page
                entry->loaded = false;
                entry->physical_frame = -1;
                entry->dirty = false;
                
                printf("Replaced page %d from process %d in frame %d\n", 
                       i, p, frame_to_replace);
                
                return frame_to_replace;
            }
        }
    }
    
    return 0;  // Should not reach here
}

void load_page_from_disk(int process_id, int virtual_page, int frame_index) {
    ProcessCB *p = &processes[process_id];
    int disk_location = p->page_table[virtual_page].disk_location;
    
    // Simulate loading from disk to memory
    sprintf(physical_memory[frame_index], "LOADED_FROM_DISK_%d", disk_location);
    disk_operations++;
    
    printf("Loaded page %d from process %d from disk location %d to frame %d\n", 
           virtual_page, process_id, disk_location, frame_index);
}

void write_page_to_disk(int process_id, int virtual_page, int frame_index) {
    ProcessCB *p = &processes[process_id];
    int disk_location = p->page_table[virtual_page].disk_location;
    
    // Simulate writing from memory to disk
    sprintf(disk[disk_location], "SAVED_FROM_FRAME_%d", frame_index);
    disk_operations++;
    
    printf("Written page %d from process %d from frame %d to disk location %d\n", 
           virtual_page, process_id, frame_index, disk_location);
}

void display_memory_status() {
    printf("\n====================================================\n");
    printf("Final Memory Status:\n");
    printf("Physical Memory:\n");
    
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        bool frame_used = false;
        
        // Check if this frame is used by any process
        for (int p = 0; p < process_count; p++) {
            for (int j = 0; j < processes[p].num_pages; j++) {
                if (processes[p].page_table[j].loaded && 
                    processes[p].page_table[j].physical_frame == i) {
                    printf("  Frame %d: Process %d, Page %d - %s\n", 
                           i, p, j, physical_memory[i]);
                    frame_used = true;
                    break;
                }
            }
            if (frame_used) break;
        }
        
        if (!frame_used) {
            printf("  Frame %d: Free\n", i);
        }
    }
    
    printf("\nPage Tables:\n");
    for (int p = 0; p < process_count; p++) {
        printf("  Process %d:\n", p);
        for (int i = 0; i < processes[p].num_pages; i++) {
            PageTableEntry *entry = &processes[p].page_table[i];
            char status[20];
            if (entry->loaded) {
                sprintf(status, "Frame %d", entry->physical_frame);
            } else {
                strcpy(status, "Not loaded");
            }
            
            printf("    Page %d: %s, %s, Disk: %d\n", 
                   i, status, entry->dirty ? "Dirty" : "Clean", entry->disk_location);
        }
    }
    
    printf("\nStatistics:\n");
    printf("  Page Faults: %d\n", page_faults);
    printf("  Disk Operations: %d\n", disk_operations);
    printf("====================================================\n");
}

// FIFO queue operations
void enqueue(int frame_index) {
    if (rear == PHYSICAL_MEMORY_SIZE - 1) {
        // Queue is full, need to resize (shouldn't happen in this implementation)
        return;
    }
    
    if (front == -1) {
        front = 0;
    }
    
    rear++;
    fifo_queue[rear] = frame_index;
}

int dequeue() {
    if (front == -1) {
        return -1;  // Queue is empty
    }
    
    int frame_index = fifo_queue[front];
    
    if (front == rear) {
        front = rear = -1;
    } else {
        front++;
    }
    
    return frame_index;
}

bool is_queue_empty() {
    return front == -1;
}

void simulate_process(int process_id, int *pages_to_access, int num_accesses) {
    printf("\nProcess %d starting with %d memory accesses\n", process_id, num_accesses);
    
    for (int i = 0; i < num_accesses; i++) {
        int page = pages_to_access[i];
        
        // Randomly choose read or write operation
        char operation = (rand() % 4 == 0) ? 'W' : 'R';  // 25% write, 75% read
        
        // Ensure the page is allocated
        allocate_memory(process_id, page);
        
        // Access the memory
        access_memory(process_id, page, operation);
    }
    
    printf("Process %d completed\n", process_id);
}