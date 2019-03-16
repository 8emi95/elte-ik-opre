#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h>
#include <sys/wait.h> 

int main(int argc, char **argv)
{
	int pipefd[2];
	pid_t pid;
	char **lakcim;
	int i; 
	
	if (pipe(pipefd) == -1) 
    {
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if( pid == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	
	if( pid == 0)
	{	// GYEREK - utas
		for (i=1;i<argc;i++)
		{
			write(pipefd[1], argv[i], sizeof(argv[i]));
		}
		close(pipefd[1]);
		fflush(NULL);
	
	}
	else
	{	//SZÜLŐ	- központ
		int status;
		waitpid(pid, &status, 0);
		for (i=1;i<argc;i++)
		{
			read(pipefd[0], lakcim[i], sizeof(lakcim[i]));
			printf("A lakcim: %s\n",lakcim[i]);
		}
		close(pipefd[0]);
		for(i=1;i<argc;i++)
		{
			waitpid(pid, &status, 0);
		}
		
	}
	
	return 0;
}