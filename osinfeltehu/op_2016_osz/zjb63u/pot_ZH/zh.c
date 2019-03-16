#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

int main (void) 
{ 
	pid_t child, child2; 
	char* mqname="/asd";
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
	 
	char buffer[100];	// mq_send, mq_receive uses char array
	

  /* child process letrehozasa */ 
  child = fork ();
  if (child == 0) 
  { 
    /*CHILD*/ /*KEVIN*/
	printf("KEVIN\n");
	int jatek;
	srand ( time(NULL) );
	jatek = rand() % 30 + 20;
	//printf("Jatek: %i\n",jatek);
	char uzenet[32];
	sprintf(uzenet,"%d", jatek);
	printf("Jatek: %s\n",uzenet);
	int db=mq_send(mq1,uzenet,32, 5 );
	
	sleep(3);
	
	mq_receive(mq1,buffer, 54,NULL);
	printf("Buffer length: %d\n",strlen(buffer));
	printf("Betoro : %s olvasott \n",buffer);
	wait( NULL ); 
	mq_close(mq1);
	mq_unlink(mqname);

    //exit(EXIT_SUCCESS); 
   } 
  else 
  { 
    /*PARENT*/ 
	printf("PARENT1\n");
	
	child2 = fork ();
	if (child2 == 0) 
	{ 
		/*CHILD*/ /*BETÖRŐ*/
		sleep(3);
		printf("BETÖRŐ\n");
	
        int db=mq_receive(mq1,buffer, 54,NULL);

	printf("Buffer length: %d\n",strlen(buffer));
	printf("Betoro : %s olvasott \n",buffer);
	
	char uzenet[100] = "csak várj, amíg a kezünk közé nem kerülsz";
		mq_send(mq1,uzenet,100, 5 );
	mq_close(mq1);

	return 0; 

		//exit(EXIT_SUCCESS); 
	}else{ 
		/*PARENT*/ 
		printf("PARENT2\n");
		wait();
		//exit(EXIT_SUCCESS); 
	}
	wait();
	mq_unlink(mqname);
    //exit(EXIT_SUCCESS); 
  } 
} 
