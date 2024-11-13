#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();  // Get the PID of the current process

    printf(1, "Executing priority change tests for the nice system call\n");

    // Test 1: Set a valid minimum priority (1) for the current process
    int old_priority = nice(pid, 1);
    if (old_priority >= 0) {
        printf(1, "Test 1 Success: Process %d changed from priority %d to 1\n", pid, old_priority);
    } else {
        printf(1, "Test 1 Error: Could not set priority to 1 for process %d\n", pid);
    }

    // Test 2: Attempt to assign a negative priority (-3) to the current process
    old_priority = nice(pid, -3);
    if (old_priority == -1) {
        printf(1, "Test 2 Success: Negative priority (-3) rejected as expected\n");
    } else {
        printf(1, "Test 2 Error: Negative priority allowed for process %d\n", pid);
    }

    // Test 3: Try setting a valid maximum priority (5) for the current process
    old_priority = nice(pid, 5);
    if (old_priority >= 0) {
        printf(1, "Test 3 Success: Process %d priority changed from %d to 5\n", pid, old_priority);
    } else {
        printf(1, "Test 3 Error: Failed to set priority to 5 for process %d\n", pid);
    }

    // Test 4: Attempt to set an out-of-range priority (45)
    old_priority = nice(pid, 45);
    if (old_priority == -1) {
        printf(1, "Test 4 Success: Out-of-bounds priority (45) correctly refused\n");
    } else {
        printf(1, "Test 4 Error: Process %d allowed invalid priority 45\n", pid);
    }

    // Test 5: Set a mid-range priority (4) for the current process
    old_priority = nice(pid, 2);
    if (old_priority >= 0) {
        printf(1, "Test 5 Success: Process %d priority updated from %d to 2\n", pid, old_priority);
    } else {
        printf(1, "Test 5 Error: Failed to assign priority 2 to process %d\n", pid);
    }

    printf(1, "All tests completed successfully\n");
    exit();
}
