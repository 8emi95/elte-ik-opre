#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

struct szavazo
{
	int db;
	int szavazok[100];
};

void handler(int signumber){
	if(signumber == SIGUSR1)
	{
		;
	}
}

void ParentProc(int child1Pid, int child2Pid, struct szavazo szav, int* pipePar/*, int messageQueue, int* child1Pipe, int* child2Pipe*/)
{
	pause();
	pause();
	printf("Keszen allnak a gyerekek.");

	close(pipePar[0]);
	write(pipePar[1], &szav, sizeof(szav));
	
	close(pipePar[1]);
	wait(0);
}

void Child1Proc(int* pipePar/*int messageQueue, int* child1Pipe*/)
{
	kill(getppid(), SIGUSR1);
	close(pipePar[1]);
	
	struct szavazo szav;
	
	read(pipePar[0], &szav, sizeof(szav));
	int n = 0;
	while(n < szav.db)
	{
		printf("%d\n", szav.szavazok[n]);
	}
	
	close(pipePar[0]);
}

void Child2Proc(int* pipePar/*int messageQueue, int* child2Pipe*/)
{
	kill(getppid(), SIGUSR1);
}

int main(int argc, char* argv[])
{
	srand(time(0));
	
	struct szavazo szav;
	int i;
	szav.db = atol(argv[1]);
	for(i = 0; i < szav.db; i++)
	{
		szav.szavazok[i] = rand() % 100;
	}
	
	int toparent[2];
	if (pipe(toparent) < 0)
	{
		perror("Failed to allocate pipes\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t child1 = fork();
	if (child1 < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child1 > 0)
	{
		signal(SIGUSR1, handler);
		//second pipe
		//int child2Pipe[2];
		/*if (pipe(child2Pipe) == -1) 
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}*/
		pid_t child2 = fork();
		if (child2 < 0)
		{
			perror("fork"); 
		}
		else if(child2 > 0)
		{
			//parent
			ParentProc(child1, child2, szav, toparent/*, messageQueue, child1Pipe, child2Pipe*/);
		}
		else
		{
			//child2
			Child2Proc(toparent/*messageQueue/*, child2Pipe*/);
		}
		
	}
	else
	{
		//child1
		Child1Proc(toparent/*messageQueue/*, child1Pipe*/);
	}
}