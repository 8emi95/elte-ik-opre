#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "unistd.h"
#include "signal.h"
#include "sys/types.h"
#include "sys/wait.h"

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

void handler(int signumber){}

int main(int argc, char *argv[])
{
	signal(SIGUSR1,handler);
	int pipefd[2];
	pid_t child;
	char address[100];
	
	if (pipe(pipefd) == -1) 
	{
	   perror("The pipe open has failed!");
	   exit(EXIT_FAILURE);
	}
	child = fork();
	if(child < 0)
	{
		perror("The fork open has failed");
		exit(EXIT_FAILURE);
	}
	if(child > 0){
		pause();
		pid_t car;
		car = fork();
		read(pipefd[0],address, sizeof(address));
		printf("The address is:%s\n",address);
		if(car < 0)
		{
			perror("The fork open has failed");
			exit(EXIT_FAILURE);
		}
		if(car > 0)
		{
			
		}
		int status;
		wait(&status);
	}else{
		write(pipefd[1],argv[1], 100 * sizeof(char));
		kill(getppid(),SIGUSR1);
	}
}