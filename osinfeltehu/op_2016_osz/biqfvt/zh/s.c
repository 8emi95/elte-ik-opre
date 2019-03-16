#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define SIZE 30

int fd1[2];
int fd2[2];

void feladatot_fogad() {
	char feladat[SIZE];
	char nev[SIZE];
	char sorszam[SIZE];
	close(fd1[1]);
	read(fd1[0], &feladat, SIZE);
	read(fd1[0], &nev, SIZE);
	read(fd1[0], &sorszam, SIZE);
	close(fd1[0]);
	printf("JATEKOS: nev: %s\n", nev);
	printf("JATEKOS: feladat: %s\n", feladat);
	printf("JATEKOS: sorszam: %s\n", sorszam);
}

void feladatot_megold() {
	int i;
	int siker = rand() % 2;
	char siker_str[SIZE];
	if(siker == 0) strcpy(siker_str, "nem sikerult kiszabadulni\n");
	if(siker == 1) strcpy(siker_str, "sikerult kiszabadulni\n");
	close(fd2[0]);
    write(fd2[1], &siker_str, SIZE);
	close(fd2[1]);
	kill(getppid(), SIGUSR1);
}

void kezelo() {
	char siker_str[SIZE];
	close(fd2[1]);
	read(fd2[0], &siker_str, SIZE);
	close(fd2[0]);
	printf("\nTRENER: ");
	printf(siker_str);
}

int main (int argc, char *argv[])
{
	int status;
	srand(time(NULL));
	if (argc < 2) {
		printf("Hianyzo arg.");
		exit(EXIT_FAILURE);
	}

	if (pipe(fd1)<0 || pipe(fd2)<0) {
		printf("Cso letrehozasa nem sikerult.");
		exit(EXIT_FAILURE);
	}
	
	pid_t child = fork();
	
	if(child > 0) {
		signal(SIGUSR1, kezelo);
		
		int i;
		char nev[SIZE];
		for(i=0;i<SIZE;i++) {
			nev[i] = argv[1][i];
		}
		char feladat[SIZE] = "feladat";
		char sorszam[SIZE] = "1";

		close(fd1[0]);
		write(fd1[1], &feladat, SIZE);
		write(fd1[1], &nev, SIZE);
		write(fd1[1], &sorszam, SIZE);
		close(fd1[1]);
		
		waitpid(child,&status,0);

	} else {
		feladatot_fogad();
		feladatot_megold();
	}
	
}