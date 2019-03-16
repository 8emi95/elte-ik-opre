#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <mqueue.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h> 

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 


int kuld( int uzenetsor) 
{ 
/*	
	
     const struct uzenet uz = { 5, "Fiú: Küldöm a nevem, Barnus vagyok."}; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	 printf("%s", uz.mtext);
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; */
} 
     

int fogad( int uzenetsor) 
{ 
     struct uzenet uz; 
     int status; 
    
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "%s\n",  uz.mtext ); 
			
     return 0; 
} 

int main (int argc,char* argv[]) { 
     pid_t gyerek;
	 
	 signal(SIGTERM,handler);
	 	 
     int uzenetsor, status; 
     
	 key_t kulcs; 
     kulcs = ftok(argv[0],1); 
	 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
 
	
    gyerek = fork(); 
     if (gyerek > 0 ) { 
				int szam=0;
				srand(time(NULL));
				while (szam<20) {
				int r2 = rand() % 51;
				szam=r2;
				}
				
				char szamstr[1024];		
				sprintf(szamstr,"%i",szam);

				char  (*ip)[1024];
				ip = &szamstr;
	 
				//printf("Gyerek: nesze, dobok játékokat: %s\n", *ip);
				
				struct uzenet uz={5, ""} ;
				strcpy(uz.mtext,*ip);
				
				int status; 
     
				status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
				if ( status < 0 ) 
				perror("msgsnd"); 
				
				
				pause();


				
				fogad( uzenetsor ); 	
				
				
				
				wait(NULL);
				
				status = msgctl( uzenetsor, IPC_RMID, NULL ); 
				 if ( status < 0 ) 
				 perror("msgctl"); 
				 
				
			}
			else{
				
				sleep(1);
				
				fogad( uzenetsor ); 	
				
				
				const struct uzenet uz={5, "Betörő: Várj csak, míg a kezünk közé nem kerülsz!" } ;
							
				int status; 
     
				status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
				if ( status < 0 ) 
				perror("msgsnd"); 
				
				
				kill(getppid(),SIGTERM);		
			}
		  
     
     return 0; 
} 
