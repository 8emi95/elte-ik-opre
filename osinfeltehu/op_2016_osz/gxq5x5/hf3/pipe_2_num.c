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
		int message[2];
		close(pipefd[1]);
		read(pipefd[0], &message[0], sizeof(message[0]));
		read(pipefd[0], &message[1], sizeof(message[1]));
		printf("Gyerek olvasta számok: %i, %i", message[0], message[1]);
		printf("\n");
		close(pipefd[0]);
	}
	else // parent
	{
		close(pipefd[0]);
		int message[2] = {4, 7};
		write(pipefd[1], &message[0], sizeof(message[0]));
		write(pipefd[1], &message[1], sizeof(message[1]));
		close(pipefd[1]);
		wait();
	}
}