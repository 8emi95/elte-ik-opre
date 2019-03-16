#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
         perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status,uz2; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     key_t kulcs2;
     kulcs2=ftok(argv[0],1);
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
     uz2 = msgget( kulcs2, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork();
     pid_t child2;
     if ( child > 0 ) {
         
          child2=fork();
          if(child2==0)
          {
              //return fogad( uz2 ); 
          }
          else
          {
            struct uzenet uz;
            uz.mtype=5; 
            //uz.mtext="Hajra Fradi!"; 
            strcpy(uz.mtext,"Hajra Fradi!");
            int status; 
     
            status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
                if ( status < 0 ) 
          perror("msgsnd"); 
            //kuld(uz2,9);
            // Parent sends a message. 
            wait( NULL ); 
            // After terminating child process, the message queue is deleted. 
                status = msgctl( uzenetsor, IPC_RMID, NULL ); 
            if ( status < 0 ) 
               perror("msgctl");
          }

          return 0; 
     } else if ( child == 0 ) { 
          return fogad(uzenetsor); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
