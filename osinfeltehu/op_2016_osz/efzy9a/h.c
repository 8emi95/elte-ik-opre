#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <signal.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int szam;
	 char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
	srand(time(NULL));
	int szam = (rand() % (50 + 1 - 20)) + 20;
     const struct uzenet uz = { 5, szam, " " }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(struct uzenet) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
 
int kuld2( int uzenetsor ) 
{ 
	//srand(time(NULL));
	//int szam = (rand() % (50 + 1 - 20)) + 20;
    int szam = 0;
	const struct uzenet uz = { 6, szam, "csak várj, míg a kezünk közé nem kerülsz" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(struct uzenet) , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, sizeof(struct uzenet), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "%i\n", uz.szam ); 
     return 0; 
} 

int fogad2( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, sizeof(struct uzenet), 6, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Kevin: %s\n", uz.mtext ); 
     return 0; 
}

void handler(int signumber){
  printf("Jelzes\n");//%i has arrived\n",signumber);
}


int main (int argc, char* argv[]) { 
     pid_t child; 
	 pid_t child2;
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
	 
     signal(SIGTERM,handler);// jelzes
     
	 int childToChild2[2];// pipe
	 int child2ToChild[2];// pipe
	 pipe(childToChild2);
	 pipe(child2ToChild);
	 
     child = fork();// Kevin
     if ( child > 0 ) {
			child2 = fork();// betoro
			if(child2 > 0){
			  wait( NULL ); 
			  wait( NULL ); 
			  // After terminating child process, the message queue is deleted. 
			  status = msgctl( uzenetsor, IPC_RMID, NULL );
			}
			else if(child2 == 0){// Betörő
				kuld( uzenetsor );  // Parent sends a message. 
			    fogad2(uzenetsor);
				
				kill(child,SIGTERM);
			    
				close(childToChild2[1]);
				char uzenetp[50];
				read(childToChild2[0], uzenetp, sizeof(uzenetp));
				printf("Betörő: %s\n", uzenetp);
				
				int rszam = rand() % 2;
				if(rszam == 0){
					// hatasos volt
					strcpy(uzenetp, "na megállj csak");
					close(child2ToChild[0]);
					write(child2ToChild[1], uzenetp, sizeof(uzenetp));
				
				}
				else{
					// nem volt hatásos
					strcpy(uzenetp, "nem úszod meg szárazon");
					close(child2ToChild[0]);
					write(child2ToChild[1], uzenetp, sizeof(uzenetp));
				}
				return 0;
			}else{
				perror("fork2");
				return 2;
			}
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { // Kevin
          fogad( uzenetsor ); 
          kuld2(uzenetsor);	  
		  
		  pause();
		  close(childToChild2[0]);
		  write(childToChild2[1], "festék", sizeof("festék"));
		  
		  char uzenetq[50];
		  close(child2ToChild[1]);
		  read(child2ToChild[0], uzenetq, sizeof(uzenetq));
		  printf("Kevin: %s\n", uzenetq);
		  return 0;
		  // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
