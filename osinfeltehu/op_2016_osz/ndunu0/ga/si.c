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
}; 

int kuld( int uzenetsor, struct uzenet uz ) { 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
	struct uzenet uz;
     int status;
     status = msgrcv(uzenetsor, &uz, 1024, 2, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "%s\n", uz.mtext ); 
     return 0; 
} 

void handler(int signumber){
  //printf("... %i\n",signumber);
}

void usr_handler(int signum){
	if( signum == SIGUSR1){
		printf("1-es jatekos kezd\n");
	} else if( signum == SIGUSR2) {
		printf("2-es jatekos kezd\n");
	}
}

int main(int argc, char* argv[]){
	const struct uzenet uz = { 2, "Hahahaha" }; 
	pid_t p1, p2;
	int uzenetsor;
	key_t kulcs; 

	srand(time(NULL));

	signal(SIGTERM,handler);
	signal(SIGUSR1,usr_handler);
	signal(SIGUSR2,usr_handler);
	
	kulcs = ftok("retech",1);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	} 

	p1=fork();
	if (p1==0)
	{ 
		pause();
		printf("1. jatekos csatlakozik\n");
		//sleep(2);
		kill(getppid(),SIGTERM);
		pause();
		return fogad( uzenetsor ); 
	}
	else 
	{
		p2 = fork();
		if(p2==0){
			pause();
			printf("2. jatekos csatlakozik\n");
			//sleep(2);
			kill(getppid(),SIGTERM);
			pause();
			return fogad( uzenetsor ); 
			
		}else {
			printf("Elkezdodott a jatek\n");
			sleep(3);
			kill(p1,SIGTERM);
			pause();
			printf("1. játékos bejelentkezett...\n");
			sleep(2);
			kill(p2,SIGTERM);
			pause();
			printf("2. játékos bejelentkezett...\n");
			sleep(3);
			int kezdo_j = rand() % 2 + 1;
			printf("Kezdo jatekos sorsolasa...\n");
			sleep(1);
			printf("(biro)Kezdo jatekos: %d\n", kezdo_j);
			if(kezdo_j == 1){
				kill(p1,SIGUSR1);
				const struct uzenet nyert = { 2, "1-es NYERT" };
				const struct uzenet nemnyert = { 2, "2-es NEM NYERT" };
				kuld( uzenetsor, nyert );
				sleep(2);
				kill(p2,SIGTERM);
				kuld( uzenetsor, nemnyert );
			} else if(kezdo_j == 2){
				const struct uzenet nyert = { 2, "2-es NYERT" };
				const struct uzenet nemnyert = { 2, "1-es NEM NYERT" };
				kill(p2,SIGUSR2);
				kuld( uzenetsor, nyert );
				sleep(2);
				kill(p1,SIGTERM);
				kuld( uzenetsor, nemnyert );
			}
			
			sleep(3);
			printf("Vege a jateknak\n");
			
			wait(NULL);

			return 0; 
		}

	}
	return 0;
}