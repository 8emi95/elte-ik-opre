
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
    pid_t child; 
    int uzenetsor, status;
	char* mqname="/ba1";
	struct mq_attr attr;
	mqd_t mq1;
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
    child = fork(); 
    if ( child > 0 ) { 

	// RANDOLÁS	RANDOLÁS	RANDOLÁS	RANDOLÁS	RANDOLÁS
	srand(time(NULL));
      char uzenet[30];
	  strcpy(uzenet,argv[1]);
	  int db=mq_send(mq1,uzenet,sizeof(uzenet), 5 );  //Parent sends a message.
	  int asd=uzenet[0]-'0';
	  int mesg=(rand()%asd)+1;
	  char uzi[30];
	  uzi[0]=mesg+'0';
      mq_send(mq1,uzi,sizeof(uzi), 4);
	  printf("Szulo: uzenet elkuldve!%d\n",db);
	  wait( NULL ); 
      mq_close(mq1);
	  mq_unlink(mqname);
	  printf("Szulo vege!\n"); 
     } else  { // child
		int db=mq_receive(mq1,buffer, 54,NULL);
		printf("gyerek vagyok ez a max: %s\n",buffer);
		db=mq_receive(mq1,buffer, 50, 0);
		char a=buffer[0];
		printf("gyerek vagyok ez vagyok én: %s\n",a);
		mq_close(mq1);
		return 0; 
		// The child process receives a message. 
     }
     
     return 0; 
} 