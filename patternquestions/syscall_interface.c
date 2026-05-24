#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

// System call numbers
#define SYS_READ    0
#define SYS_WRITE   1
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_TIME    4
#define SYS_EXIT    5
#define SYS_FORK    6
#define SYS_GETPID  7

// Error codes
#define ESUCCESS    0
#define EINVAL      1
#define EACCES      2
#define ENOENT      3
#define EBADF       4
#define ENOMEM      5

// Maximum system calls
#define MAX_SYSCALLS 8

// System call function pointer type
typedef int (*syscall_handler_t)(int, void*);

// System call table
syscall_handler_t syscall_table[MAX_SYSCALLS];

// Process control block (simplified)
typedef struct {
    int pid;
    int open_files[10];
} pcb_t;

// Global process control block
pcb_t current_process;

// System call handlers
int sys_read(int fd, void *buffer) {
    if (fd < 0 || fd >= 10 || current_process.open_files[fd] == 0) {
        return -EBADF;
    }
    
    // Simulate reading from file
    printf("Reading from file descriptor %d\n", fd);
    strcpy((char*)buffer, "Sample file content");
    return strlen((char*)buffer);
}

int sys_write(int fd, void *buffer) {
    if (fd < 0 || fd >= 10 || current_process.open_files[fd] == 0) {
        return -EBADF;
    }
    
    // Simulate writing to file
    printf("Writing to file descriptor %d: %s\n", fd, (char*)buffer);
    return strlen((char*)buffer);
}

int sys_open(const char *filename, int flags) {
    // Find an available file descriptor
    int fd = -1;
    for (int i = 0; i < 10; i++) {
        if (current_process.open_files[i] == 0) {
            fd = i;
            break;
        }
    }
    
    if (fd == -1) {
        return -ENOMEM; // Too many open files
    }
    
    // Simulate file opening
    printf("Opening file: %s\n", filename);
    current_process.open_files[fd] = 1;
    return fd;
}

int sys_close(int fd) {
    if (fd < 0 || fd >= 10 || current_process.open_files[fd] == 0) {
        return -EBADF;
    }
    
    // Simulate file closing
    printf("Closing file descriptor %d\n", fd);
    current_process.open_files[fd] = 0;
    return ESUCCESS;
}

int sys_time(void *time_buffer) {
    time_t t = time(NULL);
    *(time_t*)time_buffer = t;
    printf("System time requested: %ld\n", t);
    return ESUCCESS;
}

int sys_exit(int status) {
    printf("Process exiting with status: %d\n", status);
    exit(status);
    return ESUCCESS; // Never reached
}

int sys_fork(void) {
    printf("Forking process\n");
    // In a real OS, this would create a new process
    // Here we just return a dummy PID
    return current_process.pid + 1;
}

int sys_getpid(void) {
    printf("Process ID requested\n");
    return current_process.pid;
}

// Initialize system call table
void init_syscall_table(void) {
    memset(syscall_table, 0, sizeof(syscall_table));
    
    syscall_table[SYS_READ] = (syscall_handler_t)sys_read;
    syscall_table[SYS_WRITE] = (syscall_handler_t)sys_write;
    syscall_table[SYS_OPEN] = (syscall_handler_t)sys_open;
    syscall_table[SYS_CLOSE] = (syscall_handler_t)sys_close;
    syscall_table[SYS_TIME] = (syscall_handler_t)sys_time;
    syscall_table[SYS_EXIT] = (syscall_handler_t)sys_exit;
    syscall_table[SYS_FORK] = (syscall_handler_t)sys_fork;
    syscall_table[SYS_GETPID] = (syscall_handler_t)sys_getpid;
}

