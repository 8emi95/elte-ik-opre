#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <signal.h>
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void signalUser1Handler(int signumber){
	printf("Machine has arrived to station!\n");
}

// sendig a message
int kuld( int uzenetsor, char* msg ) 
{ 
     struct uzenet uz = { 5, "" }; 
	 strcpy(uz.mtext, msg);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor, char* msg ) 
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
	 {
		 //printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
		 strcpy(msg, uz.mtext);
	 }
	  
     return 0; 
} 

int getRandomValue(int maxValue) {
	if (maxValue < 0) {
		return 0;
	}
	int r=(rand()%(maxValue)) + 1;
	return r;
}

int main (int argc, char* argv[]) { 
	srand(time(NULL));
	
	if (argc!=2) {
		perror("Please enter maximum train number as argument!\n");
		exit(1);
	}
	int MACHINE_INDEX = 0;
	int MAX_NUMBER = atoi(argv[1]);
	if (MAX_NUMBER <= 0) {
		perror("Invalid argument, expecting integer > 0!\n");
		exit(1);
	}

	signal(SIGUSR1,signalUser1Handler);
	
     pid_t child; 
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
     
     child = fork(); 
     if ( child > 0 ) {  
		MACHINE_INDEX = getRandomValue(MAX_NUMBER);
		char message[1024];
		sprintf(message, "%i,%i", MACHINE_INDEX, MAX_NUMBER);
		printf ("Sending: %s\n",message);
		  
		  kuld( uzenetsor, message );  // Parent sends a message.
		  
		  pause();
		  
		  char msg[1024];
		  int res = fogad( uzenetsor, msg ); 
		  printf("Received from child: %s\n", msg);
		  
		  wait( NULL ); 
		  
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } else if ( child == 0 ) { 
		char msg[1024];
		int res = fogad( uzenetsor, msg ); 
		printf("Received: %s\n", msg);
		
		char *p;
		int myNum;
		int maxNum;
		p = strtok(msg, ",");
		myNum = atoi(p);
		p = strtok(NULL, ",");
		maxNum = atoi(p);
		
		printf("Hi, i'm machine umber %d!\n", myNum);
		printf("There will come %d machines after me!\n", maxNum - myNum);
		
		char result[50];
		int random = getRandomValue(100);
		printf("Random number: %i\n\n", random);
		if (random <= 50) {
			strcpy(result, "I slipped on a banana!");
		} else if (random <= 10) {
			strcpy(result, "I froze!");
		} else {
			strcpy(result, "Arrived safely!");
		}
		
		printf("My result: %s\n", result);
		
		kill(getppid(),SIGUSR1);
		
		kuld( uzenetsor, result );
		
		return 0;
          // The child process receives a message. 
     } else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
