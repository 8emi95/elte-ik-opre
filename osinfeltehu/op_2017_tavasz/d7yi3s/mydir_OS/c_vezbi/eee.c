#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>

void handler(int sig){
	printf("second player (child) is ready to play!\n");
	signal(sig, handler);
}


int main(){
	signal(SIGUSR1, handler);
	
	srand(time(NULL));
	int parent_choice = rand() % 100 + 1;
	
	printf("first player choice: %i\n", parent_choice);
	
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
		
		kill(getppid(), SIGUSR1);
		close(pipe1[0]); 
		
		int child_guess = rand() % 100 + 1;
		
		write(pipe1[1], &child_guess, sizeof(int));
		close(pipe1[1]); 
		exit(0);
		
		
		
		
		
	}else if(child>0) {
		
		pause(); 
		close(pipe1[1]);
		
		int n;
		
		
		read(pipe1[0], &n, sizeof(int));
		
		printf("second player (child) choice: %i\n", n); 
		
		close(pipe1[0]);
		
		
		if(n == parent_choice){
			
		}
		
	}
	
	
	return 0;
}