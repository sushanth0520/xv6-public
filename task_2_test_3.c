#include "types.h"
#include "stat.h"
#include "user.h"

// Function to convert an integer to a string
void int_to_string(int num, char *str) {
    int idx = 0;
    if (num == 0) {
        str[idx++] = '0';
    } else {
        int temp = num;
        while (temp != 0) {
            str[idx++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    str[idx] = '\0';

    // Reverse the string to get the correct order
    for (int i = 0; i < idx / 2; i++) {
        char temp = str[i];
        str[i] = str[idx - i - 1];
        str[idx - i - 1] = temp;
    }
}

// Function to run the 'primeprocessor' program in a child process
void run_primeprocessor(int pipe_write) {
    char write_fd_str[10];
    int_to_string(pipe_write, write_fd_str);

    char *args[] = {"primeprocessor", "3", write_fd_str, 0};
    exec("primeprocessor", args);
    printf(1, "Failed to execute 'primeprocessor' program.\n");
    exit();
}

// Function to run the 'nice' command in a child process
void run_nice() {
    char *args[] = {"nice", "5", "1", 0};
    printf(1, "\nInitiating 'nice 5 1' command...\n");
    exec("nice", args);
    printf(1, "Execution of 'nice' command failed.\n");
    exit();
}

int main() {
    int child_pid;
    int signal_pipe[2];

    // Create a pipe for communication
    if (pipe(signal_pipe) < 0) {
        printf(1, "Unable to create pipe for communication.\n");
        exit();
    }

    // Fork a child process to run 'primeprocessor' in the background
    printf(1, "Launching 'primeprocessor 3 &' as a background task.\n");
    child_pid = fork();
    if (child_pid == 0) { // Child process to execute primeprocessor
        close(signal_pipe[0]); // Close read end in the child process
        run_primeprocessor(signal_pipe[1]);
    } else if (child_pid > 0) { // Parent process
        close(signal_pipe[1]); // Close write end in the parent process

        // Wait for the "done" signal from primeprocessor
        char signal_buffer[5];
        int bytes_read = read(signal_pipe[0], signal_buffer, 4);
        if (bytes_read != 4) {
            printf(1, "Error: Expected signal from 'primeprocessor' not received.\n");
            exit();
        }
        signal_buffer[4] = '\0';

        // Verify the received signal
        if (strcmp(signal_buffer, "done") == 0) {
            printf(1, "\nThe 'primeprocessor' process has successfully launched all child processes.\n");
        } else {
            printf(1, "Unexpected signal received from 'primeprocessor': %s\n", signal_buffer);
            exit();
        }
        close(signal_pipe[0]); // Close the read end after use

        // Run the 'nice' command in a new child process
        if (fork() == 0) {
            run_nice();
        }
        wait(); // Wait for the nice command to complete

        // Wait for the 'primeprocessor' process to finish
        wait();
    } else {
        printf(1, "Error: Unable to create a new process (fork failed).\n");
        exit();
    }

    exit();
}