#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    printf(1, "Executing tests for the `nice` command-line interface.\n");

    // Test 1: Assign a valid priority (5) to the current process
    printf(1, "Setting priority to 5 for the current process.\n");
    if (fork() == 0) {
        char *args[] = {"nice", "5", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 2: Attempt to set an invalid priority (7) for the current process
    printf(1, "Trying to assign an invalid priority (7).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "7", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 3: Attempt to set a negative priority (-2) for the current process
    printf(1, "Attempting to set a negative priority (-2).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "-2", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 4: Assign a valid priority (1) to the current process
    printf(1, "Setting priority to 1 for the current process.\n");
    if (fork() == 0) {
        char *args[] = {"nice", "1", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 5: Attempt to assign an invalid non-numeric value (c) as priority
    printf(1, "Testing with non-numeric priority (c).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "c", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 6: Provide two arguments; first is PID (1) and second is an invalid priority (8)
    printf(1, "Setting invalid priority (8) for PID 1.\n");
    if (fork() == 0) {
        char *args[] = {"nice", "1", "8", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 7: Assign valid priority (3) to a specific PID (5)
    printf(1, "Assigning priority 3 to PID 5.\n");
    if (fork() == 0) {
        char *args[] = {"nice", "5", "3", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 8: Test with invalid argument (special character "@")
    printf(1, "Trying to set priority with invalid input (@).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "@", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 9: Test assigning valid priority (2) to a non-existent PID (430)
    printf(1, "Setting priority 5 for a non-existent PID (430).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "430", "2", 0};
        exec("nice", args);
        exit();
    }
    wait();

    // Test 10: Assign an invalid priority (0) to the current process
    printf(1, "Testing with an invalid priority (0).\n");
    if (fork() == 0) {
        char *args[] = {"nice", "0", 0};
        exec("nice", args);
        exit();
    }
    wait();

    printf(1, "All tests completed.\n");
    exit();
}
