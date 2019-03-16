#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "unistd.h"
#include "signal.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/ipc.h"
#include "sys/msg.h"
struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor, char *address) 
{ 
    struct uzenet uz;
	uz.mtype = 5;
	strcpy(uz.mtext,address);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 10 , 0 ); 
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
          printf( "A kapott utas lakcime: %s\n", uz.mtext); 
     return 0; 
} 

void handler(int signumber){}

int main(int argc, char *argv[])
{
	signal(SIGUSR1,handler);
	int pipefd[2];
	pid_t child;
	char address[100];
	int uzenetsor, status; 
    key_t kulcs; 
	kulcs = ftok(argv[0],1);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
     } 
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
		close(pipefd[0]);
		close(pipefd[1]);
		kuld( uzenetsor, address);
		if(car < 0)
		{
			perror("The fork open has failed");
			exit(EXIT_FAILURE);
		}
		if(car > 0)
		{
			sleep(3);
			fogad( uzenetsor); 
		}
		int status;
		wait(&status);
	}else{
		close(pipefd[0]);
		write(pipefd[1],argv[1], 100 * sizeof(char));
		close(pipefd[1]);
		kill(getppid(),SIGUSR1);	
	}
}