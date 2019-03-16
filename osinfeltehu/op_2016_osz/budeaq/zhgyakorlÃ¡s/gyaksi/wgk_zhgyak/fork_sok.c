#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

/*
	A sz�l� l�trehoz 2 gyereket, majd ezek ut�n bev�rja a befejez�s�ket
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
			// Sz�l�
		} else {
			// Gyerek
			printf("Gyerek %d: (%d)\n", i, getpid());
			exit(0); // Kil�p�s a gyerekb�l
		}
	}
	
	for(i = 0; i < processzek; i++) {
		waitpid(child[i], &status, 0);
	}
}