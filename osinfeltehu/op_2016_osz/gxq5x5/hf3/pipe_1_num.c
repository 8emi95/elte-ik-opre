#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int pipefd[2];
	
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a cső nyitásakor\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();
	
	if (pid == 0) // child
	{
		sleep(3);
		int message;
		close(pipefd[1]);
		read(pipefd[0], &message, sizeof(message));
		printf("Gyerek olvasta szám: %i", message);
		printf("\n");
		close(pipefd[0]);
	}
	else // parent
	{
		close(pipefd[0]);
		int message = 38;
		write(pipefd[1], &message, sizeof(message));
		close(pipefd[1]);
		wait();
	}
}