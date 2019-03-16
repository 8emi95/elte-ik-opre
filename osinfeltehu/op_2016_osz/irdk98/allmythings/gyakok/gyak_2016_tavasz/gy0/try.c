//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>

int main(int argc,char** argv){
 int g ; 
 g=open("result.dat",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 int number;
 int i;
  
for (i=1;i<=5;i++){
 scanf("%i",&number);
 write(g,&number,sizeof(number)); 
}

 close(g);
 return 0;
}