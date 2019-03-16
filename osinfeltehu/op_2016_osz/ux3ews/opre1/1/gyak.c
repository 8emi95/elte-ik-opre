#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main(int argc,char** argv){
 int handle = open("./text.txt", O_RDONLY );
 char c;
 int returnValue = 1;
 while( returnValue != 0)
 {
	 returnValue = read(handle, &c, sizeof(c));
	 printf("%c",c);
 }
 
 printf("%\n", c);
 close(handle);
 return 0;
}