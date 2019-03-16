
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

void handler(int signalnumber){
 printf("Erkezett egy utas!\n");
 
}

struct uzenet { 
     char mtext [ 1024 ]; 
}; 


// sendig a message
int kuld( int uzenetsor,  struct uzenet uz ) 
{ 
   
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, int fogadSzam ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, fogadSzam, 0 ); // 5-os melyik azonosíto uzenetet fogadja, pl 5helyett 3 => nem fogadja, vár ha 0 mindent megkap
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtext ); 
     return 0; 
} 

int main(int argc, char *argv[]) {
	
	
   signal(SIGTERM,handler);
   // cso letrehozasa
   int pipeUtastol[2];
	

	  if (pipe(pipeUtastol) == -1)
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
	
	///uzenet sorhoz:
	 int uzenetsor, status; 
     key_t kulcs; 
     
    
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
	 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
	
	
	
	
  int fork1 = fork();
  
 
  if (fork1 < 0) {
    printf("error\n");
  } else if (fork1 == 0) {
	  /// UTAS
    printf("En vagyok az utas (gyerek)! \n");
    printf("utas: parent: %i\n", getppid());
    printf("utas1: my pid: %i\n", getpid());
	
	sleep(1);
	// utas jelez a 
	kill(getppid(),SIGTERM); 
	
	close(pipeUtastol[0]);// lezarjuk az olvaso veget a csonek

	int i =0;
	char osszCim[100]="";

	  for (i; i<argc-1; i++){
			 
			 strcat(osszCim, argv[i+1]);
			 strcat(osszCim, " ");
	  }
	  
   
	   
	 write(pipeUtastol[1],osszCim,80);	
	 close(pipeUtastol[1]);// lezarjuk az iro veget a csonek
	
  } else {
    int fork2 = fork();
    if (fork2 < 0) {
      printf("error\n");
    } else if (fork2 == 0) {
		
		/// TAXIS
      printf("En vagyok az auto (gyerek)!\n");
      printf("auto parent: %i\n", getppid());
      printf("auto 2: my pid: %i\n", getpid());
	  
	  fogad( uzenetsor, 0 );//mindent fogadunk?
	  
    } else {
		
      printf("En vagyok a KOZpont! (szulo)\n");
      printf("The PIDs are:\n");
      printf("kozpont pid: %i\n", getpid());
      printf("utas 1: %i\n", fork1);
      printf("auto 2: %i\n", fork2);
	  
	
	
	 pause();  // varjuk az utas bejelentkezését!
	 printf("jott egy utas");
	   
	   
	  close(pipeUtastol[1]); // lezarjuk az iro veget
	  char utasCime[80];  
	  
	
		int i =0;
	
	   read(pipeUtastol[0],utasCime,sizeof(utasCime));	
	   printf("kapott cim az utastol %s \n",utasCime);	
	   close(pipeUtastol[0]); // lezarjuk az olvaso veget
	
	
	///UZENETSOR ELKULDESE 
		struct uzenet uz;
		 strcpy(uz.mtext,utasCime);
		 kuld( uzenetsor, uz );
	   
		int status;
		wait(&status);
	    printf("a kozpont bezart \n");
    }
  }
  return 0;
}