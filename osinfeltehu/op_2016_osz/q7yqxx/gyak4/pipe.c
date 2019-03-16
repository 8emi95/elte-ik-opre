#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	int pipefd[2]; 
	pid_t pid;
	char sz[100];

	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	// child process
	if (pid == 0) {
		sleep(1);
		close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
		printf("Gyerek olvasta uzenet: %s",sz);
		printf("\n");
		close(pipefd[0]); // finally we close the used read end
	}

	// szulo process 
	else {
		printf("Szulo indul!\n");
		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], "Hajra Fradi!",13);
		close(pipefd[1]); // Closing write descriptor 
		printf("Szulo beirta az adatokat a csobe!\n");
		fflush(NULL);
		wait();
		printf("Szulo befejezte!\n");	
	}
	
	exit(EXIT_SUCCESS);
}

 
