#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <math.h>

struct uzenet { 
     long mtype;
     float x;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     float y; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet p1 = { 1, 1, 2 }; 
     const struct uzenet p2 = { 1, 4, 6 }; 
     int status; 
   
     int msgSize = sizeof(struct uzenet) - sizeof(float);

     printf("send1\n");
     status = msgsnd( uzenetsor, &p1, msgSize, 0 ); 
     printf("send2\n");
     status = msgsnd( uzenetsor, &p2, msgSize, 0 ); 
     printf("send3\n");
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     struct uzenet uz2; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszu ki a sorbol 
     printf("receive1\n");
     status = msgrcv(uzenetsor, &uz, 1024, 1, 0 ); 
     printf("receive2\n");
     status = msgrcv(uzenetsor, &uz2, 1024, 1, 0 ); 
     printf("receive3\n");
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott pontok tavolsaga: %f\n",
            sqrt(pow(uz2.x - uz.x, 2) + pow(uz2.y - uz.y, 2)) ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     //printf("before ftok\n");
     kulcs = ftok(argv[0],1); 
     //printf("after ftok\n");
     printf ("A kulcs: %d\n",kulcs);
     //printf("before msgget\n");
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     //printf("after msgget\n");
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
    
     printf("before fork\n");
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          return fogad( uzenetsor ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
