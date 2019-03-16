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
int kuld( int uzenetsor, int n ) 
{ 
     const struct uzenet uz = { n, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
			// ilyenkor ha nem 0 akkor v�r addig amig valaki ki nem olvas �s nem lesz hely
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, int n ) 
{ 
     struct uzenet uz; 
     int status; 
	// az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot vesszuk ki a sorbol 
		// ez mondja meg kinek sz�l� �zeneteket szeretn�k
		// sz�l� gyerek viszonyn�l a PID sz�mokkal szoktak kommunik�llni
		// HA 0 van az 5 helyett akkor mindenki �zenet�t el tudom olvasni - a t�bbiek nem biztos h megkapj�k ha az �temez� a lop�st ind�tja el
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
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
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor , 5 );  // Parent sends a message. 
		  fogad(uzenetsor , 6 );
		  
          wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
		  // m�st is lehet ezzel csin�lni de itt most az �zenetsor t�rl�se.
		  // ha nem t�r�ljuk akkor a rendszerben  marad �s a folyamat ami l�trehozza nem t�rtli automatikusan 
		  //mert ha nem csak gyerek-sz�l� kommunik�l akkor m�s is tudja haszn�lni
		  
		  // IPCS - parancs ha nem t�rl�dik; ezzel lehet ellen�rizni hogy mi ragadt be
		  // IPCRM -q ..id.. => t�rli az �zeetsort
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
           fogad( uzenetsor, 5 ); 
		   
		   kuld(uzenetsor, 6);
		   return 0;
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
