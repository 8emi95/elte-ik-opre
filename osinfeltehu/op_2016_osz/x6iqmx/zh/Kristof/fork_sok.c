#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

/*
	A szülõ létrehoz 2 gyereket, majd ezek után bevárja a befejezésüket
*/

int main() {
	int status;
	int processzek = 3;
	
	int child[processzek];
	
	int i;
	for(i = 0; i < processzek; i++) {
		child[i] = fork();
		
		if(child[i] < 0) {
			printf("HIBA!");
			exit(1);
		}
		
		if(child[i] > 0) {
			// Szülõ
		} else {
			// Gyerek
			printf("Gyerek %d: (%d)\n", i, getpid());
			exit(0); // Kilépés a gyerekbõl
		}
	}
	
	for(i = 0; i < processzek; i++) {
		waitpid(child[i], &status, 0);
	}
}