#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
	printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[]){
	
	signal(SIGUSR1, handler);

	int pipefd[2];
	pid_t pid;
	char sz[100];

	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	
	if (pid == -1)  {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	
	// gyerek process
	if (pid == 0) {
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		close(pipefd[1]);

		int i;
		while(read(pipefd[0],sz,sizeof(sz))){
			printf("Gyerek olvasta uzenet: %s\n",sz);
			kill(getppid(), SIGUSR1);
		}


		printf("\n");
		close(pipefd[0]);
	} 
	
	// szulo process 
	else {
		printf("Szulo indul!\n");
		close(pipefd[0]); 

		int i;
		for(i = 0; i < 5; ++i){
			write(pipefd[1], "Hajra Fradi! \n",13);
			printf("Szulo beirta az adatokat a csobe!\n");
			pause();
		}

		close(pipefd[1]);

		fflush(NULL);
		wait();
		printf("Szulo befejezte!\n");	
	}
	
	exit(EXIT_SUCCESS);
}


