#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h> //pipe
#include <sys/msg.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor) 
{ 
	const struct uzenet uz = { 5, "TEST" }; 
    int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");

	status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso előtti parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
        printf( "A szerelvény hanyadik:  %s\n", uz.mtext ); 
     return 0; 
} 

int main(int argc,char ** argv) {
	pid_t pids[2];
	int uzenetsor, status; 
	key_t kulcs; 
	
	kulcs = ftok(argv[0],1); 
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	}
	
	int i;
	/* Start children. */
	for (i = 0; i < 2; i++) {
	  if ((pids[i] = fork()) < 0) {
		perror("fork");
		abort();
	  } else if (pids[i] == 0) {
		//CHILD
		fogad( uzenetsor ); 
		exit(0);
	  }
	}

	//PARENT
	kuld( uzenetsor);
	wait();
	wait();
	status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
	return 0;
}