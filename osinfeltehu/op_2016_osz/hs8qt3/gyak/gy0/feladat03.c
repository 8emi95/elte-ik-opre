//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct adat{
	char nev[25];
	int jegy;
};

int main(int argc,char** argv){
 int g;
 
 struct adat a1;
 strcpy(a1.nev, "hbv");
 a1.jegy = 17;
 
 g=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}

 write(g,&a1,sizeof(a1));
 
 close(f);
 close(g);
 return 0;
}