#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
 char *s="Korte\n";
 int fd=open("ir.log",O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
 printf("Az open visszateres kodja: %d\n",fd);
 printf("A szoveg hossza: %i\n",sizeof(char*));
 write(fd,s,4);
 close(fd);
 return 0;
}
