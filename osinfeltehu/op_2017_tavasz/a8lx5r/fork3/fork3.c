#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char const *argv[])
{


	pid_t pid = fork();

	if (pid == 0)
	{
		printf("Child with ID:%i.\n",(int) getpid());
		exit(0);
	}

	printf("Parent with ID:%i.\n",(int) getpid());
	wait(NULL);
	fork();
	
	printf("Hello.\n");




	return 0;
}