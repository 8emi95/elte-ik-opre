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

typedef struct
{
unsigned int t;
unsigned int nr;
}mess;

int main (int argc, char* argv[]) {
     pid_t child;
     int uzenetsor, status;
	char* mqname="/baba1";
        mqd_t mq1;
	struct mq_attr attr;
	 attr.mq_flags   = 0;
  attr.mq_maxmsg  = 10;
  attr.mq_msgsize = sizeof(mess);
  attr.mq_curmsgs = 0;
	//
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
     if ( mq1 < 0 ) {
          printf("mq_open error: %d \n",errno);
          return 1;
     }
     mess buffer;	// mq_send, mq_receive uses char array
    //char buffer[100];
	//printf("Buffer length: %d\n",strlen(buffer));
     child = fork();
     if ( child > 0 ) {
          char uzenet[30]="Hajrá Fradi!";
	uzenet[13]=65; uzenet[14]=65; uzenet[15]=65;
	  mess t;
	  t.nr = 12;
	  printf("int : %d:\n",t.nr);

	  int db=mq_send(mq1,(char *) &t,sizeof(mess), 5 );  //Parent sends a message.
	  sleep(1); // ha ez nincs a fogad be is olvassa!!!!
          //mq_send(mq1,"Hajra Debrecen!",20, 4);
	  //mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
	  //printf("Szulo: uzenet elkuldve!%d\n",db);
	  wait( NULL );
          // After terminating child process, the message queue is deleted.
          mq_close(mq1);
	  mq_unlink(mqname);
	  printf("Szulo vege!\n");
     } else  { // child
	//sleep(1);
	//mq2=mq_open(mqname,O_RDWR);
	//strcpy(buffer,"                  ");
	printf("A szam fogadas elott: %d\n",buffer.nr);
        int db=mq_receive(mq1,(char*) &buffer, sizeof(mess),NULL);
	printf("A szam fogadas utan: %d\n",buffer.nr);
	// 5 is the message priority
	//
	//printf("Buffer length: %d\n",strlen(buffer));
	//printf("mq_receive : %s, olvasott bájtok hossza: %d errno: %s\n",buffer, db,strerror(errno));
        //db=mq_receive(mq1,buffer, 50, 0);
	//printf("kovetkező uzenet: %s hossza: %d\n", buffer,db);
	mq_close(mq1);
	//mq_send(mq1,"Gyerek befejezte!", 20,6);
	//msgsnd(uzenetsor,&valasz,sizeof(struct uzenet),0);
	return 0;
          // The child process receives a message.
     }

     return 0;
}
