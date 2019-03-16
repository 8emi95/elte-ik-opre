#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

/*
	A szülő létrehoz 2 gyereket, majd ezek után bevárja a befejezésüket
*/

int main() {
	int status;

	// 1. gyerek	
		int child_1 = fork();
		if(child_1 < 0) {
			printf("Forkot nem sikerült létrehozni!\n");
			exit(1);
		}
		
		if(child_1 > 0) {
			// Szülő
		} else {
			// Gyerek
			printf("1. gyerek folyamata (%d)\n", getpid());
			printf("A szulo(%d)\n", getppid());
			sleep(3);
			exit(0); // Hogy az if utáni részt már ne csinálja a gyerek
		}
	
	
	// 2. gyerek
		int child_2 = fork();
		if(child_2 < 0) {
			printf("Forkot nem sikerült létrehozni!\n");
			exit(1);
		}
		
		if(child_2 > 0) {
			// Szülő
			
		} else {
			// Gyerek
			printf("2. gyerek folyamata (%d)\n", getpid());
			printf("A szulo(%d)\n", getppid());
			sleep(3);
			exit(0); // Hogy az if utáni részt már ne csinálja a gyerek
		}
	
	sleep(1);
	
	printf("A szülő az 1. gyerekre vár ... \n");
	waitpid(child_1, &status, 0);
	
	printf("A szülő a 2. gyerekre vár ... \n");
	waitpid(child_2, &status, 0);
	
	printf("Vege!\n");
	
	return 0;
}