//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

struct twoints{
	int first;
	int second;	
};


int main(int argc,char** argv){
	//megnyitás olvasásra
 //int handler = open("./twoInts.txt", O_CREAT|O_RDWR|S_IRUSR|S_IWUSR);
 FILE* f;
 f = fopen("./twoInts.txt", "rwb");
 
 
 struct twoints tI;
 tI.first = 2;
 tI.second = 3;
 
 fwrite(&tI.first, sizeof(tI), sizeof(tI), f);
 
 fclose(f);
 return 0;
}