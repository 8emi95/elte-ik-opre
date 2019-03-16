#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h>
#include <sys/time.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor , int maxSzerelveny) 
{ 
	int sorszam = (rand() % maxSzerelveny) + 1;
	
	char strSorszam[100];
	//sprintf(str1, "%d", sorszam);
	snprintf(strSorszam, 100, "%d", sorszam);
	
	char strMaxSzerelveny[100];
	//sprintf(str2, "%d", maxSzerelveny);
	snprintf(strMaxSzerelveny, 100, "%d", maxSzerelveny);
		
     const struct uzenet uz = { 3, *strSorszam };
     const struct uzenet uz2 = { 5, *strMaxSzerelveny }; 
     int status;
	 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     status = msgsnd( uzenetsor, &uz2, strlen (uz2.mtext) + 1, 0);
	 
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
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 3, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
     printf( "Ennyiedik szerelveny vagyok:  %s\n", uz.mtext );
 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0);
	 
	 
     printf("Ennyien vagyunk: %s\n", uz.mtext);

	 
     return 0; 
} 

int main (int argc, char* argv[]) {
	srand(time(NULL));	
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs;
	 
	 int maxSzerelveny;
	 sscanf (argv[1],"%d",&maxSzerelveny);
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
	 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor, maxSzerelveny );  // Parent sends a message.
          wait(NULL); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          fogad( uzenetsor ); 
		  return 0;
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
