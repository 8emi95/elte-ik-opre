#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

struct EgyStruct {
	char x[100];
	int szam;
};


int main(int argc,char** argv){
	struct EgyStruct a;
	
	strcpy(a.x,"egy egy csodalatos stringsdfhasdfhjdghasdffaghsdfagsdfagsdfaghsdfgasfdgadgasdgafdg");
	a.szam = 1;
	
	int f,g;
	
	f = open("test.txt",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if (f<0){ perror("Error at opening the file\n");exit(1);}
	if(write(f,&a,sizeof(struct EgyStruct)) != sizeof(struct EgyStruct)) {
		printf("NEM IRTAM A FILEBA MERT ROSSZ GONOSZ C PROGRAM VAGYOK MUAHAHAHAHAHAHAHAHAHAHA\n");
	}
	close(f);
	
	g=open("test.txt",O_RDONLY);
	if (g<0){ perror("Error at opening the file\n");exit(1);}
	
	struct EgyStruct b;
	if(read(g,&b,sizeof(struct EgyStruct)) == 0){
		printf("NEM OLVASTAM A FILEBOL MERT ROSSZ GONOSZ C PROGRAM VAGYOK MUAHAHAHAHAHAHAHAHAHAHA\n");
	}
	
	
	printf("%s\n%i\n",b.x,b.szam);
	 
	close(g);
	return 0;
}