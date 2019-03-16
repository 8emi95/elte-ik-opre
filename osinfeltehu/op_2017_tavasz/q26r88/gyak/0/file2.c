#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct struktura {
    int		szam;
    char 	nev[50]; 
};

int main(int argc,char** argv){
 
 if (argc!=2){perror("You have to use program with 1 arguments");exit(1);}
 int f,g;
 
 struct struktura adat;
 
 adat.szam = 12;
 strcpy(adat.nev,"Gaál Tamás");
  
 g=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 if (write(g,&adat,sizeof(adat)) != sizeof(adat))
	 {perror("There is a mistake in writing\n");exit(1);}
 
 close(g);
 
 f=open(argv[1],O_RDONLY);
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 struct struktura adatbe;
 read(f,&adatbe,sizeof(adatbe));
 
 printf("%s %i \n",adatbe.nev,adatbe.szam);
 
 close(f);
 return 0;
}