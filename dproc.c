#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int child_pid;
  int num_processes, counter;
  int computation, temp;

  // Check and parse arguments
  if (argc < 2)
    num_processes = 1; // Default value
  else
    num_processes = atoi(argv[1]);
  
  // Ensure the number of processes is within a valid range
  if (num_processes < 0 || num_processes > 20)
    num_processes = 2;

  computation = 0; // Placeholder for calculations
  child_pid = 0;   // Initialize child process ID

  // Create the specified number of processes
  for (counter = 0; counter < num_processes; counter++) {
    child_pid = fork();

    if (child_pid < 0) {
      printf(1, "Fork failed for process %d\n", getpid());
    } else if (child_pid > 0) {
      // Parent process logic
      printf(1, "Parent process %d spawned child process %d\n", getpid(), child_pid);
      wait(); // Wait for child to finish execution
    } else {
      // Child process logic
      printf(1, "Child process %d started\n", getpid());
      for (temp = 0; temp < 2500000000; temp++) {
        computation += 1.57 * 42.73; // Simulate CPU workload
      }
      break; // Prevent child from spawning further processes
    }
  }

  exit();
}
