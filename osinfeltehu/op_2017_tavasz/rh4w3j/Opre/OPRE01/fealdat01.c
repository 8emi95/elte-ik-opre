#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

	struct alma{
		char a[10];
		int b;
	};


int main(int argc,char** argv){
	


	struct alma csoki;
	csoki.b=3;
	strcpy(csoki.a,"almafa");
	
	 int f,g;
	 g=open(argv[1],O_RDONLY); 
	 
	 if (g<0){ perror("Error at opening the file\n");exit(1);}
	 
	if(read(g, &csoki, sizeof(a)));
	
	 
	 
	 
	 struct alma csoki2;
	 if(read(f,&csoki2,sizeof(struct alma csoki2)))
	 
	
//	printf("Szöveg: \'%s\'\n",csoki.a);
//	printf("Számol: \'%d\'\n",csoki.b);

	printf("Hamis");
	
	
	
	
	
	
	
	 return 0;
}