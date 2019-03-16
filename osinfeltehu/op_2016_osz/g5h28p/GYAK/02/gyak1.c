//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open file megnyitas
#include <sys/stat.h>
#include <errno.h> //perror, errno

//egy file kiolvasása és kiírsa karakterenként addig olvassuk amig vege a
// a filenak errol a returnValue gondoskodik

int main(int argc,char** argv){
	int handle =open("./text.txt", O_RDONLY); 
    //megmondani mivel akarunk foglalkozni es mit akarunk csinalni (open - visszateres int)
 	char c;
 	int returnValue = 1;
 	while (returnValue != 0)
 	{
 		returnValue = read(handle, &c, sizeof(c));
 		printf("%c", c);
 	}
 	printf("\n");	
 	//read(handle, &c, sizeof(c));
 	//printf("%c\n", c); 
 	close(handle);
 return 0;
}
