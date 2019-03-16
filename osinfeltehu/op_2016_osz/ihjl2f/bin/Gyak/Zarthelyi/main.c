#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <sys/time.h>
#include <time.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, char* msg ) 
{ 
     struct uzenet uz = { 4, "" };
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
int fogad( int uzenetsor)//, char* message ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 )  {
		perror("msgsnd");  
	 } else {
			printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
			//strcpy(message, uz.mtext);
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

int main(int argc,char** argv){
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
	
	printf("\n\nLet's get started!\n\n");
	
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
	
	pid_t child=fork(); //forks make a copy of variables
	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (child>0) 
	{
		MACHINE_INDEX = getRandomValue(MAX_NUMBER);
		char message[1024];
		sprintf(message, "%i,%i", MACHINE_INDEX, MAX_NUMBER);
		printf ("Sending: %s\n",message);
		
		kuld( uzenetsor, message );
		
		wait( NULL ); 
		// After terminating child process, the message queue is deleted. 
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
		if ( status < 0 ) 
		   perror("msgctl"); 

		printf("Ferwell!\n\n");
		return 0;
	}
	else //child process
	{/*
		char msg[1024];
		int res = fogad( uzenetsor, msg );
		printf("Received: %s\n", msg);
		
		return res;
		*/
		return fogad( uzenetsor );
		
		/*
		int myNumber;
		int maxNumber;
		read(pipe_parent_child[0], &myNumber, sizeof(myNumber));
		printf("I got my number: %i\n", myNumber);
		read(pipe_parent_child[0], &maxNumber, sizeof(maxNumber));
		printf("I got the max number: %i\n\n", maxNumber);
		
		char result[50];
		int res = getRandomValue(100);
		printf("Random number: %i\n\n", res);
		if (res <= 5) {
			strcpy(result, "I slipped on a banana!");
		} else if (res <= 10) {
			strcpy(result, "I froze!");
		} else {
			strcpy(result, "Arrived safely!");
		}
		
		write(pipe_child_parent[1], &result, sizeof(result));;*/
	}

	return 0;
}
	