#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno


int main(int argc,char** argv){
    printf("Melyik rendezvenyre jelentkezik?\n");

    char strvar[100];
    fgets (strvar, 100, stdin);

    FILE *f = fopen(strvar, "w");
    if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}
    const char *text = "Write this to the file";
    fprintf(f, "Some text: %s\n", text);
    fclose(f);

}
