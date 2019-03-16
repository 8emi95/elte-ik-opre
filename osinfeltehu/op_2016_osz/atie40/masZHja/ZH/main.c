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
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void handler(int signumber){

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
int fogad( int uzenetsor , int l) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else if(l > 0){
		printf( "Az en sorszamom:  %s\n", uz.mtext );  
	 } else {
		 printf("Pot vagyok, ezt helyettesitem: %s\n",uz.mtext);
	 }
        
	int sorszam = atoi(uz.mtext);
	 status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
        printf( "A max sorszam:  %s\n", uz.mtext ); 
	int max = atoi(uz.mtext);
	
	printf("Utánnam még ennyi van: %d\n",max-sorszam);
	int r = rand() % 100;
	if(r < 10){
		if(l == 0){
			printf("Nem sikerult odaernem egyben\n");
		}
		kuld2(uzenetsor,"Bevontattak"); //hiba volt
		kill(getppid(),SIGTERM);
	} else {
		printf("Sikeresen odaertem\n");
		kuld2(uzenetsor,"Beertem"); //
		kill(getppid(),SIGTERM);
	}
	return 0;
} 

int fogad2( int uzenetsor, int l ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     else {
		 if (l==0) printf("Ezt kudlte: %s\n",uz.mtext);
	 }
	return strcmp(uz.mtext,"Beertem");
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
	
	int n = atoi(argv[1]);
	int i;
	int db =0;
	/* Start children. */
	for(i = 0; i<n; i++){
		rand();
		if ((pid = fork()) < 0) {
			perror("fork");
			abort();
		  }
		if (pid == 0) {
			//CHILD
			fogad( uzenetsor,1); 
			exit(0);
		} else {
			//PARENT
			kuld(uzenetsor,i+1);
			kuld(uzenetsor,n);
			sigset_t sigset;					
			sigfillset(&sigset);				
			sigdelset(&sigset,SIGTERM);			
			sigsuspend(&sigset);				
			if(fogad2(uzenetsor, 1) != 0){
				pid_t pid2;
				db++;
				if ((pid2 = fork()) < 0) {
					perror("fork");
					abort();
				}
				if(pid2 == 0){
					fogad(uzenetsor,0);
					return 0;
				} else {
					kuld(uzenetsor,i);
					kuld(uzenetsor,n);
					sigsuspend(&sigset);
					fogad2(uzenetsor, 0);
					wait();
				}
			}
			wait();
			if (i<n-1) printf("Indul az uj\n\n");
		}
	}
	status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
	printf("Potok szama: %d\n",db);
	return 0;
}