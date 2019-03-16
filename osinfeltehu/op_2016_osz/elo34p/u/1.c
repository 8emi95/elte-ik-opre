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
	 int azon;
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, int max ) 
{ 
	srand(time(NULL)); //the starting value of random number generation
	int r=(rand()%max)+1; //number between 1-100
	
     const struct uzenet uz = { 5, r }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.azon) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

/*int valasz( int uzenetsor, pid_t pid ) 
{ 
     const struct uzenet uz = { 5, r }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.azon) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} */

     
// receiving a message. 
int fogad( int uzenetsor, int max ) 
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
          printf( "A kapott uzenet kodja: %ld, ennyidadik vagy:  %i, osszes szerelveny: %i\n", uz.mtype, uz.azon, max ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
	 
	 int max=argc;
     
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
          kuld( uzenetsor, max );  // Parent sends a message. 
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor, max ); 
		  pid_t parent=getppid();
		  //kuld(uzenetsor,0,parent);
		  exit(0); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 