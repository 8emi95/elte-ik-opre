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
     struct uzenet uz ;
	 int max = 100;
	srand(time(NULL));
     int r = rand()% 30+20;
	  
     char* uzen;
     sprintf(uzen,"%d",r);
	 uz.mtype =pid;
	 strcpy(uz.mtext,uzen);
     int status;      
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
int kuld2( int uzenetsor,pid_t pid ) 
{ 
     struct uzenet uz ={pid, "csak varj mig a kezunk koze kerulsz!"} ;
	 
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
    
     int uzenetsor;
     key_t kulcs; 
     
	 
	 
	 int status; 
     pid_t child1, child2; 
	 
	 
     kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	 
	 
	 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child1 = fork(); 
     if ( child1 > 0 ) { 
	      printf("Szulo");
		  fogad(uzenetsor);

          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
		  
	 }else{
		 if(child1==0){}
		 printf("Szulo");
		 kuld(uzenetsor,child2);
		 
	 }
	

	
	
	
	
	
	
/*	
	
	
	{
		 child2 = fork();
		  if (child2 == 0) {
			printf("A Betroro vagyok!") ; 
			fogad(uzenetsor);
		  }else{
			  
			  waitpid(child1,&status,0);
			  waitpid(child2, &status, 0);
			  printf("Bevartam Kevint es a betorot is!");
			  
		  }
	 }

     */
     return 0; 
} 
