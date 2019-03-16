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
	int child_1 = fork();
	
	if(child_1 < 0) {
		exit(-1);
	}
	
	if(child_1 > 0) {
		// Szülõ
		int child_2 = fork();
		if(child_2 < 0) {
			exit(-2);
		}
		
		if(child_2 > 0) {
			// Szülõ
			waitpid(child_1, &status, 0);
			waitpid(child_2, &status, 0);
			printf("Szulo\n");
		} else {
			// 2. gyerek
			waitpid(child_1, &status, 0);
			printf("2. gyerek\n");
			exit(0);
		}
	} else {
		// 1. gyerek
		printf("1. gyerek\n");
		exit(0);
	}
	
	return 0;
}