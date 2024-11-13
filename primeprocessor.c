#include "types.h"
#include "stat.h"
#include "user.h"

int mutex;

void busy_work() {
    volatile int dummy = 0;
    for (volatile int i = 0; i < 2000000000; i++) {
        dummy += 2.71 * 42.0; // Consume CPU time
    }
}

void delay(int seconds) {
    int start_time = uptime();
    while ((uptime() - start_time) < (seconds * 100)) {
        // Wait for the specified duration
    }
}

int check_prime(int value) {
    if (value < 2) return 0;
    for (int divisor = 2; divisor * divisor <= value; divisor++) {
        if (value % divisor == 0) return 0;
    }
    return 1;
}

void find_primes(int count, int mutex) {
    int prime_candidate = 2;
    int found_primes = 0;

    while (found_primes < count) {
        if (check_prime(prime_candidate)) {
            lock(mutex);
            printf(1, "PID %d: Prime #%d: %d\n", getpid(), found_primes + 1, prime_candidate);
            unlock(mutex);

            found_primes++;
            delay(1); // Simulate some work for a second
        }
        prime_candidate++;
    }
}

int main(int argc, char *argv[]) {
    int processes = 1;

    if (argc > 1) {
        processes = atoi(argv[1]);
    }

    if (processes < 1) {
        processes = 1;
    }

    int child_pid;

    mutex = getmutex();
    if (mutex < 0) {
        printf(1, "Error: Failed to create mutex.\n");
        exit();
    }

    for (int i = 0; i < processes; i++) {
        child_pid = fork();
        if (child_pid < 0) {
            printf(1, "Error: Fork failed.\n");
            exit();
        } else if (child_pid == 0) {
            printf(1, "Child process %d started.\n", getpid());
            find_primes(1000, mutex);
            printf(1, "Child process %d completed.\n", getpid());
            exit();
        } else {
            printf(1, "Parent process %d created child process %d.\n", getpid(), child_pid);
        }
    }

    for (int i = 0; i < processes; i++) {
        wait();
    }

    exit();
}