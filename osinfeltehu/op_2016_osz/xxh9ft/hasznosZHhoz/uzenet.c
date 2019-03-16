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
int kuld( int uzenetsor, pid_t pid, char msg[1024] ) {
     /*const*/ struct uzenet uz;
     uz.mtype = pid;
     strcpy(uz.mtext, msg);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) {
          perror("msgsnd"); 
     }
     return 0; 
}
     
// receiving a message. 
int fogad( int uzenetsor, pid_t pid) {
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, pid, 0 ); 
     
     if ( status < 0 ) {
          perror("msgsnd");
     } else {
          printf( "%d vagyok, a kapott uzenet kodja: %ld, szovege:  %s\n", getpid(), uz.mtype, uz.mtext );
          //printf( "%s vagyok, a kapott uzenet kodja: %ld, szovege:  %s\n", getpid(), uz.mtype, uz.mtext );
     }

     return 0; 
} 

int main (int argc, char* argv[]) {
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok general 
     //. 
    //kulcs = ftok(argv[0],1); // Ez nem kell, ha csak a gyereknek küldünk
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); // kulcs helyett lehet IPC_PRIVATE, amikor csak a gyereknek kell küldeni
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork();
     // parent
     if ( child > 0 ) {
          kuld( uzenetsor, child, "Szulo ezt kuldte." );  // Parent sends a message. 
          fogad( uzenetsor, getpid() );
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL);
          wait( NULL ); // megvárja a gyerekfolyamat befejeződését, ide nem feltétlen szükséges
          if ( status < 0 ) {
               perror("msgctl");
          }
          return 0;
     } else if ( 0 == child ) { // child
          fogad( uzenetsor, getpid() ); // The child process receives a message. 
          return kuld( uzenetsor, getppid(), "Gyerek ezt kuldte." );  // child sends a message. 
     } else { 
          perror("fork"); 
          return 1; 
     }
     
     return 0; 
} 
