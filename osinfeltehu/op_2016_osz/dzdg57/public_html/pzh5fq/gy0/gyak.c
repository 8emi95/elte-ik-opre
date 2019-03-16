//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct coords{
   char x[10];
   char y[10];
};

int main(int argc,char** argv){
 int handle = 0;
 handle = open("a.txt", O_RDWR|O_CREAT, S_IRUSR);
 struct coords cor;
 printf("%i\n", sizeof(cor) );
 //cor.x = malloc(10* sizeof(char));
// cor.y = malloc(10* sizeof(char));
 printf("%i\n", sizeof(cor));
 strcpy(cor.x, "100000");
 strcpy(cor.y, "100000");
 write(handle, &cor, 20*sizeof(char));
 /*char c;
 int returncode = 1;
 while (returncode != 0)
 {
  returncode = read(handle, &c, sizeof(c));
  printf("%c", c);
 }
 printf("\n");*/
 return 0;
}
