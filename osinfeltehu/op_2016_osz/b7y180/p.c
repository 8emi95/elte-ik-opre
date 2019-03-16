//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
struct stru{
	char nev[10];
	int jegy;
};
int main(int argc,char** argv){
 
 if (argc!=2){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
 int g;
 
 g=creat(argv[2],O_WRONLY);

 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 struct stru s;
 s.jegy = 5;
    if (write(g,&s,sizeof(s))!=sizeof(s)) 
      {perror("There is a mistake in writing\n");exit(1);}
      //write gives back the number of written bytes
      //1. parameter the file descriptor
      //2. parameter the address of variable we want to write out
      //3. parameter the number of bytes we want to write out 
 
 close(g);
 return 0;
}