#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{


	int status;

	pid_t fork1 = fork();
	if (fork1 < 0)
	{
		perror("The fork call was unsucessful\n");
		exit(1);
	}
	if (fork1 > 0) //parent process
	{
		printf("I am the parent\n");
		waitpid(fork1, &status,0);
	}
	else
	{
		printf("I am the child\n");

	}


	return 0;
}