#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
	//printf("Signal with number %i has arrived\n",signumber);
}

int main() {
	signal(SIGUSR1,handler);
	int status;
	
	int child = fork();
	
	if(child < 0) {
		printf("Hiba a gyerek letrehozasakor!\n");
		exit(1);
	}
	
	if(child > 0) {
		// Szülő
		int i;
		for(i = 1; i <= 5; i++) {
			pause();
			printf("%d\n", i);
		}
		sleep(5);
		waitpid(child, &status, 0);
	} else {
		// Gyerek
		int parent = getppid();
		
		int i;
		for(i = 1; i <= 5; i++) {
			sleep(1);
			kill(parent, SIGUSR1);
		}
		
		exit(0);
	}
	return 0;
}