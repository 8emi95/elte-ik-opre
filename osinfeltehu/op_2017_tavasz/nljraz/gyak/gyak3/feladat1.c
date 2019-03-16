#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#define bool int
#define true 1
#define false 0
#define IsChild(id) ((id) > 0 ? true : false)

void handler(int signumber)
{ }

int main(int argc, char **argv) 
{
	signal(SIGTERM, handler);
	
	//bool parentiswaiting = false,
	//     childiswaiting = false;
	
	pid_t child = fork();
	if (0 == IsChild(child))
	{
		printf("Child: waiting for parent\n");
		//while (parentiswaiting == 0)
		//	sleep(1);
		kill(getppid(), SIGTERM);
		printf("Child: signal sent\n");
		//childiswaiting = true;
		pause();
		printf("Child: signal arrived\n");
		printf("Child: process ended\n");
	}
	else
	{
		//parentiswaiting = true;
		printf("Parent: waiting for signal\n");
		pause();
		printf("Parent: signal arrived\n");
		//while (childiswaiting == 0)
		//	sleep(1);
		kill(child, SIGTERM);
		printf("Parent: signal sent\n");
		int status;
		wait(&status);
		printf("Parent: process ended\n");
	}
}
