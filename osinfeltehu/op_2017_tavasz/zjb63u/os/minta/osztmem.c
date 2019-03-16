#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <time.h>

void signal_1(int param)
{
	printf ("\nSIGNAL 1\n");
}

void signal_2(int param){
	printf ("\nSIGNAL 2\n");
}

int main (int argc, char* argv[]) { 

	signal(SIGRTMIN,signal_1);
	signal(SIGRTMAX,signal_2);
    pid_t child, child2; 
	
	key_t kulcs;
    int oszt_mem_id;
    char *s;
    kulcs=ftok(argv[0],1);
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);  
	s = shmat(oszt_mem_id,NULL,0);
	
    child = fork();  
	
	
	if ( child > 0 ) 				/*PARENT*/ 
	{
		child2 = fork ();
		if (child2 > 0) 			/*PARENT*/ 
		{ 
			pause();
			printf("PARENT1 : %s olvasott \n",s);
			shmdt(s);
			wait( NULL ); 
			printf("PARENT2\n");
			exit(EXIT_SUCCESS); 
		}else if ( child2 == 0 ){ 	/*CHILD1*/ 
		printf("CHILD1\n");
		kill(getppid(),SIGRTMAX);
		sleep(1);
		exit(EXIT_SUCCESS); 
		}
	printf("PARENT1\n");
	exit(EXIT_SUCCESS); 
    }else if ( child == 0 )			/*CHILD2*/ 
	{
	printf("CHILD2\n");
	
	int random;
	srand ( time(NULL) );
	random = rand() % 100 + 1;
	char uzenet[32];
	sprintf(uzenet,"%d", random);
	printf("Random: %s\n",uzenet);
	strcpy(s,uzenet);
	
	kill(getppid(),SIGRTMIN);
	sleep(1);
	
	shmdt(s);
	
    exit(EXIT_SUCCESS); 
     }
     return 0; 
}