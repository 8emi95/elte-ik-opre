#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#define MAX_SZAM 512


struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	 char mtext[1024];
	}; 

// sendig a message
int kuld( int uzenetsor, pid_t pid, int max) 
{     
      int a=0;
      srand(time(NULL) + a);
      int r = rand() % max;
	  
	  char str[100]="probe";
	  //sprintf(str,"szam %d\n",r);
      

	  
	 struct uzenet uz={pid,"dd"};
     //uz.mtype = pid;
     //strcpy(uz.mtext,"ddd");	 
	  
     int status;
     
     status = msgsnd( uzenetsor, &uz, strlen( uz.mtext ) + 1 ,0 ); 
	     // a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ){ 
          perror("msgsnd");
	 }
     return 0; 
} 


/*int kuld2( int uzenetsor, pid_t pid,int max ) 
{ 
    
	   char str[10]="hsdbdddd";
	   sprintf(str,"szam %d\n",max);
	   const struct uzenet uz = { pid, str }; 
       int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 


int kuld3( int uzenetsor, pid_t pid) 
{     
    
	 const struct uzenet uz = { pid,"Elromlottam"}; 
	  
     int status;
     
     status = msgsnd( uzenetsor, &uz, strlen( uz.mtext ) + 1 , 0 ); 
	     // a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ){ 
          perror("msgsnd");
	 }
     return 0; 
} 
int kuld4( int uzenetsor, pid_t pid) 
{     
    
	 const struct uzenet uz = { pid,"Jo voltam"}; 
	  
     int status;
     
     status = msgsnd( uzenetsor, &uz, strlen( uz.mtext ) + 1 , 0 ); 
	     // a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ){ 
          perror("msgsnd");
	 }
     return 0; 
} */
     
// receiving a message. 
int fogad( int uzenetsor ) 
{    

     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
	 
	 pid_t pid = getpid();
     status = msgrcv(uzenetsor, &uz, 1024, pid , 0 );  //getpid() a saját pid getppid() a szülõé
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( " szovege:  %s\n", uz.mtext ); 
     return 0; 
} 


void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  
  
  
}

int main (int argc, char* argv[]) { 
     
	 
	 /*
	 int max;
	 printf("max szam:");
	 scanf("%d",max);
	 */
	 
	 pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 
     signal(SIGTERM,handler); 
	
	 int max=100;
	
	 int a =0;
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
    
     child = fork(); 
     if ( child > 0 ) { 
	     
		 
          kuld( uzenetsor,child,max);
          /*kuld2(uzenetsor,child,max);		  // Parent sends a message. 
          pause();
          fogad(uzenetsor);		  
		 // fogad(uzenetsor);
          // After terminating child process, the message queue is deleted. */
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) {
           pid_t parent = getppid();	 
	       fogad(uzenetsor);
		   /*fogad(uzenetsor);
		   
		   
		           a++;
				   srand(time(NULL) + a);
			       int r2 = rand() % 10;  
			       if(r2 == 1)
			      {
				     printf("Nm voltam sikeres");
					 kill(getppid(),SIGTERM);
			         kuld3(uzenetsor,getppid());
			      } else{
					  kill(getppid(),SIGTERM);
			          kuld4(uzenetsor,getppid());
					  
				  }  
			   
			   */

			   
           //return kuld2( uzenetsor,parent ); 
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
