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
int kuld( int uzenetsor, pid_t cpid, int szam, int max ) 
{ 
     const struct szerelveny uz = { cpid, "", szam, max }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int kuld2( int uzenetsor, pid_t cpid, int szam, int max ) 
{ 
	 int randNum = rand()%(100-1 + 1) + 1;
     int status; 
     
	 if(randNum < 90) {
		 const struct szerelveny uz1 = { cpid, "Ez lefagyott!", szam, max }; 
		 status = msgsnd( uzenetsor, &uz1, strlen ( uz1.mtext ) + 1 , 0 );
	 } else {
		 const struct szerelveny uz2 = { cpid, "Sikeresen celba ert!", szam, max }; 
		 status = msgsnd( uzenetsor, &uz2, strlen ( uz2.mtext ) + 1 , 0 );
	 }
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
    // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, pid_t cpid ) 
{ 
     struct szerelveny uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, cpid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "[Gyerek]: Ez: %i, Max: %i, Hatravan: %i\n", uz.szam, uz.max, uz.max - uz.szam ); 
     return 0;
} 

int fogad2( int uzenetsor, pid_t cpid ) 
{ 
     struct szerelveny uz; 

     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, cpid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "[Szulo] Visszajelzes: %s\n", uz.mtext ); 
			kill(cpid, SIGKILL);
     return 0; 
} 

int main (int argc, char* argv[]) { 
	
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs;
	 int x = atoi(argv[1]);
	 int randNum = rand()%(x-1 + 1) + 1;
     
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
		  pid_t childpid = getpid();
          kuld( uzenetsor, childpid, randNum, x );
          wait( NULL );
		  
		  fogad2(  uzenetsor, childpid );
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
		  pid_t parentpid = getppid();
		  kuld2( uzenetsor, parentpid, randNum, x );
          return fogad( uzenetsor, parentpid ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
