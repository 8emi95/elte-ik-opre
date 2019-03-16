#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

int main() {
	int number = 1;
	
	int status;
	int child;
	
	child = fork();
	if(child < 0) {
		printf("ERROR\n");
		exit(-1);
	}
	
	if(child > 0) {
		// Szülõ
		
		printf("Szulo kezdete!\n");
		printf("Szulo a gyerekre var ...\n");
		waitpid(child, &status, 0);
		printf("Szulo: gyereknek vege!\n");
		printf("Number: %d\n", number);
	} else {
		// Gyerek
		printf("Gyerek kezdete!\n");
		number = 5;
		printf("Gyerek vege!\n");
		exit(0);
	}
	
	printf("Vege\n");
	
	return 0;
}