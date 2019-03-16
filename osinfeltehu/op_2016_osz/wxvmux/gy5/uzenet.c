//SYSTEM5 �zenetsor zh-ra!

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
//ipcs lebego uzenetsor kikeres�se, ipcrm -q msqid-vel lehet torolni 
struct uzenet { 
//k�t�tt sorrend, ennek a longnak kell els� helyen lennie
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, char uzenet, int pid) 
{ 
	char uz[20];
	strcpy(&uz, uzenet);
     const struct uzenet uz = { pid, uzenet }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	 printf("Uzenet elkuldve \n");
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, int pid ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, pid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok gener�l 
     //. 
     
	 kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( /*kulcs vagy*/ IPC_PRIVATE /*de kivulrol nem lehet csatlakozni*/, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
		  char uz[20] = "Hajra fradi";		  
		  
          kuld( uzenetsor , uz, child);  // Parent sends a message.
		  fogad(uzenetsor, getpid());
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
           fogad( uzenetsor, getpid() ); 
		   char uz[20] = "Hajra vasas";	
		   kuld(uzenetsor, uz, getppid());
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
