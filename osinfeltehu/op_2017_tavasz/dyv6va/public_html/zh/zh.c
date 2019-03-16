#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>

#define MAX_CHILDREN 2
#define CHILDCOUNT 2 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 1
#define ERR_ARG "Please pass the address as an argument"
#define ERR_PIPE "Error opening pipe"
#define ERR_WRITING "Error writing to pipe"
#define ERR_READING "Error reading from pipe"
#define proc_owner "Taxi organization"
#define proc_owner2 "Client"
#define proc_owner3 "Cab driver"

void handler(int signumber) {}

int main (int argc, char **argv) 
{ 
	int pipefd[2];
	pid_t pids[MAX_CHILDREN]; 
	if (argc < 2)
	{
		perror(ERR_ARG);
		exit(1);
	}
	
	if (pipe(pipefd) == -1)
    {
        perror(ERR_PIPE);
        exit(1);
    }
	
	int i;
	int pid;
	
	signal(SIGUSR1, handler);

	for (i = 0; i < CHILDCOUNT; ++i) 
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			abort();
		} 
		else if (pid == 0)
		{
			//child i
			if (i != SPECIAL) //we can start more than one type of child proc
			{
				printf("[CHILD] Starting..(%d)\n", getpid());
				char address[500] = "";
				int j;
				for (j = 1; j < argc; j++)
				{
					strcat(address, argv[j]);
					strcat(address, " ");
				}
				
				if (write(pipefd[1], &address, sizeof(address)) < 0)
				{
					perror(ERR_WRITING);
					exit(1);
				}
				close(pipefd[1]);
				close(pipefd[0]);
				printf("[CHILD] Finished.\n");	
			}			
			else 
			{	
				printf("[CHILD] Taxi starting..(%d)\n", getpid());
				
				
				close(pipefd[1]);
				close(pipefd[0]);
				printf("[CHILD] Taxi process finised.\n");
			}
			break;
		} 	
		else
		{
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) 
	{
		//parent
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		char recvaddr[500];
		if (read(pipefd[0], &recvaddr, sizeof(recvaddr)) < 0)
		{
			perror(ERR_READING);
			exit(1);
		}
		
		printf("%s: The address of the client is: %s\n", proc_owner, recvaddr);
		
		
		// Wait for children
		int n = 0;
		int status;
		pid_t pid;
		
		while (n < CHILDCOUNT) 
		{
			pid = wait(&status);
			printf("[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}	
		
		printf("[PARENT] Finished waiting for child processes.\n");
		
		close(pipefd[1]);
		close(pipefd[0]);
	}

}