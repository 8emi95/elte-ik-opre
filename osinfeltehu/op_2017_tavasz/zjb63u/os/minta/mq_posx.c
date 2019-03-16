#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <time.h>
// Posix Message queue sample -> -lrt
// mq_open, mq_send,mq_receive, mq_unlink

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
    int uzenetsor, status;
	char* mqname="/justnameit";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	//
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    if ( mq1 < 0 ) { 
        printf("mq_open error: %d \n",errno); 
        return 1; 
    } 
    char buffer[100];	// mq_send, mq_receive uses char array
	printf("Buffer length: %d\n",strlen(buffer));
    child = fork();  
	
	
	if ( child > 0 ) 				/*PARENT*/ 
	{
		child2 = fork ();
		if (child2 > 0) 			/*PARENT*/ 
		{ 
			pause();
			mq_receive(mq1,buffer, 54,NULL);
			printf("Buffer length: %d\n",strlen(buffer));
			printf("PARENT1 : %s olvasott \n",buffer);
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
	kill(getppid(),SIGRTMIN);
	sleep(1);
	
	int random;
	srand ( time(NULL) );
	random = rand() % 100 + 1;
	char uzenet[32];
	sprintf(uzenet,"%d", random);
	printf("Random: %s\n",uzenet);
	int db=mq_send(mq1,uzenet,32, 5 );
	
    exit(EXIT_SUCCESS); 
     }
     return 0; 
}