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
int kuld( int uzenetsor) 
{ 
     
     srand(time(NULL));
     int r = rand()% 30+20;
	 char uzen[100];
     sprintf(uzen,"%d",r);
	 struct uzenet uz;
	 uz.mtype=5;
	 strcpy(uz.mtext,uzen); 
	 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd");
	  
	  
     printf("Kuld vege\n");	  
     return 0; 
} 
     
int kuld2( int uzenetsor) 
{ 
     
 
	 struct uzenet uz={5,"CSak varj amig..."};
	 
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
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child1,child2; 
     int uzenetsor, status; 
     key_t kulcs; 
 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( IPC_PRIVATE, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
    

	 child1=fork();
     if ( child1 == 0 ) { 
	      printf("Kevin\n");
	      kuld(uzenetsor);
         // fogad(uzenetsor);		  
	    }else{
			 child2=fork();
			 if(child2==0){
				 printf("Betoro\n");
				 fogad(uzenetsor);
				// kuld2(uzenetsor);
				 
				 			 
			 }
			 	 
		else{
				 printf("Szulo");
				 waitpid(child2, &status, 0); 
		         waitpid(child1, &status, 0); 
				 
				 status = msgctl( uzenetsor, IPC_RMID, NULL ); 
                 if ( status < 0 ) 
                 perror("msgctl"); 	
				 }
			 
		}
		  
		
     return 0; 
} 
