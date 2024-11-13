#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// Function to validate and parse priority input
int validate_priority(const char *input) {
    int parsed_priority = 0;
    int index = 0;

    // Ensure the input contains only numeric characters
    while (input[index] != '\0') {
        if (input[index] < '0' || input[index] > '9') {
            return -1; // Return -1 if a non-numeric character is found
        }
        parsed_priority = parsed_priority * 10 + (input[index] - '0');
        index++;
    }

    // Ensure the priority falls within the allowed range (1-5)
    if (parsed_priority < 1 || parsed_priority > 5) {
        return -1;
    }

    return parsed_priority;
}

int main(int argc, char *argv[]) {
    int target_pid, new_priority, previous_priority;

    // Validate arguments
    if (argc == 2) {
        // Use current process if only priority is provided
        target_pid = getpid();
        new_priority = validate_priority(argv[1]);
    } else if (argc == 3) {
        // Use specified PID if both PID and priority are provided
        target_pid = atoi(argv[1]);
        new_priority = validate_priority(argv[2]);
    } else {
        // Print usage message for incorrect argument count
        printf(2, "Usage: set_priority [pid] priority\n");
        exit();
    }

    // Check if priority validation failed
    if (new_priority == -1) {
        printf(2, "Error: Priority must be a number between 1 and 5.\n");
        exit();
    }

    // Attempt to update the process priority using the system call
    previous_priority = nice(target_pid, new_priority);

    if (previous_priority == -1) {
        // Handle error if the process is not found or priority change fails
        printf(2, "Error: Unable to set priority for PID %d. Process may not exist.\n", target_pid);
    } else {
        // Print success message with PID and the previous priority
        printf(1, "PID: %d, Previous Priority: %d\n", target_pid, previous_priority);
    }

    exit();
}
