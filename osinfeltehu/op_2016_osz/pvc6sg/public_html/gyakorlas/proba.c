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
// -lrt -vel fordit!

int main (int argc, char* argv[]) { 
     pid_t child; 
     int uzenetsor, status;
	char* mqname="/baba1";
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
     if ( child > 0 ) { 
          char uzenet[30]="Hello!";
	  int db=mq_send(mq1,uzenet,17, 4 );  //Parent sends a message. 
	  printf("Szulo: uzenet elkuldve!%d\n",db);
	  wait(NULL);
	  db = mq_receive(mq1,buffer, 54,NULL);
	  printf("mq_receive : %s, olvasott bajtok hossza: %d\n",buffer, db);
          mq_close(mq1);
	  mq_unlink(mqname);
	  printf("Szulo vege!\n"); 
     } else if (child == 0)  { // child
	sleep(1); 
        int db=mq_receive(mq1,buffer, 54,NULL);
	//printf("Buffer length: %d\n",strlen(buffer));
printf("dunno");
	printf("mq_receive : %s, olvasott bajtok hossza: %d\n",buffer, db);
        db=mq_send(mq1,"Szia!",17, 4 );
	mq_close(mq1);
	return 0; 
          // The child process receives a message. 
     }
     
     return 0; 
}