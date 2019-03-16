#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CHILDREN 10
#define CHILDCOUNT 2 //you can use a number from args[] instead just dont forget to pass it to functions
#define SPECIAL 1   //utas


int signalFlag = 0;

void handler(int signumber)
{
	if(signumber==2){
		printf("Interrupted by signal!\n");
		exit(-1);
	}else{
   signalFlag = signumber;
	}
}


/*int childProc()
{
	
}

int parentProc()
{	
	
}

int specialChild()
{
	
}*/

int main (int argc, char **argv) 
{ 
	
	//you can use the pids array to send signals to every child from the parent
	int pipefd[2];
	pid_t pids[MAX_CHILDREN]; 
	
	if (pipe(pipefd) == -1) 
	{
	perror("Hiba a pipe nyitaskor!");
	exit(EXIT_FAILURE);
	}
	
	
	srand(time(NULL));
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);
	signal(SIGINT, handler);
	
	//int childCount = 1;
	int i;
	pid_t pid;

	for (i = 0; i < CHILDCOUNT; ++i) 
	{
		pid = fork();
		if (pid < 0) 
		{
			perror("fork");
			abort();
		} 
		else if (pid == 0) 
		{
			//child i
			printf("[CHILD] Starting..(%d)\n", getpid());
			
			
			if (i!=SPECIAL) //we can start more than one type of child proc
			{
				//do anything	
				sleep(1);
				
				printf("[CHILD] Finished.\n");	
			}			
			else 
			{	
				//do something different	
				sleep(3);
				
				char lakcim[100];
				
				if(argv[1]==NULL){
					  printf("Nincs megadott argumentum!\n");
					  kill(getppid(),SIGINT);
					  //wait(&status);
					  exit(-1);
					}
				strcpy(lakcim,argv[1]);
				
				printf("%s\n",lakcim);
				
				kill(getppid(),SIGUSR1);
				write(pipefd[1], lakcim, 100);
				
				
				
				printf("[CHILD] I was special. Now I am nothing.\n");
			}	
			break;	
				
							

			
				
			close(pipefd[0]);
			close(pipefd[1]);
		} 	
		else
		{
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) 
	{
		//parent
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		int n=0;
		int status;
		pid_t pid;
		
		char beLakcim[100];
		
		pause();
		read(pipefd[0],beLakcim,100);
		
		printf("Szulo beolvasta az adatot: %s\n",beLakcim);
		
		close(pipefd[0]);
		close(pipefd[1]);
		
		while (n < CHILDCOUNT) 
		{
			pid = wait(&status);
			printf("[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}	
	
		printf("[PARENT] Finished waiting for child processes.\n");
		
		
		//parentProc();
		
	}

}
