//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main(int argc,char** argv){
	//megnyitás olvasásra
 int handler = open("./text.txt", O_RDONLY);
 char c;
 int reader = 1;
 while (reader != 0){
	reader = read(handler, &c, sizeof(c));
	printf("%c", c);
 }
 printf("\n");
 close(handler);
 return 0;
}
