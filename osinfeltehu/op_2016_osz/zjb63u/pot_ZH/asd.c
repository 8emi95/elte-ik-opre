#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink

int main (int argc, char* argv[]) { 
     pid_t child, child2; 
     int uzenetsor, status;
	char* mqname="/asdasd";
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
     if ( child == 0 ) { 
	 int jatek;
	srand ( time(NULL) );
	jatek = rand() % 30 + 20;
	//printf("Jatek: %i\n",jatek);
	char uzenet[32];
	sprintf(uzenet,"%d", jatek);
	printf("Jatek: %s\n",uzenet);
	  int db=mq_send(mq1,uzenet,17, 5 );  //Parent sends a message. 
	  //sleep(1); // ha ez nincs a fogad be is olvassa!!!!
          mq_send(mq1,"Hajra Debrecen!",20, 4);
	  //mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
	  printf("Szulo: uzenet elkuldve!%d\n",db);
	  wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          mq_close(mq1);
	  mq_unlink(mqname);
	  printf("Szulo vege!\n"); 
     } else  { // child
		/*PARENT*/ 
	printf("PARENT1\n");
	
	child2 = fork ();
	if (child2 == 0) 
	{ 
		/*CHILD*/ /*BETÖRŐ*/
		sleep(3);
		printf("BETÖRŐ\n");
		int db=mq_receive(mq1,buffer, 32,0);
		printf("Buffer length: %d\n",strlen(buffer));
		printf("Jatek: %s\n",buffer);

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
     
     return 0; 
} 
