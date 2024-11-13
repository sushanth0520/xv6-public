#include "types.h"
#include "stat.h"
#include "user.h"

// Integer to ASCII conversion function
void int_to_string(int num, char *str) {
    int idx = 0;
    if (num == 0) {
        str[idx++] = '0';
    } else {
        int temp_num = num;
        while (temp_num != 0) {
            str[idx++] = (temp_num % 10) + '0';
            temp_num /= 10;
        }
    }
    str[idx] = '\0';

    // Reverse the string
    for (int i = 0; i < idx / 2; i++) {
        char temp = str[i];
        str[i] = str[idx - i - 1];
        str[idx - i - 1] = temp;
    }
}

// Run 'primeprocessor' with a given argument
void run_primeprocessor(int pipe_write) {
    char write_fd_str[10];
    int_to_string(pipe_write, write_fd_str);

    char *args[] = {"primeprocessor", "3", write_fd_str, 0};
    exec("primeprocessor", args);
    printf(1, "Execution failed: Unable to start the 'primeprocessor' process.\n");
    exit();
}

// Run 'nice' command with specified arguments
void run_nice() {
    char *args[] = {"nice", "5", "5", 0};
    printf(1, "\nStarting the 'nice 5 5' command...\n");
    exec("nice", args);
    printf(1, "Execution failed: Unable to execute the 'nice' command.\n");
    exit();
}

int main() {
    int child_pid;
    int status_pipe[2];

    // Create a pipe for inter-process communication
    if (pipe(status_pipe) < 0) {
        printf(1, "Error: Failed to create a status communication pipe.\n");
        exit();
    }

    // Fork a new process to run 'primeprocessor 3 &' in the background
    printf(1, "Launching the background process 'primeprocessor 3 &'...\n");
    child_pid = fork();
    if (child_pid == 0) { // Child process to run primeprocessor
        close(status_pipe[0]); // Close the read end in child
        run_primeprocessor(status_pipe[1]);
    } else if (child_pid > 0) { // Parent process
        close(status_pipe[1]); // Close the write end in parent

        // Wait for the signal from primeprocessor
        char signal_buf[5];
        int bytes_read = read(status_pipe[0], signal_buf, 4);
        if (bytes_read != 4) {
            printf(1, "Error: Did not receive the expected signal from 'primeprocessor'.\n");
            exit();
        }
        signal_buf[4] = '\0';

        // Verify the received signal
        if (strcmp(signal_buf, "done") == 0) {
            printf(1, "\nThe 'primeprocessor' process has successfully launched all child processes.\n");
        } else {
            printf(1, "Unexpected signal received from 'primeprocessor': %s\n", signal_buf);
            exit();
        }
        close(status_pipe[0]); // Close the read end

        // Run 'nice' in a new child process
        if (fork() == 0) {
            run_nice();
        }
        wait(); // Wait for 'nice' command to complete

        // Wait for 'primeprocessor' to finish
        wait();
    } else {
        printf(1, "Error: Unable to create a new process (fork failed).\n");
        exit();
    }

    exit();
}