// System call interface
int syscall(int number, int arg1, void* arg2) {
    if (number < 0 || number >= MAX_SYSCALLS || syscall_table[number] == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    return syscall_table[number](arg1, arg2);
}

// Error handling function
void handle_error(int error_code, const char* syscall_name) {
    switch (error_code) {
        case ESUCCESS:
            break;
        case EINVAL:
            fprintf(stderr, "Error in %s: Invalid argument\n", syscall_name);
            break;
        case EACCES:
            fprintf(stderr, "Error in %s: Permission denied\n", syscall_name);
            break;
        case ENOENT:
            fprintf(stderr, "Error in %s: No such file or directory\n", syscall_name);
            break;
        case EBADF:
            fprintf(stderr, "Error in %s: Bad file descriptor\n", syscall_name);
            break;
        case ENOMEM:
            fprintf(stderr, "Error in %s: Out of memory\n", syscall_name);
            break;
        default:
            fprintf(stderr, "Error in %s: Unknown error (%d)\n", syscall_name, error_code);
            break;
    }
}

// Test cases
void run_test_cases(void) {
    printf("=== Running System Call Test Cases ===\n");
    
    // Test 1: sys_open
    printf("\nTest 1: Opening a file\n");
    int fd = syscall(SYS_OPEN, (int)"test.txt", 0);
    if (fd < 0) {
        handle_error(-fd, "open");
    } else {
        printf("File opened with FD: %d\n", fd);
    }
    
    // Test 2: sys_write
    printf("\nTest 2: Writing to a file\n");
    char *text = "Hello, System Calls!";
    int result = syscall(SYS_WRITE, fd, text);
    if (result < 0) {
        handle_error(-result, "write");
    } else {
        printf("Bytes written: %d\n", result);
    }
    
    // Test 3: sys_read
    printf("\nTest 3: Reading from a file\n");
    char buffer[100];
    result = syscall(SYS_READ, fd, buffer);
    if (result < 0) {
        handle_error(-result, "read");
    } else {
        printf("Bytes read: %d, Content: %s\n", result, buffer);
    }
    
    // Test 4: sys_time
    printf("\nTest 4: Getting system time\n");
    time_t current_time;
    result = syscall(SYS_TIME, 0, &current_time);
    if (result < 0) {
        handle_error(-result, "time");
    } else {
        printf("Current time: %s", ctime(&current_time));
    }
    
    // Test 5: sys_getpid
    printf("\nTest 5: Getting process ID\n");
    int pid = syscall(SYS_GETPID, 0, NULL);
    if (pid < 0) {
        handle_error(-pid, "getpid");
    } else {
        printf("Process ID: %d\n", pid);
    }
    
    // Test 6: sys_fork
    printf("\nTest 6: Forking process\n");
    int child_pid = syscall(SYS_FORK, 0, NULL);
    if (child_pid < 0) {
        handle_error(-child_pid, "fork");
    } else {
        printf("Child process ID: %d\n", child_pid);
    }
    
    // Test 7: sys_close
    printf("\nTest 7: Closing a file\n");
    result = syscall(SYS_CLOSE, fd, NULL);
    if (result < 0) {
        handle_error(-result, "close");
    } else {
        printf("File closed successfully\n");
    }
    
    // Test 8: Error case - bad file descriptor
    printf("\nTest 8: Error case - Bad file descriptor\n");
    result = syscall(SYS_WRITE, 99, "This should fail");
    if (result < 0) {
        handle_error(-result, "write");
    }
    
    printf("\n=== All test cases completed ===\n");
}

int main() {
    // Initialize process control block
    current_process.pid = 1234;
    for (int i = 0; i < 10; i++) {
        current_process.open_files[i] = 0;
    }
    
    // Initialize system call table
    init_syscall_table();
    
    printf("System Call Interface Development\n");
    printf("=================================\n");
    
    // Run test cases
    run_test_cases();
    
    // Demonstrate exit system call
    printf("\nDemonstrating exit system call...\n");
    syscall(SYS_EXIT, 0, NULL);
    
    // This won't be reached
    printf("This message won't be displayed\n");
    
    return 0;
}