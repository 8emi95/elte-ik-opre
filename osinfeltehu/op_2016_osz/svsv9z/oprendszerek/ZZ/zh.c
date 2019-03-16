#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h>

#include<sys/sem.h>
#include<semaphore.h>

#include <sys/wait.h> //waitpids  
#include <string.h>
#include <errno.h>

#include <sys/ipc.h> 
#include <sys/msg.h> 

#include <mqueue.h>

void sikerSignal(int signum){
	printf("SIKER!\n");
}

void hibaSignal(int signum){
	printf("MEGHIBASODAS\n");
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	signal(SIGUSR1, sikerSignal);
	signal(SIGUSR2, hibaSignal);
	int parentpid = getpid();
	


  


	int uzenetsor, status;
	char* mqname="/vasut";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5;
	attr.mq_msgsize=50;
	char buffer[100];
	mq_unlink(mqname); 
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);


 int maxSzam = atoi(argv[1]);		

  if ( mq1 < 0 ) { 
     printf("mq_open error: %d \n",errno); 
     return 1; 
   } 

  int pid;
  pid = fork();
  if (pid > 0){
			int currNum = (rand() % (maxSzam)) + 1;
			char uzenet[30];
			sprintf(uzenet, "%d", currNum);
			mq_send(mq1, uzenet, 30, 5);
			wait( NULL ); 
			mq_receive(mq1, buffer, 100, NULL);
			mq_close(mq1);
			mq_unlink(mqname);
    
  }
  else if(pid == 0){


		mq_receive(mq1, buffer, 100, NULL);
		printf("Szerelvenyszam: %s. \n Hatra van meg: %d\n", buffer, maxSzam - atoi(buffer));
		int rate = rand() % 100;
			if (rate < 50) {
				mq_send(mq1,"PROBLEMA", 30, 5);
				kill(getppid(), SIGUSR2);
			}
			else {
				mq_send(mq1,"SIKER", 30, 5);
				kill(getppid(), SIGUSR1);
			}
		//	kill(getppid(), SIGUSR1);
}
return 0;
}