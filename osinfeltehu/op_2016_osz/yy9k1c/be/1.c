#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 


struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){
	printf("Vegallomas.\n",signumber);
}

int kuld( int uzenetsor ) { 

	srand(time(NULL));
	int rndm = rand() % 100;
	char randomNumber[5];
	sprintf(randomNumber, "%d", rndm);

     const struct uzenet uz = { 5, "5" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
int fogad( int uzenetsor ) { 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Ma a %s. vagyok.\n", uz.mtext ); 
     return 0; 
} 

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status; 
     key_t kulcs; 

     kulcs = ftok(argv[0],1); 
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
     
     child = fork(); 
	 
     if ( child > 0 ) {
          kuld( uzenetsor );
          wait( NULL ); 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
     } 
	 
	 else if ( child == 0 ) { 
          return fogad( uzenetsor ); 
     }
	 
	 else { 
          perror("fork"); 
          return 1; 
     } 
     
     return 0; 
} 
