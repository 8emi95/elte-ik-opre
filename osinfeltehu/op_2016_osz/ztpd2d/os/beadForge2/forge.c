#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void skipHandler(int signumber){ /* SKIP */ }

void doStuff() {
	/* SIGNAL HANDLING INIT */
	struct sigaction sigact;
	sigact.sa_handler=skipHandler;
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0;
	sigaction(SIGUSR1,&sigact,NULL);

	/* UNNAMED PIPE SETUP */
	int pipefd[2]; 
	pid_t pid;
	char buffer[32];
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
		   
	/* MULTI PROCESS START */  
	pid_t child=fork();
	if (child>0)
	{
		/* PARENT PROCESS */
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGUSR1);
		printf("Parent waits for SIGUSR1\n");
		sigsuspend(&sigset);

		printf("Parent's signal has arrived!\n");
		
		printf("Parent writes the pipe!\n");
		int msgCount = 6;
		write(pipefd[1], &msgCount, sizeof(msgCount));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "Raise my children!",sizeof("Raise my children!"));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "Father should be proud!",sizeof("Father should be proud!"));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "One",sizeof("One"));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "Two",sizeof("Two"));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "Three",sizeof("Three"));
		printf("Parent writes the pipe!\n");
		write(pipefd[1], "SAY SOMETHING TO MEEEEE!!!....",sizeof("SAY SOMETHING TO MEEEEE!!!...."));
		
		sigsuspend(&sigset);
		printf("Parent reads from pipe..\n");
		read(pipefd[0],buffer,sizeof(buffer));
		printf("Parent gets message: %s\n", buffer);
		
		wait();
		printf("Parent finished\n");
	}
	else 
	{
		/* CHILD PROCESS */
		printf("Child waits a bit..\n");
		sleep(2);
		
		printf("Child send valid signal..\n");
		kill(getppid(),SIGUSR1);
		
		printf("Child reads from pipe..\n");
		int msgCount = 0;
		read(pipefd[0],&msgCount,sizeof(int));
		printf("Child will recive %d messages.\n", msgCount);
		int i;
		for(i=0; i < msgCount; ++i) {
			read(pipefd[0],buffer,sizeof(buffer));
			printf("Child gets message: %s\n", buffer);
		}
		
		printf("Child writes the pipe!\n");
		write(pipefd[1], "Thnx father!",sizeof("Thnx father!"));
		kill(getppid(),SIGUSR1);
		
		sleep(2);
		printf("Child finished\n");
		exit(0);
	}
}

int main() {
	doStuff();
	
	printf("Main program returns..\n");
	return 0;
}