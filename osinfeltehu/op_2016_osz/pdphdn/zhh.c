#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <signal.h>
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
int kuld( int uzenetsor, const struct uzenet uz ) 
{ 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof (uz) , 0 ); 
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
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, uz, sizeof (*uz), 0, 0 ); 
     return status;
}

static int beert = 0;

void sikeres(int signumber){
  beert = 1;
  printf("Az inditott szerelveny sikeresen beert\n");
}

void sikertelen(int signumber){
  beert = 0;
  printf("Az inditott szerelveny nem ert be\n",signumber);
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs;

     signal(SIGUSR1,sikeres);
     signal(SIGUSR2,sikertelen);

     srand(time(NULL));

     const long sz_szam = atoi (argv[1]); 
     
     kulcs = ftok(argv[0],1); 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) {
	  int i;
	  for (i=1; i<=sz_szam; ++i) {
	  struct uzenet uz = { i, sz_szam, "Inditom a szerelvenyt!" };  
          kuld( uzenetsor, uz );  // Parent sends a message. 

          // After terminating child process, the message queue is deleted. 
          
	  pause();

	  if (beert) {
	    sleep (1);
            continue;
          }

     	  if (fogad( uzenetsor, &uz) < 0 ) 
          	perror("msgsnd"); 
    	  else {
          	printf( "A vonat: %s\n", uz.mtext );
                --i;
	  }
	  }
          //status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          //if ( status < 0 ) 
          //     perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          struct uzenet uz;
          do {
	  if (fogad( uzenetsor, &uz) < 0 ) 
          	perror("msgsnd"); 
    	  else
          	printf( "En a %ld. szerelveny vagyok, utanam meg %ld jon\n", uz.mtype, (uz.mtype2-uz.mtype), uz.mtext ); 

	 int r=rand()%101;
                 
          if (r>=90) {
		 kill(getppid(),SIGUSR2);
		 r = rand()%2;
                 if (r==0) 
		 	strcpy (uz.mtext,  "Elcsuszott egy bananon!");
                 else
			strcpy (uz.mtext,  "Lefagyott a zord telben!");

		 kuld ( uzenetsor, uz );
                 exit (0);   
	  } else {
		  kill(getppid(),SIGUSR1);
	  }
          } while (uz.mtype <= uz.mtype2); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
