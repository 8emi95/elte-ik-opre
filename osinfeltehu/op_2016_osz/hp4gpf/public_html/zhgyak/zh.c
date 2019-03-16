#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h> //pipe
#include <signal.h>

#include <stdbool.h> //

void handler(int signumber){
	printf("Jott signal!\n");
}


int main(int argc,char ** argv) 
{	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	
	//csövek létrehozása:
	int pipeptoch1[2];
	int pipepch1toch2[2];
	
	pid_t pid1;
	pid_t pid2;
	
	pid1 = fork();	
	
	if (pid1 == -1) 
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0) 
	{	// child process1
		kill(getppid(), SIGUSR1);//child 1 jelez
		exit(0);
	
	}
	else 
	{	// parent process
		pid2 = fork();
		
		if (pid2 == -1) 
		{
			perror("Fork hiba");
			
		}
		if (pid2 == 0) 
		{	// child process2
			kill(getppid(), SIGUSR2);//child 2 jelez
			exit(0);
		}
		else 
		{// real parent process
			wait();//várakozás, h jelezzenek
			wait();
			
			srand(time(NULL));
			(rand() % 10);
			
			printf("Mind2 uzebet megjott!\n");
			
			//wait(NULL)；//wait for all children
			
		}
	
	}
	return 0;
}