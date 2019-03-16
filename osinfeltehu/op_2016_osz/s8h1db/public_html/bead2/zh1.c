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
	int szszam;
	printf("Hany szerelveny van?\n");
	scanf("%d",&szszam);
	
	srand(time(NULL));
	
	signal(SIGTERM,handler);
	
	char* mqname="/pyltuy";
    int pipeid = mkfifo("PYLTUY_pipe",0600);
    pipeid = open("PYLTUY_pipe",O_RDWR);
    struct mq_attr attr;
    mqd_t mq1;
    attr.mq_maxmsg=10; //MAXMSGS
    attr.mq_msgsize=sizeof(int); //MSGSIZE
    mq_unlink(mqname); // delete if exist
    mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	
	if ( mq1 < 0 )
	{ 
        printf("badmqueue: %s",strerror(errno)); 
    }
	if (pipeid < 0)
    {
        printf("badpipe\n");
    }
	
	pid_t pid = fork();
    if (pid < 0 )
    {
        printf("badfork\n");
    }
    else if (pid == 0) //child
    {
		int id, max;
		char ok[100];
		mq_receive(mq1, (char*)&id, sizeof(int), 0);
		mq_receive(mq1, (char*)&max, sizeof(int), 0);	
		bool broken = (rand()%10 + 1) == 1;
		printf("En vagyok a(z) %i. szerelveny!\nUtanam meg lesz %i db szerelveny!\n",id,max - 1);
		max--;
		
		if(broken)
		{
			strcpy(ok,"Bananhej! Potlobusz kozlekedik!\n");
		}
		else
		{
			strcpy(ok, "Sikeres celbaeres!\n");
		}        
		kill(getppid(), SIGTERM);
		write(pipeid, ok, sizeof(ok));
        while(1)
        {
            sleep(1);
        }
        exit(1);
    }
    else //parent
    {					
		int mID = (rand()%szszam) + 1;
		char ok[100];
		mq_send(mq1, (char*)&mID, sizeof(int), 1);
		mq_send(mq1, (char*)&szszam, sizeof(int), 1);
		pause();
		read(pipeid, ok, sizeof(ok));
		printf("%s", ok);
		printf("Nincs tobb szerelveny!\n");
		unlink("PYLTUY_pipe");
        mq_close(mq1);
        mq_unlink(mqname);
    }
	return 0;
}