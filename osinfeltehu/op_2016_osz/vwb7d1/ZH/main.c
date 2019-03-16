#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

int fd[2];

char feladat[256];
char nev[128];
int sorszam = 42;

int siker;


/**
* Signal handler
*/
void sigHandler(int signum) {
	close(fd[1]);
	printf(" -- A trener megkapta az eredmenyt --\n");
	read(fd[0], &siker, sizeof(siker));
	close(fd[0]);
	
	printf("A versenyzonek a feladat: ");
	if (siker == 1) {
		printf("sikerult\n");
	} else {
		printf("nem sikerult\n");
	}
}

/**
* Main
*/
int main(int argc, char **argv) {
	pid_t versenyzoPid;

	if (argc < 2) {
		printf("Tul keves argumentum!\n");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, sigHandler);
	srand(time(NULL));
	
	if (pipe(fd) == -1) {
		perror("Hiba a cso nyitasakor!");
		exit(EXIT_FAILURE);
	}

	// Fork
	versenyzoPid = fork();
	if (versenyzoPid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	
	// Versenyzo - trener szetvalasztasa
	if (versenyzoPid == 0) {
		// Versenyzo		
		sleep(1);
		printf(" -- Versenyzo olvas a csobol --\n");
		
		printf("Feladat beolvasasa\n");
		read(fd[0], feladat, sizeof(feladat));

		printf("Nev beolvasasa\n");
		read(fd[0], nev, sizeof(nev));

		printf("Sorszam beolvasasa\n");
		read(fd[0], &sorszam, sizeof(sorszam));
		
		printf("A versenyzo megkapta az adatokat.\n");
		
		printf("\n");
			printf("A kapott adatok:\n");
			printf(" - Feladat: %s\n", feladat);
			printf(" - Nev: %s\n", nev);
			printf(" - Sorszam: %i\n", sorszam);
		printf("\n");
		
		if (rand() % 10 < 5) {
			printf("A feladatot sikerult megoldanom, ugyes voltam!\n");
			siker = 1;
		} else {
			printf("A feladatot nem sikerult megoldanom, nem vagyok ugyes :(\n");
			siker = 0;
		}
		
		// Kuldes a trenernek
		printf("\nEredmeny kuldese a trenernek\n\n");
		write(fd[1], &siker, sizeof(siker));
		
		fflush(NULL);
		
		kill(getppid(), SIGUSR1);
	} else {
		// Trener		
		strcpy(feladat, "Oldd meg a kovetkezo feladatot!");
		strcpy(nev, argv[1]);
		
		printf("-- A trener elkuldi az adatokat a csobe --\n");
		
		printf("Feladat kuldese (%s)\n", feladat);
		write(fd[1], &feladat, sizeof(feladat));

		printf("Nev kuldese (%s)\n", nev);
		write(fd[1], &nev, sizeof(nev));

		printf("Sorszam kuldese (%i)\n", sorszam);
		write(fd[1], &sorszam, sizeof(sorszam));
		
		printf("A trener vegzett az adatok elkuldesevel!\n\n");
		
		fflush(NULL);
		
		pause();
	}

	return 0;
}