//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

void fvout(int handler){
	char name[] = "Asd dsa";
	write(handler, &name, sizeof(name));
	
}

int main(int argc,char** argv){
	//megnyitás olvasásra
 int handler = open("./nev.txt", O_CREAT|O_RDWR|S_IRUSR|S_IWUSR);
 fvout(handler);
 
 
 close(handler);
 return 0;
}