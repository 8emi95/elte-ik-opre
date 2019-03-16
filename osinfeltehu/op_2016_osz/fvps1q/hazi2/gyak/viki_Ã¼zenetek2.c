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
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 50 ]; 
}; 

// sendig a message
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
     
// receiving a message. 
int fogad( int uzenetsor) 
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
         // printf( "Lány: A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
			printf( "\nLány: Örül az a pici szívem!\n" ); 
     return 0; 
} 

int main (int argc,char* argv[]) { 
     pid_t lanygyerek;
	 pid_t fiugyerek;
	 
	 const char * lanynev="Anna";
	 const char * fiunev="Barna";
	 
	 int cso_esemeny_hely1[2]; 
	 int cso_esemeny_hely2[2];
     char csotartalom1[100]; 
	 char csotartalom2[100]; 
	 
	 signal(SIGTERM,handler);
	 
	 
     int uzenetsor, status; 
     
	 pipe(cso_esemeny_hely1);
	 pipe(cso_esemeny_hely2);
	 key_t kulcs; 
     
     // msgget needs a key, amelyet az ftok generál 
     //. 
     kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
 
	
     lanygyerek = fork(); 
     if ( lanygyerek > 0 ) { 
	 
			fiugyerek=fork();
			
			if (fiugyerek>0)
			{
				close(cso_esemeny_hely1[0]);
				write(cso_esemeny_hely1[1],lanynev,strlen(lanynev));
				close(cso_esemeny_hely1[1]);

				close(cso_esemeny_hely2[0]);
				write(cso_esemeny_hely2[1],fiunev,strlen(fiunev));
				close(cso_esemeny_hely2[1]);
				
				printf("\nBeleírt a szülő a csőbe\n");
				
				//kill(lanygyerek,SIGTERM);
			    //kill(fiugyerek,SIGTERM);

				//kuld( uzenetsor );  // Parent sends a message. 
				//wait( NULL ); 
				// After terminating child process, the message queue is deleted. 
				wait(NULL);
				wait(NULL);

				
				
				status = msgctl( uzenetsor, IPC_RMID, NULL ); 
				if ( status < 0 ) 
					perror("msgctl"); 
				return 0; 
				
				
			}
			else{
				sleep(3);
				close(cso_esemeny_hely2[1]);
				read(cso_esemeny_hely2[0],csotartalom1,sizeof(csotartalom1));
				close(cso_esemeny_hely2[0]);
					printf("\nFiú: megkaptam a nevem:%s\n", csotartalom1);
				
				sleep(3);
				//printf("%s",&kezdoszoveg);
				char src[50], dest[50];

				strcpy(src,  csotartalom1);
				strcpy(dest, "Nevem: ");

				strcat(dest, src);
			
				char  (*ip)[50];        /* pointer variable declaration */

				ip = &dest;
				
				printf("%s", *ip);
				
				const struct uzenet uz={5, "Fiú:Nevem Barna." } ;
							
				int status; 
     
				status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
				printf("%s", uz.mtext);
				
			
				//printf("\nFiú: név elkuldve!\n");
				
				//kill(lanygyerek,SIGTERM);		
			}
		  
     } else if ( lanygyerek == 0 ) { 
			sleep(4);
			close(cso_esemeny_hely1[1]);
			read(cso_esemeny_hely1[0],csotartalom2,sizeof(csotartalom2));
			close(cso_esemeny_hely1[0]);
					printf("\nLány: megkaptam a nevem:%s\n", csotartalom2);
				
		    sleep(5);
			 return fogad( uzenetsor ); 	
			 
	
	
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
