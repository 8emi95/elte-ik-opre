#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int hanyadik;
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 3, 5 };    
     int status; 
     
     //status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	 status = msgsnd( uzenetsor, &uz, sizeof ( uz.hanyadik ) + 1 , 0 );       
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) { 
	 struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, sizeof(uz.hanyadik), 3, 0 );      
     if ( status < 0 ) {
		 perror("msgsnd"); 
		fflush(stdout);
		return 1;
	 }          
     else{
		 printf( "Ez a jaratszamom:  %ld\n",uz.hanyadik );  
		  fflush(stdout);
		  return 0; 
	 }
          
     
} 

int main (int argc, char* argv[]) { 
     pid_t child;	
     int uzenetsor, status,hanyadik,osszes=atoi(argv[1]); 
     key_t kulcs;     
	 srand(time(NULL));	 
     kulcs = ftok(argv[0],1);      
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     }      
	 fflush(stdout);
     child = fork();	 
	 fflush(stdout);
     if ( child > 0 ) { 
		  hanyadik = rand()%(osszes+1);
		  printf("inditom a jaratot \n");
		  fflush(stdout);
          kuld( uzenetsor );  // Parent sends a message.		
		  wait(NULL);
          //pause(); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
	      printf("elindul a metro \n");
		  fflush(stdout);
          return fogad( uzenetsor ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
		  fflush(stdout);
          return 1; 
     } 
     
     return 0; 
} 
