#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

#define READING (size_t)0
#define WRITING (size_t)1

int main(int argc, char *argv[])
{
	int passanger_pipe[2]; // unnamed pipe file descriptor array
	pid_t child;
	int status;
	char address[100];
	
	if (argc < 2)
	{
		perror("Parameterkent add meg a cimet!\n");
		return 1;
	}
	
	strcpy(address, argv[1]);
	
	if (pipe(passanger_pipe) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   return 1;
	}

	child = fork();	// creating parent-child processes
	if (child == -1) 
	{
	   perror("Fork hiba");
	   return 1;
	}

	if (child > 0) // szülő
	{
		printf("szulo: varom az utast\n");
		close(passanger_pipe[WRITING]);
		read(passanger_pipe[READING], address, sizeof(address));
		printf("szulo: az utas cime: %s", address);
		waitpid(child, &status, 0);
	}
	else if (child == 0) // gyerek
	{
		printf("utas: bemondom a cimet\n");
		close(passanger_pipe[READING]);
		write(passanger_pipe[WRITING], address, sizeof(address));
		close(passanger_pipe[WRITING]);
	}
	
	return 0;
}

 
