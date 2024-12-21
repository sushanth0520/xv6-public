#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int process_id = fork();  // Create first child process
    
    if (process_id < 0) {  // Failed to create first child
        printf(1, "Failed to fork the first child.\n");
    } else if (process_id == 0) {  // In first child process
        printf(1, "First child is running\n");
    } else {  // In parent process
        process_id = fork();  // Create second child process
        
        if (process_id < 0) {  // Failed to create second child
            printf(1, "Failed to fork the second child.\n");
        } else if (process_id == 0) {  // In second child process
            printf(1, "Second child is running\n");
        } else {  // In parent process
            printf(1, "Parent is waiting for children to finish.\n");
            wait();  // Wait for first child to exit
            wait();  // Wait for second child to exit
            
            printf(1, "Both children have exited.\n");
            printf(1, "Parent is continuing execution.\n");
            printf(1, "Parent process is exiting.\n");
        }
    }

    exit();  // Exit the current process
}
