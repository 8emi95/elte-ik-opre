
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/time.h> 

#include <sys/ipc.h> 
#include <sys/msg.h>
#include <wait.h>  

volatile int done_waiting = 0;

void handler( int signum ){
    if (signum == SIGUSR1)
    {
        done_waiting = 1;
    }    
}

struct uzenet { 
      long mtype;
      char mtext [ 80 ]; 
	}; 

int kuld( int uzenetsor , int cimzett, char* uzenet ) 
{    
     
	 struct uzenet uz;	
	 uz.mtype = cimzett;
	 stpcpy( uz.mtext , uzenet );
	    
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
	
int fogad( int uzenetsor, int cimzett ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 	
     status = msgrcv(uzenetsor, &uz, 80, cimzett, 0 ); 
     
     if ( status < 0 ) 
          perror("msgrcv"); 
    // else
    //      printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 

	
int main(int argc, char** argv) {
	
	
	int jatekDB;
	int uzenetsor;
	int msgqstatus;
	int statusbetoro, statuskevin;
	int i;
	struct uzenet rec_uz;
	
	 key_t kulcs;
	 kulcs = ftok(argv[0],1);     
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
    
	char uzi[80]="jatek";
	char fogadott_uzi[80];
	
		srand(time(NULL)); 
		
		pid_t kevin = fork();
			
		//szulo folyamat:    
		if(kevin > 0){
			
			pid_t betoro = fork();
			
			waitpid( betoro, &statusbetoro, 0);
			printf("\n betoro veget ert \n");
			
			if(betoro > 0){
				
				
			}
			//betoro folyamat
			else if(betoro == 0){
				int status; 
				printf("\n betoro vagyok \n");
				msgrcv(uzenetsor, &rec_uz, 80, 2, 0 ); 
				//ennyi jatekot szor ki kevin
				int darab = atoi( rec_uz.mtext );
				printf("\n Bet: AAa Kevin, elestem a ");
				for(i=0; i< darab; i++){
					msgrcv(uzenetsor, &rec_uz, 80, 2, 0 ); 
					if(i<darab-1) {
					printf(" %s, ", rec_uz.mtext); 
					}else{
							printf(" %s jatekodban!\n", rec_uz.mtext);
					}
			
				}
				fflush(stdout);
				kuld( uzenetsor, 1, "csak varj mig a kezunk koze nem kerulsz");
				
				//exit(0);
			}
			else{
				printf(" Fork Betoro hiba! "); 
			}
			
			/*msgqstatus = msgctl( uzenetsor, IPC_RMID, NULL ); 
			if ( msgqstatus < 0 ) perror("msgctl"); */
		}	
		//kevin folyamat
		else if( kevin == 0) {	
					
			jatekDB = (rand()%(50-20))+20;
			//printf("\n kev : %d ", jatekDB);fflush(stdout);
			
			char darabStr[10];
			//eloszor bekuldi hany darab jatekot szor ki
			sprintf( darabStr, "%d", jatekDB);
			kuld( uzenetsor, 2, darabStr );
			
			for(i=0; i< jatekDB; i++){
				char temp[10];
				sprintf(temp, "%d", i);
				strcat(uzi, temp);
				//printf("\n %s", uzi);
				kuld( uzenetsor, 2, uzi );				
				memset(temp, 0, 10);
				memset(uzi, 0, 80);
				strcpy(uzi, "jatek");
			}
			
			msgrcv(uzenetsor, &rec_uz, 80, 1, 0);
			printf(" \n Kev: Juj a Betoro azt uzente: \"%s\" ", rec_uz.mtext);
			fflush(stdout);
			//exit(0);
		}
		else {
			printf(" Fork hiba! "); 
		}
	
			fflush(stdout);
			waitpid( kevin, &statuskevin, 0);
			printf("\n kevin veget ert \n");
				
				
		msgqstatus = msgctl( uzenetsor, IPC_RMID, NULL ); 
			if ( msgqstatus < 0 ) perror("msgctl"); 
	
return 0;
}

