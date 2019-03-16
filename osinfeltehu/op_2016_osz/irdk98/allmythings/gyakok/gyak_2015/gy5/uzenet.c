#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct mesg { 
     long mtype;//address - number - to this number will listen to a process
     char mtext [ 1024 ]; 
}; 

// sendig a message
int sendm( int msgqueue ) 
{ 
     const struct mesg mq = { 5, "Hello world" }; 
     int status; 
     
     status = msgsnd( msgqueue, &mq, strlen ( mq.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int recm( int msgqueue ) 
{ 
     struct mesg mq; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(msgqueue, &mq, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "The code: %ld, text:  %s\n", mq.mtype, mq.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int msgqueue, status; 
     key_t key; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     key = ftok(argv[0],1); 
     printf ("The key: %d\n",key);
     msgqueue = msgget( key, 0600 | IPC_CREAT ); 
//   uzenetsor=msgget(IPC_PRIVATE,0600|IPC_CREAT);
     if ( msgqueue < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          sendm( msgqueue );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
         status = msgctl( msgqueue, IPC_RMID, NULL ); 
         if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          return recm( msgqueue ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
