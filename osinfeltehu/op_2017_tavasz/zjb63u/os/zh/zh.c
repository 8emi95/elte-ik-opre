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

#define MAX_SIZE 512
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

	if(argc < 2){
		printf("Nem adta meg a lakcimet!\n");
		exit(1);
	}
	
	system("clear");
	
	signal(SIGRTMIN,signal_1);
	signal(SIGRTMAX,signal_2);
    pid_t child, child2; 
	
	/*pipe létrehozása*/ 
	int mypipe[2]; 
	char readbuffer[MAX_SIZE];
	pipe (mypipe);
	/*pipe létrehozása*/ 
	
	/*üzenetsor létrehozása*/ 
	int uzenetsor, status;
	char* mqname="/justnameit";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    if ( mq1 < 0 ) { 
        printf("mq_open error: %d \n",errno); 
        return 1; 
    } 
    char buffer[100];	// mq_send, mq_receive uses char array
	/*üzenetsor létrehozása*/ 

    child = fork();  
	
	
	if ( child > 0 ) 				
	{
		child2 = fork ();
		if (child2 > 0) 			/*PARENT*/ //központ
		{ 
			//pause();
			sleep(1);
			read(mypipe[0],readbuffer,30);
			printf("kozpont ' %s ' olvasott \n",readbuffer);
			sleep(1);
			char uzenet[50];
			//sprintf(uzenet,"%s", readbuffer);
			strcat(uzenet, readbuffer);
			//sprintf(uzenet,"%s", ", ");
			strcat(uzenet, ", ");
			char temp[10];
			sprintf(temp,"%i", child2);
			strcat(uzenet, temp);
			mq_send(mq1,uzenet,50, 5 );
			sleep(4);
			mq_receive(mq1,buffer, 50,NULL);
			printf("kozpont ' %s ' olvasott \n",buffer);
			wait(); 
			printf("--kozpont vege--\n");
			exit(EXIT_SUCCESS); 
		}else if ( child2 == 0 ){ 	/*CHILD1*/ //utas - pipe
		//kill(getppid(),SIGRTMAX);
		char uzenet[30];
		int i;
		for(i=1; i<argc; i++){
			strcat(uzenet, argv[i]);
			strcat(uzenet, " ");
		}
		write(mypipe[1], uzenet ,30); 
		sleep(1);
		printf("--utas vege--\n");
		exit(EXIT_SUCCESS); 
		}
	exit(EXIT_SUCCESS); 
    }else if ( child == 0 )			/*CHILD2*/ //kisz. auto - üzenets
	{
	sleep(3);
	mq_receive(mq1,buffer, 50,NULL);
	printf("taxis ' %s ' olvasott \n",buffer);
	char uzenet[32];
	//sprintf(uzenet,"%s", "Elindultam az utasert");
	strcat(uzenet, "Elindultam az utasert");
	mq_send(mq1,uzenet,50, 5 );
	
	//kill(getppid(),SIGRTMIN);
	sleep(1);
	printf("--taxis vege--\n");
	
	
    exit(EXIT_SUCCESS); 
     }
     return 0; 
}