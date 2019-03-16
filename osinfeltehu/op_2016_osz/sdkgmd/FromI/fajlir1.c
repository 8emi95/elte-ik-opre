#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	// open-hez, param�terekhez kell
#include <sys/stat.h>  // open-hez kell
#include <string.h>
#include <unistd.h>	// read utasítás kivanja
//
struct csapat
{
	int alapitas;
	char* nev;
};
//
struct edzo
{
	int kor;
	char nev[20];
	char* aneve;
	char* bneve; // +8 bajt
};
/*
	   int fd; 
	// O_CREAT �s O_EXCL egy�tt hib�t dob, ha m�r l�tezik a file
       if ((fd = open(LOCKFILE, O_WRONLY | O_CREAT | O_EXCL,  // exkluz�v megnyit�s
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
       {
          fprintf(stderr, "Cannot open LOCKFILE. Try again later.\n");
          exit(1);
       }
*/
int main()
{
 char *s="Almafa";
 int fd=open("ir.log",O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR|S_IWGRP);
 printf("Az open visszateres kodja: %d\n",fd);
 write(fd,s,7);
 close(fd);
// 
 struct csapat fradi,ujpest;
 printf("A csapat struktúra mérete: %u\n",sizeof(struct csapat));
 printf("Az edző struktúra mérete: %u\n",sizeof(struct edzo));
 printf("A karakter mutató mérete: %u\n",sizeof(char*)); // 8 bajt
 char* n="FTC";
 fradi.nev=malloc(strlen(n)+1);
 strcpy(fradi.nev,n);
 fradi.alapitas=1892; 
 ujpest.alapitas=1949;
 char u[]="Ujpest";
 ujpest.nev=malloc(strlen(u)+1);
 strcpy(ujpest.nev,u);
 fd=open("ir1.log",O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR|S_IRWXG|S_IRWXO);
 write(fd,&fradi,sizeof(struct csapat));
 write(fd,&ujpest,sizeof(struct csapat));
 close(fd);
//
 struct csapat x;
 fd=open("ir1.log",O_RDONLY);
 while (read(fd,&x,sizeof(struct csapat)))
 {
 	//read(fd,&y,sizeof(struct csapat));
 	printf("A beolvasott évszám: %d\n",x.alapitas);
 	printf("A csapat neve: %s \n",x.nev);
 }
 close(fd);
 return 0;
}
