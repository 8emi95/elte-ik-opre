#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <time.h>


struct uzenet { 
     //long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     //char mtext [ 2048 ]; 
	int n;
	char text[100];
}; 

// sendig a message
int kuld( int uzenetsor, struct uzenet* uz) 
{ 
     int status; 
     
     status = msgsnd( uzenetsor, uz, sizeof(struct uzenet)/*strlen ( uz->mtext )*/ , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet* uz = malloc(sizeof(struct uzenet)); 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, uz, sizeof(struct uzenet), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "%s\n", uz->text); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
		struct uzenet* uz = malloc(sizeof(struct uzenet));
		srand(time(NULL));
		uz->n = (rand() % 50) + 20;
		strcpy(uz->text, "Hajra Fradi!"); 
          kuld( uzenetsor, uz );  // Parent sends a message. 
		  //fogad( uzenetsor);
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
		  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor );
		  struct uzenet* uz = malloc(sizeof(struct uzenet));
		  strcpy(uz->text, "csak várj, míg a kezünk közé nem kerülsz");
		  //kuld(uzenetsor, uz);
		  return 0;
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
