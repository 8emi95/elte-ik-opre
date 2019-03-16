#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>


void handler(int sig){
	signal(sig, handler);
}


int main()
{
	signal(SIGUSR1, handler);
	srand(time(NULL));
	int r = rand() % 100 + 1;
	printf("Parent gives a random number: %i\n", r);
	int child;
	int pipefd[2];


	if(pipe(pipefd) == -1){
		perror("PIPE error\n");
		exit(1);
	}
	
	child = fork();
	
	if(child < 0){
		perror("Error fork child!");
		exit(1);
}
if (child == 0){
		//PARENT (FIRST PLAYER)
		kill(getppid(), SIGUSR1);
		close(pipefd[0]); 
		int r1 = rand() % 100 + 1;
		write(pipefd[1], &r1, sizeof(int));
		printf("First player gives bigger RN than second player!");
		close(pipefd[1]); 
		exit(0);
		
}

else if(child > 0) {
		//CHILD (SECOND PLAYER)
		pause(); 
		close(pipefd[1]);
		int n;
		read(pipefd[0], &n, sizeof(int));
		printf("Child gives random number: %i\n", n); 
		close(pipefd[0]);
		
	}
	
	
	return 0;
}