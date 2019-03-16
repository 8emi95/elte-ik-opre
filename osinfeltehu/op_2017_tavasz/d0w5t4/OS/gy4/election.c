#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


void handler(int signumber)
{
	printf("Both children are ready",signumber);
}

void verify()
{
	
}

void stamp()
{
	
}


// president
void main()
{
	signal(SIGUSR1,handler);
	
	
	pid_t president = getppid();
	int verifier_ready = 0;
	int stamper_ready  = 0;
	
	// the verifier will only be set in the parent process
	// in the child it will 0
	pid_t verifier = fork();
	if (getppid() == president)
	{ 
		pid_t stamper = fork();
		if (getppid() == president)
		{
			while(!verifier_ready || !stamper_ready)
			{
				usleep(200);
			}
			
			printf("Both children are ready\n",SIGUSR1);
			
			// it waits for the two children to end
			wait(NULL); wait(NULL);
		}
		else
		{
			// stamper
		}
	}
	else 
	{
		// verifier
		verify();
	}
	
	return 0;
}

