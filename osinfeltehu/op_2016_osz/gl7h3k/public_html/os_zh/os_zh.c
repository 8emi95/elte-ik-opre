#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct szerelveny { 
     long mtype;
     char mtext [ 1024 ];
	 int szam;
	 int max;
}; 

// sendig a message
int send( int uzenetsor, pid_t cpid, int szam, int max ) 
{ 
     const struct szerelveny uz = { cpid, "", szam, max }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int send2( int uzenetsor, pid_t cpid, int szam, int max ) 
{ 

	srand(time(NULL));
	//int r = rand();
	 int randNum = rand()%(200-1 + 1) + 1;
     int status; 
	
     
	 if(randNum < 100) {
		 const struct szerelveny uz1 = { cpid, "Frozen!", szam, max }; 
		 status = msgsnd( uzenetsor, &uz1, strlen ( uz1.mtext ) + 1 , 0 );
	 } else {
		 const struct szerelveny uz2 = { cpid, "Reach the destination!", szam, max }; 
		 status = msgsnd( uzenetsor, &uz2, strlen ( uz2.mtext ) + 1 , 0 );
	 }

     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int receive( int uzenetsor, pid_t cpid ) 
{ 
     struct szerelveny uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, cpid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Child: This is %i, Maximum: %i, Left: %i\n", uz.szam, uz.max, uz.max - uz.szam ); 
     return 0;
} 

int receive2( int uzenetsor, pid_t cpid ) 
{ 
     struct szerelveny uz; 

     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, cpid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Parent message: %s\n", uz.mtext ); 
			kill(cpid, SIGKILL);
     return 0; 
} 

int main (int argc, char* argv[]) { 
	
     pid_t child; 
     int uzenetsor, status; 
	 int x = atoi(argv[1]);
	 int randNum = rand()%(x-1 + 1) + 1;
     
	 int kulcs = ftok("data", 1);
	 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
		  pid_t childpid = getpid();
          send( uzenetsor, childpid, randNum, x );
          wait( NULL );
		  
		  receive2(  uzenetsor, childpid );
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
		  pid_t parentpid = getppid();
		  send2( uzenetsor, parentpid, randNum, x );
          return receive( uzenetsor, parentpid ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
