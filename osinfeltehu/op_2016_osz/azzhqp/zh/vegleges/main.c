#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h> //pipe
#include <sys/msg.h> 
#include <signal.h>	

struct uzenet { 
     long mtype;
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){
	printf("Egy szerelveny megerkezett\n");
}

// sendig a message
int kuld( int uzenetsor, int r) 
{ 
	int aInt = 368;
	char str[15];
	sprintf(str, "%d", r);
	struct uzenet uz = { 5, "" }; 
	strcpy(uz.mtext,str);
	
    int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");
     return 0; 
} 
 
int kuld2( int uzenetsor, char s[]) 
{ 
	struct uzenet uz = { 5, "" }; 
	strcpy(uz.mtext,s);
	
    int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
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
        printf( "Szerelvényszám:  %s\n", uz.mtext ); 
	int sorszam = atoi(uz.mtext);
	 status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
        printf( "MAX:  %s\n", uz.mtext ); 
	int max = atoi(uz.mtext);
	
	printf("Aktuális utáni db: %d\n",max-sorszam);
	int r = rand() % 100;
	if(r < 10){
		printf("Nem sikeres!\n");
		kuld2(uzenetsor,"Bevontattak"); //hiba volt
		kill(getppid(),SIGTERM);
	} else {
		printf("Sikeres!\n");
		kuld2(uzenetsor,"Beert!"); //
		kill(getppid(),SIGTERM);
	}
	return 0;
} 

int fogad2( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else {
		 printf("Ezt kudlte: %s\n",uz.mtext);
	 }
	return 0;
} 

int main(int argc,char ** argv) {
	srand(time(NULL));
	pid_t pid;
	int uzenetsor, status; 
	key_t kulcs; 

	struct sigaction sigact;
	sigact.sa_handler=handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags=0;
	sigaction(SIGTERM,&sigact,NULL);

	kulcs = ftok(argv[0],1); 
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	}
	
	int max = atoi(argv[1]);
	int i;
	//children
	if ((pid = fork()) < 0) {
		perror("fork");
		abort();
	  }
	if (pid == 0) {
		//CHILD
		fogad( uzenetsor ); 
		exit(0);
	} else {
		//PARENT
		int  r = rand() % max + 1;
		kuld(uzenetsor,r);
		kuld(uzenetsor,max);
		sigset_t sigset;					
		sigfillset(&sigset);				
		sigdelset(&sigset,SIGTERM);			
		sigsuspend(&sigset);
		fogad2(uzenetsor);
		wait();
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	}
	
	
	if ( status < 0 ) 
		perror("msgctl"); 
	return 0;
}