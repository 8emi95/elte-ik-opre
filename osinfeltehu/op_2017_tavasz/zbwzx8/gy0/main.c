#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

void handler(int signum)
{
	printf("Az egyik tag(ID: %i) bejelentkezett az elnokhoz.\n", signum);
}

void rmFifo(int signum)
{
	remove("child1.fifo");
	printf("\n");
	exit(0);
}

int main(int argc, char** argv)
{
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	signal(SIGINT, rmFifo);

	int pipefd[2];
	if(pipe(pipefd) == -1) 
	{
		perror("Error while creating pipe.\n");
		remove("child1.fifo");
		exit(0);
	}

	pid_t child1;
	pid_t child2;

	child1 = fork();
	if(child1 < 0)
	{
		perror("Error while creating 1st child process.\n");
		exit(1);
	}

	if(child1 == 0) // child1 process -- adatellenorzo
	{
		//printf("-- Az 1. tag megkezdi a munkat.\n");
		sleep(1);
		kill(getppid(), SIGUSR1);

		sleep(2);
		close(pipefd[1]);
		int numOfVotes;
		read(pipefd[0], &numOfVotes, sizeof(int));
		int* randomIDs = (int*)malloc(sizeof(int)*numOfVotes);

		printf("Adatok fogadasa:\nValasztok szama: %i\nValasztok azonositoja sorban:\n", numOfVotes);
		for(int i = 0; i < numOfVotes; ++i)
		{
			int tmp;
			read(pipefd[0], &tmp, sizeof(int));
			printf("%i, ", tmp);
			randomIDs[i] = tmp;
		}
		printf("\n");
		close(pipefd[0]);

		//printf("Az 1. tag befejezi a munkat.\n");
		free(randomIDs);
	}else
	{
		child2 = fork();
		if(child2 < 0)
		{
			perror("Error while creating 1st child process.\n");
			exit(1);
		}

		if(child2 == 0) // child2 process -- pecsetelo
		{
			//printf("-- A 2. tag megkezdi a munkat.\n");
			sleep(2);
			kill(getppid(), SIGUSR2);

			//printf("A 2. tag befejezi a munkat.\n");
		}else // parent process
		{
			//printf("--- Az elnok megkezdi a munkat.\n");
			pause();
			pause();
			printf("------ A tagok sikeresen bejelentkeztek az elnokhoz\n");

			int numOfVotes = atoi(argv[1]);
			int* randomIDs = (int*)malloc(sizeof(int)*numOfVotes);
			close(pipefd[0]);
			write(pipefd[1], &numOfVotes, sizeof(int));

			for(int i = 0; i < numOfVotes; ++i)
			{
				srand(i * (((i + 13) * 32 + numOfVotes * (i % 6)) % 3 + 61) % 23);
				int r = rand() % 100;
				write(pipefd[1], &r, sizeof(int));
				randomIDs[i] = r;
			}
			close(pipefd[1]);

			while(wait(NULL) > 0);
			remove("child1.fifo");
			free(randomIDs);
			//printf("Az elnok befejezi a munkat.\n");
		}
	}

	return 0;
}
