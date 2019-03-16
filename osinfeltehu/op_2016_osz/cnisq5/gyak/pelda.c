#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> //ctime
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <wait.h> 

void handler(int signumber){
  if (signumber == SIGUSR1)
  {
	  
  };
}

struct message { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

// sendig a message
int send( int msg, char * text) 
{ 
    struct message m = { 119, "asd" }; 
    int status; 

    strcpy(m.mtext, text);

    status = msgsnd( msg, &m, strlen( m.mtext )+ 1 , IPC_NOWAIT ); 
	 
    if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int receive( int msg ) 
{ 
     struct message m; 
     int status; 
     status = msgrcv(msg, &m, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
		printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", m.mtype, m.mtext ); 
     return 0; 
} 

int main(int argc,char *argv[])
{	
	signal(SIGUSR1,handler);
	
	srand(time(0));
	
	int msg, status; 
    key_t key; 
     
	key = ftok(argv[0],1); 
    printf ("A kulcs: %d\n",key);
    msg = msgget( key, 0600 | IPC_CREAT ); 
    if ( msg < 0 ) { 
        perror("msgget"); 
        return 1; 
    } 
	
	int pipechild1[2];
	int pipechild2[2];
	
	if (pipe(pipechild1) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	if (pipe(pipechild2) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
	pid_t pid;
	pid_t pid2;
	
	
	pid = fork();	// creating parent-child processes
	if (pid == -1) 
	{
		perror("Fork error");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) // child process
	{		    	
		printf("I am the first child\n");	
		return receive(msg);
	} 
	else	//parent process	
	{
		pid2 = fork();
		if (pid2 == -1) 
		{
			perror("Fork error");
			exit(EXIT_FAILURE);
		}
		if (pid2 == 0) // second child process
		{		    	
			printf("I am the second child\n");
			 
          // The child process receives a message. 
		}
		else //parent process
		{
			printf("I am the proud parent\n");
			
			send( msg, "First" );  // Parent sends a message. 
			wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( msg, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
		     return 0;
		}
		
        
		wait(NULL);
	}
	
}

