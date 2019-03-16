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
     long mtype;
     long mtype2;
     char mtext [ 1024 ]; 
};
     
int fogad( int uzenetsor, struct uzenet* uz ) 
{ 
     int status; 
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
          long r=rand()%sz_szam+1;
	  struct uzenet uz = { r, sz_szam, "Inditom a szerelvenyt!" };  
          msgsnd( uzenetsor, &uz, sizeof (uz) , 0 );
	  pause();

	  if (beert)
	    return 0; 
		msgrcv(uzenetsor, uz, sizeof (*uz), 0, 0 ); 
        printf( "A vonat: %s\n", uz.mtext ); 

          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
        struct uzenet uz;
        msgrcv(uzenetsor, uz, sizeof (*uz), 0, 0 ); 
        printf( "En a %ld. szerelveny vagyok, utanam meg %ld jon\n", uz.mtype, (uz.mtype2-uz.mtype), uz.mtext ); 

	 int r=rand()%101;
                 
          if (r>=90) {
		 kill(getppid(),SIGUSR2);
		 r = rand()%2;
                 if (r==0) 
		 	strcpy (uz.mtext,  "Elcsuszott egy bananon!");
                 else
			strcpy (uz.mtext,  "Lefagyott a zord telben!");

		 msgsnd( uzenetsor, &uz, sizeof (uz) , 0 );
	  } else {
		  kill(getppid(),SIGUSR1);
	  }  
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
