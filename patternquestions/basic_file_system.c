#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_CONTENT 1024
#define TOTAL_SPACE 10240  // 10KB total space

typedef struct {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
    int size;
    int allocated;
    time_t created;
    time_t modified;
} File;

typedef struct {
    File files[MAX_FILES];
    int total_space;
    int used_space;
} FileSystem;

void initialize_filesystem(FileSystem *fs) {
    fs->total_space = TOTAL_SPACE;
    fs->used_space = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        fs->files[i].allocated = 0;
        fs->files[i].name[0] = '\0';
        fs->files[i].content[0] = '\0';
        fs->files[i].size = 0;
    }
}

int create_file(FileSystem *fs, const char *filename) {
    // Check if file already exists
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->files[i].allocated && strcmp(fs->files[i].name, filename) == 0) {
            printf("Error: File '%s' already exists.\n", filename);
            return 0;
        }
    }
    
    // Find empty slot
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs->files[i].allocated) {
            strncpy(fs->files[i].name, filename, MAX_FILENAME);
            fs->files[i].content[0] = '\0';
            fs->files[i].size = 0;
            fs->files[i].allocated = 1;
            fs->files[i].created = time(NULL);
            fs->files[i].modified = time(NULL);
            
            printf("File '%s' created successfully.\n", filename);
            return 1;
        }
    }
    
    printf("Error: Maximum number of files reached.\n");
    return 0;
}

int write_file(FileSystem *fs, const char *filename, const char *content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->files[i].allocated && strcmp(fs->files[i].name, filename) == 0) {
            int new_size = strlen(content);
            int size_difference = new_size - fs->files[i].size;
            
            // Check if there's enough space
            if (fs->used_space + size_difference > fs->total_space) {
                printf("Error: Not enough space to write to file '%s'.\n", filename);
                return 0;
            }
            
            // Update content and size
            strncpy(fs->files[i].content, content, MAX_CONTENT);
            fs->files[i].size = new_size;
            fs->used_space += size_difference;
            fs->files[i].modified = time(NULL);
            
            printf("Content written to file '%s' successfully.\n", filename);
            return 1;
        }
    }
    
    printf("Error: File '%s' not found.\n", filename);
    return 0;
}

int read_file(FileSystem *fs, const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->files[i].allocated && strcmp(fs->files[i].name, filename) == 0) {
            printf("Content of file '%s':\n%s\n", filename, fs->files[i].content);
            printf("Size: %d bytes, Created: %sModified: %s", 
                   fs->files[i].size, 
                   ctime(&fs->files[i].created), 
                   ctime(&fs->files[i].modified));
            return 1;
        }
    }
    
    printf("Error: File '%s' not found.\n", filename);
    return 0;
}

int delete_file(FileSystem *fs, const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->files[i].allocated && strcmp(fs->files[i].name, filename) == 0) {
            // Free up space
            fs->used_space -= fs->files[i].size;
            
            // Deallocate file
            fs->files[i].allocated = 0;
            fs->files[i].name[0] = '\0';
            fs->files[i].content[0] = '\0';
            fs->files[i].size = 0;
            
            printf("File '%s' deleted successfully.\n", filename);
            return 1;
        }
    }
    
    printf("Error: File '%s' not found.\n", filename);
    return 0;
}

void display_filesystem_status(FileSystem *fs) {
    printf("\n=== File System Status ===\n");
    printf("Total space: %d bytes\n", fs->total_space);
    printf("Used space: %d bytes\n", fs->used_space);
    printf("Available space: %d bytes\n", fs->total_space - fs->used_space);
    printf("Files in system:\n");
    
    int file_count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs->files[i].allocated) {
            printf("  %s (%d bytes)\n", fs->files[i].name, fs->files[i].size);
            file_count++;
        }
    }
    
    if (file_count == 0) {
        printf("  No files found.\n");
    }
    printf("==========================\n\n");
}

int main() {
    FileSystem fs;
    initialize_filesystem(&fs);
    
    int choice;
    char filename[MAX_FILENAME];
    char content[MAX_CONTENT];
    
    printf("Basic File System Simulation\n");
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Create file\n");
        printf("2. Write to file\n");
        printf("3. Read file\n");
        printf("4. Delete file\n");
        printf("5. Show file system status\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar();  // Consume newline
        
        switch (choice) {
            case 1:
                printf("Enter filename: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0;  // Remove newline
                create_file(&fs, filename);
                break;
                
            case 2:
                printf("Enter filename: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0;
                printf("Enter content: ");
                fgets(content, MAX_CONTENT, stdin);
                content[strcspn(content, "\n")] = 0;
                write_file(&fs, filename, content);
                break;
                
            case 3:
                printf("Enter filename: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0;
                read_file(&fs, filename);
                break;
                
            case 4:
                printf("Enter filename: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0;
                delete_file(&fs, filename);
                break;
                
            case 5:
                display_filesystem_status(&fs);
                break;
                
            case 6:
                printf("Exiting file system. Goodbye!\n");
                exit(0);
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}