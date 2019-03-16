#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
	int pipefd[2]; // unnamed pipe file descriptor array
	int pipefd2[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char sz[100];  // char array for reading from pipe
	char sz2[100];
	
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd2) == -1) 
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
		//sleep(3);	// sleeping a few seconds, not necessary
		//close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars 
		printf("Gyerek olvasta uzenet: %s",sz);
		printf("\n");
		close(pipefd[0]); // finally we close the used read end
		write(pipefd2[0], "Nem is...", 9);
		close(pipefd2[0]);
	} 
	else 
	{    // szulo process 
		printf("Szulo indul!\n");
		close(pipefd[0]); //Usually we close unused read end
		close(pipefd2[0]);
		write(pipefd[1], "Hajra Fradi!",13);
		//read(pipefd[0], sz, sizeof(sz));
		//printf("Ezt a valaszt kaptam: %s\n",sz);
		close(pipefd[1]); // Closing write descriptor 
		printf("Szulo beirta az adatokat a csobe!\n");
		read(pipefd2[1], sz2, sizeof(sz2));
		printf("Ezt a valaszt kaptam: %s\n",sz);
		close(pipefd2[1]);
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait();		// waiting for child process (not necessary)
		// try it without wait()
		printf("Szulo befejezte!");	
	}
	exit(EXIT_SUCCESS);	// force exit, not necessary
}