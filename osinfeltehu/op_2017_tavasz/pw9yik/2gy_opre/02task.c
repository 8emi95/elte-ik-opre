#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber)
{
    printf("Signal with number %i has arrived\n", signumber);
}

int main(void)
{
	signal(SIGTERM, handler);
	
	pid_t child = fork();
	
	if(child > 0) //parent
	{
		while(1)
		{
			pause();
			int signal;
			wait(&signal);
		}
	}
	else //child
	{
		int i;
		int random;
		for (i = 0; i<100; i++)
		{
			random = rand() % 100 + 1;
			if(random > 50 && random < 75)
			{
				kill(getppid(), SIGTERM);
				printf("(child process): Random number: %i\n", random);
			}
		}
	}
}

//ylik6z