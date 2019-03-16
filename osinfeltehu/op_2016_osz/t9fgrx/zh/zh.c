#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>


void signal_handler(int sing_number) {
	//printf("Beertem\n", sing_number);
}



int main(int argc, char* argv[]){
	
	int uzenetsor, status;
	char* mqname="/t9fgrx";
	struct mq_attr attr;
	mqd_t mq1;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE 
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
    } 
    char buffer[100];
	
	time_t t;
	srand(time(&t));

	signal(SIGTERM, signal_handler);
	
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		perror("The fork calling was not succesful\n"); 
		exit(1);
	}

	if(pid==0)
	{
		char uzenet[50];
		int mennyivanmeg, hanyadik, ossz;
		int db=mq_receive(mq1,buffer, sizeof(buffer),NULL);
		hanyadik=atoi(buffer);
		printf("Szerelveny sorszama: %d\n", hanyadik);
		db=mq_receive(mq1,buffer, sizeof(buffer),NULL);
		ossz=atoi(buffer);
		printf("Osszes szerelveny szama: %d\n", ossz);
		mennyivanmeg=ossz-hanyadik;
		printf("Ennyi szerelveny van meg utanam: %d\n",mennyivanmeg);
		int esely=(random()%100)+1;
		if(esely<=10)
		{
			strcpy(uzenet,"Bevontattak!");
			kill(getppid(),SIGTERM);
			db=mq_send(mq1,uzenet,sizeof(uzenet), 5 );
		}else
		{
				strcpy(uzenet,"Beertem!");
				kill(getppid(),SIGTERM);
				db=mq_send(mq1,uzenet,sizeof(uzenet), 5 );
		}
		
		
	}else
	{
		int random=(rand()%atoi(argv[1]))+1;
		char ran[2];
        sprintf(ran,"%d",random);
		int db=mq_send(mq1,ran,sizeof(ran), 5 );
		db=mq_send(mq1,argv[1],sizeof(argv[1]), 5 );
		printf("Szamok elkuldve!\n");
		pause();
		printf("Megkaptam a jelzest!\n",SIGTERM);
		wait(NULL);
		int rec=mq_receive(mq1,buffer, sizeof(buffer),NULL);
		printf("Megkaptam hogy mi lett veled: %s\n",buffer);
		mq_unlink(mqname);
		
	}
	
return 0;
}
