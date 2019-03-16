#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // DateTime kezeles
#include <fcntl.h> // Rendszerhivasok file
#include <unistd.h> // fork
#include <sys/types.h> // pid_t típus
#include <sys/wait.h> // wait, waitpid
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

//////////////////////////////////
// IMPORTANT!!!!!!!!!!!!!!!!!!!!!!
// GEANY BUILD COMMAND (for SEMs):
// gcc -Wall -pthread -o "%e" "%f"
//////////////////////////////////

typedef enum {FALSE, TRUE} bool;
int pipes[2][2];
char continueMessage[] = "-1";

//////////////////
// Signal handling
//////////////////
void HandlerParent(int signalNumber){
	printf("[PARENT] Parent received signal: %d\n", signalNumber);
}

int main(int argc, char *argv[])
{
	pid_t pid1;
	signal(SIGUSR2, HandlerParent);
	// Parent prep 1 code
	printf("[PARENT] Parent started! PID: %d\n", getpid());

	// Pipes
	if (pipe(pipes[0]) == -1) 
	{
		perror("[ERROR] Pipe2 is not created!\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipes[1]) == -1) 
	{
		perror("[ERROR] Pipe1 is not created!\n");
		exit(EXIT_FAILURE);
	}

	// Main Code
	printf("[PARENT] Child1 is starting!\n");
	pid1 = fork();
	if(pid1 == -1)
	{
		perror("[PARENT] Child1: Fork error!\n");
	    exit(EXIT_FAILURE);
	}
	else if(pid1 == 0)
	{
		// Child1 code
		printf("[CHILD1] Child1 is running! PID: %d\n", getpid());
		sleep(1);
		printf("[CHILD1] Sending READY signal to parent: %d\n", getppid());
		kill(getppid(), SIGUSR2);
		
		char inMessage[100];
		do
		{
			printf("DO KEZDES!\n");
			//close(pipes[1][1]);
			read(pipes[1][0], inMessage, sizeof(inMessage));
			printf("[CHILD1] Pipe message arrived: %s\n", inMessage);
			if(atoi(inMessage) >= 0)
			{
				char outMessage[] = "-2";
				close(pipes[0][0]);
				write(pipes[0][1], outMessage, sizeof(outMessage));
				close(pipes[0][1]);
				fflush(NULL);
				printf("[CHILD1] CONTINUE message sent to the parent!\n");
			}
			else if(atoi(inMessage) == -2)
			{
				printf("[CHILD1] TERMINATING");
			}
		}
		while(atoi(inMessage) > -2);
	}
		
	if(pid1 > 0)
	{
		// Parent prep 2 code
		printf("[PARENT] Waiting for Child1 signal.\n");
		pause();
		
		char pipeMessage[] = "TEST";
		
		close(pipes[1][0]);
		write(pipes[1][1], pipeMessage, sizeof(pipeMessage));
		close(pipes[1][1]);
		fflush(NULL);
		
		char inMessage[100];
		do
		{
			read(pipes[0][0], inMessage, sizeof(inMessage));
			printf("[PARENT] Pipe message arrived: %s\n", inMessage);
			
			if(atoi(inMessage) >= 0)
			{
				char outMessage[] = "-2";
				printf("PARENTMESSAGE: %s; %d\n\n", outMessage, atoi(outMessage));
				close(pipes[1][0]);
				write(pipes[1][1], outMessage, sizeof(outMessage));
				close(pipes[1][1]);
				fflush(NULL);
				printf("[PARENT] CONTINUE message sent to the child1!\n");
			}
		}
		while(atoi(inMessage) > -2);
		
		char outMessage[] = "-2";
		close(pipes[1][0]);
		write(pipes[1][1], outMessage, sizeof(outMessage));
		close(pipes[1][1]);
		fflush(NULL);
		printf("[PARENT] TERMINATION message sent to the child1!\n");
	}
	
	close(pipes[0][1]);
	fflush(NULL);			
	close(pipes[1][1]);
	fflush(NULL);
	
	sleep(2);
	kill(pid1, SIGKILL);
	printf("[PARENT] Child1 is killed!\n");
	return 0;
}
