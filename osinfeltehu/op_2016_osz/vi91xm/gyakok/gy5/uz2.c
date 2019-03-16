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
int kuld( int uzenetsor, pid_t pid ) 
{ 
     const struct uzenet uz = { pid, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
    // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

// sendig a message
int valasz( int uzenetsor, pid_t pid ) 
{ 
     const struct uzenet uz = { pid, "Csak a Vidi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0 ); 
     
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
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1);
	 /* nem muszáj mindig ftok fv-t használni, hanem szülő-gyereknél lehet:
	 IPC_PRIVATE az msgget-nél, ami a gyerekfolyamat számára egy elérhető üzenetsor lesz*/
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); // itt az IPC_PRIVATE használható
     if ( uzenetsor < 0 ) { 
          perror("msgget\n"); 
          return 1; 
     }
     
     child = fork(); 
     if ( child > 0 ) { 
          kuld( uzenetsor, getpid() );  // Parent sends a message to child.
          wait( NULL ); // itt nem kell szinkronizálni, mert a címzés alapján végezzük el, hogy csak a sajátját olvassa vissza
		  // pause is vár, de nem egyenértékűek: ez egy jelre vár
			fogad(uzenetsor);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl\n"); 
          return 0; 
     } else if ( child == 0 ) {
		 pid_t parent = getppid();
         fogad( uzenetsor );
		 return valasz(uzenetsor, parent);
          // The child process receives a message. 
     } else { 
          perror("fork\n"); 
          return 1; 
     } 
     
     return 0; 
} 
