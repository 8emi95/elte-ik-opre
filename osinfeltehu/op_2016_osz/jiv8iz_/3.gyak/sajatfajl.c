//CSináljunk egy fájl és írjuk ki bele a nevünket
//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

void fv(int handle){
	char nev[] = "Tanka Aniko";
	write(handle,&nev ,sizeof(nev));
	
}




int main(int argc,char** argv){
 int handle = open("./nev.txt",O_RDW|O_CREAT,S_IRUSR|S_IWUSR);
 fv(handle);
 
 
 close(handle);
 return 0;
}
