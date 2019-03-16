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
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     sleep(2);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s, PID: %i\n", uz.mtype, uz.mtext, pid ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child_a; 
     pid_t child_b; 
     pid_t parent; 
     int uzenetsor, status; 
     key_t kulcs; 
	 int db = 0;
     printf ("INIT");
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
	 parent = getpid();
     child_a = fork(); 
	 if (child_a > 0)
	 {
	 child_b = fork();
	 }
     printf ("SZULO: %i, A CHILD: %i, B child: %i\n",parent,child_a,child_b);

     if ( child_a > 0 && child_b > 0 ) { 
		  printf ("A szulo kuldi az uzenetet:\n");
          kuld( uzenetsor );  // Parent sends a message. 
          kuld( uzenetsor );  // Parent sends a message. 
		  printf ("A szulo varakozik\n");
          wait( NULL ); 
//		waitpid(child_a,&status,0); 
//		waitpid(child_b,&status,0); 
	// After terminating child process, the message queue is deleted. 
		  printf ("A szulo kap valamit\n");
		  printf ("DB: %i\n",db);
		  db += 1;
		  if (db > 2)
		  {
			  status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			  if ( status < 0 ) 
				   perror("msgctl"); 
			  return 0; 
		  }
     } else {
		  printf ("A PID: %i\n",getpid());
		 if ( child_a == 0 ) { 
		  printf ("A gyerek fogad valamit A\n");
		  printf ("DB: %i\n",db);
          return fogad( uzenetsor, getpid() ); 
          // The child process receives a message. 
		} 
		if ( child_b == 0 ) { 
		  printf ("A gyerek fogad valamit B\n");
		  printf ("DB: %i\n",db);
          return fogad( uzenetsor ,getpid() ); 
          // The child process receives a message. 
		} 
     } 
     
     return 0; 
} 
