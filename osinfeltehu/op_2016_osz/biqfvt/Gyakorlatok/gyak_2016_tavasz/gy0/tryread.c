//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>

int main(int argc,char** argv){
 
 int f; 
 f=open("result.dat",O_RDONLY); 
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 int number;
 while (read(f,&number,sizeof(number))){ 
    printf("%i",number); //we prints out the content of the file on the screen
 } 
 printf("\n");
 close(f);
 return 0;
}