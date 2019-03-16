#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h> 
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct uzenet { 
     long mtype; 
     char mtext [ 1024 ]; 
}; 

// Üzenetet küld az üzenetsorba. 
int kuld( int uzenetsor, struct uzenet* uz ) { 

     int status; 
	 
     printf("Uzenet kuldese csatornan: %s \n", uz->mtext);
     status = msgsnd( uzenetsor, uz, strlen ( uz->mtext ) + 1 , 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// Üzenetet fogad az üzenetsorból. 
int fogad( int uzenetsor ) { 
     struct uzenet uz; 
     int status; 
     
     status = msgrcv( uzenetsor, & uz, 1024, 0, 0 ); 
     printf("Fogadott uzenet: %s \n",  uz.mtext);
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
	 pid_t child2;
     int uzenetsor, uzenetsor2,  status; 
     key_t kulcs; 
     

     kulcs = ftok(argv[0],1); 
     
     uzenetsor = msgget( kulcs, 0700 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 						// létrejön child
     if ( child > 0 ) {  					//SZÜLŐK
		printf("Szulo: Otthon felejtettuk Kevint :( \n");
								// várakozás Kevinre
		child2 = fork(); 					 //létrejön child2
			if ( child2 > 0 ) {				//SZÜLŐK
				wait( NULL ); 				//várakozás a betörőkre
			} else if ( child2 == 0) {		//BETÖRŐK
			printf("Betoro: ");
			fogad( uzenetsor );				//fogadják a rakás apró játékot
			printf("Betoro: *hanyatt esik*\n");
			struct uzenet uz = {1, "Csak varj, amig a kezunk koze nem kerulsz!\n"};
			printf("Betoro: ");
			kuld( uzenetsor, &uz );  			// A szülőfolyamat küldi az üzenetet.
			  return 0; 
			} else {
				perror("fork");
				return 1;
			}
			
			wait( NULL ); 
			printf("Szulo vege.\n");
			
     } else if ( child == 0 ) { 			//KEVIN
			srand(time(NULL));
			int r = rand() % 30 + 20;
			char ran[2];
			sprintf(ran, "%d", r);
			struct uzenet uz;
			uz.mtype = 1;
			strcpy( uz.mtext, ran);
			printf("Kevin: *ledob %d darab apro jatekot a foldre...* \n",r);
			printf("Kevin: ");
			kuld( uzenetsor, &uz);
			sleep(1);
			printf("Kevin: ");
			fogad( uzenetsor ); 
			return 0;
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
}


