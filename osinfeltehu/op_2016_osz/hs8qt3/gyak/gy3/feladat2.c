#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <time.h>
//
// addig olvas a gyerek amig csak tud
//
int main(int argc, char *argv[])
{
	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char sz[100];  // char array for reading from pipe

	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork();	// creating parent-child processes
	if (pid == -1) 
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) 
	{	// child process
		close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		while(read(pipefd[0],sz,sizeof(sz)) != 0){
			printf("Gyerek olvasta uzenet: %s",sz);
			printf("\n");
		}
		//write(pipefd[1], "Nem is...", 9);
		//printf("\n");
		close(pipefd[0]); // finally we close the used read end
	} 
	else 
	{    // szulo process 
		printf("Szulo indul!\n");
		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], "Hajra Fradi!",13);
		srand(time(NULL));
		/*int randNumbers[3];
		int i=0;
		for(i<=randNumbers.length(); ++i){
			
		}*/
		//read(pipefd[0], sz, sizeof(sz));
		//printf("Ezt a valaszt kaptam: %s\n",sz);
		close(pipefd[1]); // Closing write descriptor 
		printf("Szulo beirta az adatokat a csobe!\n");
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait();		// waiting for child process (not necessary)
		// try it without wait()
		printf("Szulo befejezte!");	
	}
	exit(EXIT_SUCCESS);	// force exit, not necessary
}