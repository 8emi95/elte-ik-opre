#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // for errno, the number of last error
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
	 long mtype;
     char mtext [ 1024 ]; 
}; 
int kuld();
int fogad();

// sendig a message
int kuld( int uzenetsor,int type, char* msg ) 
{ 
printf("%s",msg);
    struct uzenet uz; 
	 uz.mtype=type;
	 sprintf(uz.mtext,"%s",msg);
     int status; 
     status = msgsnd( uzenetsor, &uz, sizeof (uz.mtext) , 0 ); 
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
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
         printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return atoi(uz.mtext); 
} 


int main(int argc, char* argv[]) 
{
	pid_t pid = fork();
	int uzenetsor, status; 
    key_t kulcs; 
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
	    perror("msgget"); 
		return 1; 
	} 
	
    srand(time(NULL));
	
	if(pid>0){ //parent
		int id= (rand() % atoi(argv[1]))+1;
		char x[1024];
		sprintf(x,"%i",id);
		kuld( uzenetsor,5 ,x);  // Parent sends a message. 

          // After terminating child process, the message queue is deleted. 
        status = msgctl( uzenetsor, IPC_RMID, NULL ); 
        if ( status < 0 ) 
            perror("msgctl"); 
		
	} else { // child
		int id = fogad( uzenetsor ); 
		printf("En vagyok a %i. szerelveny\n",id);
		int fail=3;
			if((rand() % 10) == 0)
				fail=(rand() % 2);
		char tmp[50];
		if(fail==3)
			printf("Sikeres volt a muszak!\n");
			else if (fail==2)
				printf("Megcsusztam egy bananhejon!\n");
			else
				printf("Lefagyott a sin!\n");
		sprintf(tmp,"%s",fail);

			
		exit(1);
	}	
};