#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/msg.h> 

#define MAX_SZAM 512

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char* mtext;	 
}; 

int kuld( int uzenetsor, pid_t pid , char* mtext) 
{ 
     const struct uzenet uz = { pid,  mtext}; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ){ 
          perror("msgsnd"); 
	      printf("valami");
	 }
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
	
	pid_t pid = getpid();
     status = msgrcv(uzenetsor, &uz, 1024, pid , 0 );  //getpid() a saját pid getppid() a szülőé
     
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
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
	 int a=0;
     child = fork(); 
     if ( child > 0 ) { 
	  
	     srand(time(NULL) + a);
		 int r = rand() % (MAX_SZAM);
	      char uzi[1024];
		  char* mas;
		  sprintf(mas,"%d",r);
		  strcpy(uzi,mas);
          kuld( uzenetsor,child,"valami" );  // Parent sends a message. 
          wait( NULL ); 
		  fogad(uzenetsor);
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) {
           pid_t parent = getppid();	 
	       return fogad(uzenetsor);
           //return kuld2( uzenetsor,parent ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
