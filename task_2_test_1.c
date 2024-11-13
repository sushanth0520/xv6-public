#include "types.h"
#include "stat.h"
#include "user.h"

// Integer to ASCII conversion function
void itoa(int num, char *str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        int temp = num;
        while (temp != 0) {
            str[i++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    str[i] = '\0';

    // Reverse the string to correct order
    for (int j = 0; j < i / 2; j++) {
        char swap = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = swap;
    }
}

// Function to run 'primeprocessor' with a given argument
void run_primeprocessor(int pipe_write) {
    char fd_str[10];
    itoa(pipe_write, fd_str);

    char *args[] = {"primeprocessor", "4", fd_str, 0};
    exec("primeprocessor", args);
    printf(1, "Execution failed: Unable to start the 'primeprocessor' process.\n");
    exit();
}

// Function to run 'ps' to display process status
void run_ps() {
    char *args[] = {"ps", 0};
    printf(1, "\nDisplaying the current process status using 'ps'...\n");
    exec("ps", args);
    printf(1, "Execution failed: Unable to run the 'ps' command.\n");
    exit();
}

int main() {
    int pid, pipe_fd[2];

    // Create a pipe for communication between processes
    if (pipe(pipe_fd) < 0) {
        printf(1, "Error: Failed to create the pipe for inter-process communication.\n");
        exit();
    }

    pid = fork();
    if (pid == 0) { // Child process to run 'primeprocessor'
        close(pipe_fd[0]); // Close the read end in the child
        run_primeprocessor(pipe_fd[1]);
    } else if (pid > 0) { // Parent process
        close(pipe_fd[1]); // Close the write end in the parent

        // Wait for the signal from primeprocessor
        char buf[5];
        if (read(pipe_fd[0], buf, 4) != 4) {
            printf(1, "Error: Failed to receive the expected signal from 'primeprocessor'.\n");
            exit();
        }
        buf[4] = '\0';

        // Check if the signal is 'done'
        if (strcmp(buf, "done") == 0) {
            printf(1, "\nThe 'primeprocessor' process has successfully launched all child processes.\n");
        } else {
            printf(1, "Unexpected message received from 'primeprocessor': %s\n", buf);
            exit();
        }

        close(pipe_fd[0]); // Close the read end after use

        // Run 'ps' in a new child process to display process status
        if (fork() == 0) {
            run_ps();
        }
        wait(); // Wait for the 'ps' command to complete
        wait(); // Wait for 'primeprocessor' to finish execution
    } else {
        printf(1, "Error: Fork operation failed.\n");
        exit();
    }

    exit();
}