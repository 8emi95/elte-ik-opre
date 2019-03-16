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
     long mtype2;
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, struct uzenet uz ) 
{ 
     int status; 
     
     status = msgsnd( uzenetsor, (char*)&uz, sizeof (uz) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, struct uzenet* uz ) 
{ 
     int status; 

     status = msgrcv(uzenetsor, uz, sizeof(*uz), 0, 0 ); 
     return status;     
} 

int main (int argc, char* argv[]) {
     const int max_szerelveny = atoi (argv[1]);
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     srand(time(NULL)); //the starting value of random number generation
     
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
		  srand(time(NULL));
		  long sz_szam = rand()%max_szerelveny+1;
	  struct uzenet uz = { sz_szam, max_szerelveny, "Indul a vonat!" };  
          kuld( uzenetsor, uz );  // Parent sends a message. 
          //wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          
          fogad( uzenetsor, &uz );

if ( fogad( uzenetsor, &uz ) < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
          
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          struct uzenet uz;
	  fogad( uzenetsor, &uz  );
          int r=rand()%101;
          
          if (r>=90) {
			  strcpy (uz.mtext, "Bevontattak!");  
		  } else {
			  strcpy (uz.mtext, "Beertem az allomasra!");
		  }
		  
		  kuld( uzenetsor, uz );
          
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
