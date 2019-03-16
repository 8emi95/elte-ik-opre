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
	char* mqname="/baba1";
	struct mq_attr attr;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //limit: /proc/sys/fs/mqueue/msg_max=10
	attr.mq_msgsize=50; //limmit: /proc/sys/fs/mqueue/msgsize_max=8192 
			// queue limit: /proc/sys/fs/mqueue/queues_max=256
	mq_unlink(mqname); // delete if exist
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	//mq1=mq_open(IPC_PRIVATE, O_CREAT|O_RDWR,0600,&attr);
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
     char buffer[100];	// mq_send, mq_receive uses char array
	printf("Buffer length: %d\n",strlen(buffer));
     child = fork(); 
     if ( child > 0 ) { 
          char uzenet[30]="Hajrá Fradi!";
	uzenet[13]=65; uzenet[14]=65; uzenet[15]=65;
	  int db=mq_send(mq1,uzenet,17, 5 );  //Parent sends a message. 
		// 5 is the priority of msg
		// if the mq is noit empty, the actual msg is inserted 
		// into mq at the right place!!!
	  //sleep(1); // ha ez nincs a fogad be is olvassa!!!!
	  int adatok[]={4,5,6,7};
          //mq_send(mq1,"Hajra Debrecen!",20, 6);
          mq_send(mq1,(char*)adatok,sizeof(adatok), 6);
	  //mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
	  printf("Szulo: uzenet elkuldve!%d, adatok mérete: %d\n",db,sizeof(adatok));
	  wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          mq_close(mq1);
	  mq_unlink(mqname);
	  printf("Szulo vege!\n"); 
     } else  { // child
	//sleep(1); 
	//mq2=mq_open(mqname,O_RDWR);
	//strcpy(buffer,"                  ");
	int *prio;
	int adat[5];
	printf("Adat tömb 5 elemű, mérete: %d\n",sizeof(adat));
        int db=mq_receive(mq1,(char*)adat, 54,NULL); // receive the first msg
	// 5 is the message priority
	//
	printf("Buffer length: %d\n",strlen(buffer));
	printf("mq_receive : %d, olvasott bájtok hossza: %d errno: %s\n",adat[3], db,strerror(errno));
        db=mq_receive(mq1,buffer, 50, prio);
	printf("A kovetkező uzenet: %s hossza: %d prioritása: %d\n", buffer,db,*prio);
	mq_close(mq1);
	//mq_send(mq1,"Gyerek befejezte!", 20,6);
	//msgsnd(uzenetsor,&valasz,sizeof(struct uzenet),0);
	return 0; 
          // The child process receives a message. 
     }
     
     return 0; 
} 
