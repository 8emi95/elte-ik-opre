#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct utas {
	 long mtype;
     int tszam; 
}; 

int kuld( int uzenetsor ) 
{ 
     const struct utas adat = {5, 2345}; 
     int status; 
     
     status = msgsnd( uzenetsor, &adat, strlen ( adat.tszam ) + 1 , 0 ); 

     if ( status < 0 )  perror("msgsnd"); 
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
     char adat[100];
     int status; 

     status = msgrcv(uzenetsor, &adat, 1000, 5, 0 ); 
     
     if ( status < 0 )  perror("msgsnd"); 
     else
          printf( "A kapott telefonszam:  %i\n", adat.tszam );
		  printf("Hívas!\n");
	  return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs;
	char *adat=argv[1];
 
     kulcs = ftok(argv[0],1);
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) {
		perror("msgget"); 
		return 1; 
     } 
     
     child = fork(); 
     if ( child > 0 ) { 
          wait( NULL ); 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
          return fogad( uzenetsor ); 

     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 



