#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //read
#include <time.h> //time
#include <string.h> //strncpy
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int fd[2], nbytes;
pid_t child1pid;
pid_t child2pid;
pid_t parentpid;
char readbuffer[80];
int status;
key_t key;
int messageQ;

int main(int argc, char* argv)
{
	pid_t parentpid = getpid();
	key = ftok(argv[0],1);
	
	if(child1pid = fork() == -1)
	{
		perror("fork");
		exit(1);
	}
	if(child1pid == 0)
	{
		child2pid = fork();
		if(child2pid = fork() == -1)
		{
			perror("fork2");
			exit(2);
		}
		if(child2pid = 0)
		{
			
		}
	}
	else //szulo
	{
		
	}
	
	return 0;
}