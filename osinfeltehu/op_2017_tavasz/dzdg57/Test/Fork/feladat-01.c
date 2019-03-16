#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

int main() {
	pid_t gy1, gy2;
	
	gy1 = fork();
	
	// ha nem nulla, tehát a szülõ (nulla az a gyerek)
	if(gy1) {
		gy2 = fork();
		// itt ugyanaz a logika
		if(gy2) {
			int status;
			printf("Szulo: 1. gyerek: %i\nSzulo: 2. gyerek: %i\n", gy1, gy2);
			printf("waitpid (2): %i\n", waitpid(gy2, &status, 0));
			printf("waitpid -1: %i\n", waitpid(-1, &status, 0));
			printf("waitpid bash %i\n", waitpid(getppid(), &status, 0));
			printf("waitpid -1: %i\n", waitpid(-1, &status, 0));
		}
		else {
			sleep(5);
			printf(" 2. gyerek %i\n 2. gyerek szuloje: %i\n", getpid(), getppid());		
		}
	}
	else {
		printf(" 1. gyerek: %i\n 1. gyerek szuloje: %i\n", getpid(), getppid() );
	}
}