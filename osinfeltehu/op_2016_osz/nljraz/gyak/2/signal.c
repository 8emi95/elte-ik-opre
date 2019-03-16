#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int sigcount = 0;

void handler(int signumber)
{
	printf("Signal with number %i has arrived\n", signumber);
	sigcount++;
}

int main()
{
	signal(SIGTERM, handler);
	
	pid_t child = fork();
    if (child > 0)
	{
		printf("Parent waiting for signal...\n");
		while (sigcount == 0) 
			usleep(1);
		printf("Parent received signal!\n");
		
		printf("Parent sending signal...\n");	
		kill(getppid(), SIGTERM);
		
		printf("Parent done.\n");	
	}
	else
	{
		printf("Child sending signal...\n");
		kill(getppid(), SIGTERM);
		
		printf("Child waiting for signal...\n");
		pause();
		printf("Child received signal!\n");
		printf("Child done.\n");	
	}
}
