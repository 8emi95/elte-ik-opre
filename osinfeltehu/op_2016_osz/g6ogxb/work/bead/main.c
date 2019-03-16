#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
mqd_t mq;
char* mqname="/ujpest";
char buffer[100];


void fkevin()
{
	srand(time(NULL));
	int jatekok = rand()%30+20;
	char uzenet[30] = "40"; //hogy bizonyitottan ne azt kuldjuk
	sprintf(uzenet,"%d", jatekok);
	mq_send(mq, uzenet, sizeof(uzenet), 0);
	printf("msg sent: %s \n", uzenet);
	//sleep(1);
	mq_receive(mq, buffer, sizeof(buffer), NULL);
	printf("orosz betoro Mondja: %s!\n", buffer);
	wait(NULL);
	mq_close(mq);
	mq_unlink(mqname);
};
void fbetoro()
{
	int hanyjatek = 0;
	//sleep(1);
	int db = mq_receive(mq, buffer, sizeof(buffer), NULL);
	sscanf(buffer,"%d",&hanyjatek);
	printf("msg rcvd: %s %d\n", buffer, hanyjatek);
	printf("betoro feljajdul\n");
	mq_send(mq, "Nu Zaiec! Nu Pogodi", 50, 0 );
	
	
	mq_close(mq);
	//mq_unlink(mqname);
};

int main (int argc, char* argv[]) { 
	//DECLARE BLOCK
    pid_t betoro, kevin;
	
	
	//MQ letrehozasa
	
	struct mq_attr attr;
	
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=100; //MSGSIZE 
	
	//MQ_SETUP
	mq_unlink(mqname);
	mq=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	if ( mq < 0 ) { 
	  printf("mq_open error: %d \n",errno); 
	  return 1; 
	}
	
	
	
	 //villasreggeli
	 betoro = fork();
	 {
		 if (betoro>0)
		 {
			 kevin = fork();
			 if(kevin>0)
			 {
				//szulo folyamat
				//var amig a gyerek vegig nem ment;
				wait(NULL);
				wait(NULL);

				//gyerekek vege, shm torlese;
				mq_close(mq);
				mq_unlink(mqname);
				//vege a programnak;
				 
			 }
			 else
			 {
				 //kevin folyamat
				 //csinalja a dolgat;
				fkevin();
				
				exit(EXIT_SUCCESS);
			 }
		 }
		 else
		 {
			 fbetoro();
			 
			 exit(EXIT_SUCCESS);
		 }
	 }
	 
	 return 0;
}
