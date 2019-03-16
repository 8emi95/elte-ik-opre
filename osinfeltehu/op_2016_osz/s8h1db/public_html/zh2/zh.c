#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <string.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h> //rand

void handler(int signumber){}

int main()
{ 
	srand(time(NULL));
	
	signal(SIGTERM,handler);
	pid_t pid;
	pid_t pid2;
	
	char* mqname="/s8h1db";
	char* mqname2="/s8h1db-2";
    struct mq_attr attr;
    mqd_t mq1;
    attr.mq_maxmsg=10; //MAXMSGS
    attr.mq_msgsize=sizeof(int); //MSGSIZE
    mq_unlink(mqname); // delete if exist
    mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	struct mq_attr attr2;
    mqd_t mq2;
    attr2.mq_maxmsg=10; //MAXMSGS
    attr2.mq_msgsize=50; //MSGSIZE
    mq_unlink(mqname2); // delete if exist
    mq2=mq_open(mqname2, O_CREAT|O_RDWR,0600,&attr2);
	char buffer[50];
	if ( mq1 < 0 )
	{ 
        printf("badmqueue: %s",strerror(errno)); 
    }
	if ( mq2 < 0 )
	{ 
        printf("badmqueue: %s",strerror(errno)); 
    }
	pid = fork();
    if (pid < 0 )
    {
        printf("badfork-kevin\n");
    }
    else if (pid == 0) //child - Kevin
    {
		int jatek = rand() % 31 + 20;
		mq_send(mq1, (char*)&jatek, sizeof(int), 1);
		mq_receive(mq2, buffer, 50, 0);
		printf("%s\n", buffer);
        exit(0);
    }
    else //parent
    {
		pid2 = fork();
		if(pid2 < 0) {
			printf("badfork-betoro\n");
		}
		else if (pid2 == 0) { // child - betoro
			int szam;
			mq_receive(mq1, (char*)&szam, sizeof(int), 0);
			printf("%i\n", szam);
			mq_send(mq2, "csak varj, mig a kezunk koze nem kerulsz", 50, 1);
			exit(0);
		}
		else {
			wait(NULL);
			wait(NULL);
			mq_close(mq1);
			mq_close(mq2);
			mq_unlink(mqname);
			mq_unlink(mqname2);
		}
    }
	return 0;
}