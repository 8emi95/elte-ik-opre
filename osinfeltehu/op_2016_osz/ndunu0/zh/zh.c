#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
	 int szer;
}; 

int kuld( int uzenetsor, char* msg, int szer) { 
     int status; 
	 struct uzenet uz = { 1, "", szer };
	 strcpy(uz.mtext, msg);
     
	 
     status = msgsnd( uzenetsor, &uz, sizeof uz , 0 );
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int fogad( int uzenetsor, char* msg) 
{ 
	struct uzenet uz;
	int status;
	status = msgrcv(uzenetsor, &uz, sizeof uz, 1, 0 ); 

	if ( status < 0 ) 
	  perror("msgsnd"); 
	else{
	  printf( "%s\n", uz.mtext );
	  strcpy(msg,uz.mtext);
	}
	return uz.szer; 
} 

void handler(int signumber){
  //printf("... %i\n",signumber);
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	
	pid_t child;
	key_t kulcs; 
	int uzenetsor;
	
	int maxSzer = atoi(argv[1]);
	
	kulcs = ftok(argv[0],1);
	//uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	} 
	
	//printf("max szerelveny: %d\n",maxSzer);
	
	signal(SIGTERM,handler);
	
	child = fork();
	
	if(child == 0){ //child
		pause();
		int rn = rand() % 100 + 1;
		int maxsz, sze;
		char* m;
		//maxsz = fogad( uzenetsor, m );
		//printf("Szerelveny: %d\n", maxsz);
		sleep(2);
		kill(getppid(),SIGTERM);
		if(rn < 11){
			kill(getppid(),SIGTERM);
			sleep(1);
			
			//kuld( uzenetsor, "ELCSUSZOTT", 1);
		} else {
			kill(getppid(),SIGTERM);
			sleep(1);
			//kuld( uzenetsor, "SIKERÜLT", 1);
		}
		return 0;
	} else if(child > 0){ //parent
		int rn = rand() % 100 + 1;
		int szerelveny = rand() % maxSzer + 1;
		printf("Budapest Metro Funs\nMai szerelvény szám: %d\n",szerelveny);
		//printf("Kapott szerelveny: %d\n",szerelveny);
		sleep(2);
		kill(child,SIGTERM);
		char n[100];
		sprintf(n, "%d", szerelveny);
		//kuld( uzenetsor, n, szerelveny);
		char* d;
		//int t = fogad(uzenetsor,d);
		
		
		if(rn < 11){
			printf("ELCSÚSZOTT\n");
		} else {
			printf("SIKER\n");
		}
		wait(NULL);
	} else {
		perror("fork fail");
		exit(1);
	}
	
	return 0;
}








