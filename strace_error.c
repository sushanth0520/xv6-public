#include <stdio.h>
int main(void){
FILE * fp;
fp = fopen ("README", "r");
fprintf(fp, "Could we write here?\n");
fclose(fp);
return 0;
}