#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

struct Kerdes {
	char kerdes[100];
	char valasz[50];
};

int main(int argc, char *argv[]) {
	int pfd[2], n, m;                              /* pipe változó, olvasott bájtok száma */
	int c2p[2];
	pid_t pid;

	char buf[MAXLINE];                          /* ebbe fogja kiolvasni a gyerek az inf. */
	
	struct Kerdes masolat;
	struct Kerdes elso;
	strcpy(elso.kerdes, "Mi ez?");
	strcpy(elso.valasz, "Ez a valasz.");

	char test[] = "pipe teszt\n";               /* ezt küldi a gyerek */

	if (pipe(pfd) < 0)                          /* pipe létrehozása */	
		printf("Pipe hiba!\n");
		
	if (pipe(c2p) < 0)                          /* pipe létrehozása */	
		printf("Pipe hiba!\n");

	if ((pid = fork()) < 0)                     /* gyerekfolyamat létrehozása */
		printf("fork hiba");
	else
		if (pid == 0)                             /* gyerek */
		{
		  close(pfd[1]);                          /* fölösleges vezeték bezárása */
		  n = read(pfd[0], &masolat, sizeof(struct Kerdes));         /* információ kiolvasása */
		  printf("beolvastam %d bajtot: \n", n);
		  fflush(stdout);
		  printf("Ezt kaptam: %s \n", masolat.kerdes);
		  printf("Valasz: %s \n", masolat.valasz);
		  
		  char val[50] = "kalap";
		  
		  close(c2p[0]);
		  write(c2p[1], val, sizeof(val));
		  
		}
		else                                      /* szülő */
		{
		  close(pfd[0]);                          /* fölösleges vezeték bezárása */
		  write(pfd[1], &elso, sizeof(struct Kerdes));      /* információ beírása a pipe-ba */
		  
		  char val_masol[50];
		  close(c2p[1]);
		  m = read(c2p[0], &val_masol, sizeof(val_masol));
		  printf("parent vagyok ezt kaptam: %d bajt\n", m);
		  fflush(stdout);
		  printf("par olvastam: %s\n", &val_masol);
		}
  exit(0);

}