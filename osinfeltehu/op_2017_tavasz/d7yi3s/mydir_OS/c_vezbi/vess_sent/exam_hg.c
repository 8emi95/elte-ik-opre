#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>

void handler(int sig){
	printf("Player 2 (child) is ready to play!\n");
	signal(sig, handler);
}


int main(){
	signal(SIGUSR1, handler);
	
	srand(time(NULL));
	int random_number = rand() % 100 + 1;
	
	printf("FIRST PLAYER choice: %i\n", random_number);
	
	int child;
	
	int pipe1[2];
	if(pipe(pipe1) == -1){
		perror("Error in creating pipe!\n");
		exit(1);
	}
	
	child = fork();
	
	if(child < 0){
		perror("Error fork child.");
		exit(1);
	}
	
	if(child == 0){
		//child process
		kill(getppid(), SIGUSR1);
		close(pipe1[0]); //child closes the reading end
		
		int child_guess = rand() % 100 + 1;
		
		write(pipe1[1], &child_guess, sizeof(int));
		close(pipe1[1]); //child closing the writing end
		exit(0);
		
		
		
		
		
	}else if(child>0) {
		//parent
		pause(); //waits for the signal of the child
		close(pipe1[1]); //parent closing writing end
		
		int n;
		
		read(pipe1[0], &n, sizeof(int));
		
		printf("Parent received this number: %i\n", n);
		
		close(pipe1[0]); //parent closing reading end
		
	}
	
	
	return 0;
}