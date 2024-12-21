#include "types.h"
#include "user.h"
#include "fcntl.h"
int main(void){
 char *reading_file = "README";
 int fd = open(reading_file, O_CREATE|O_RDONLY); //opening file in read mode and then

 printf(fd, "Could we write here?\n");
 exit();
}