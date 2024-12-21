#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void) {
    char *file_path = "README";
    int file_desc = open(file_path, O_CREATE | O_RDONLY);
    printf(1, "File Descriptor: %d\n", file_desc);

    // Try to write to the file
    int result = write(file_desc, "Test Write\n", -1);
    printf(1, "Write Operation Result: %d\n", result);

    close(file_desc);
    exit();
}

