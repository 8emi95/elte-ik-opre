#include <unistd.h>	//Fork
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit

#include <string.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
	
	int uzenetsor, status;
	char* mqname="/baba1";
	struct mq_attr attr;
	
	mqd_t mq1,mq2;
	//MAXMSGS
	attr.mq_maxmsg=5;
	//MSGSIZE
	attr.mq_msgsize=50; 
	
	//delete if exist
	mq_unlink(mqname); 
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
	if ( mq1 < 0 )
	{ 
		printf("mq_open error: %d \n",errno); 
		return 1; 
	} 
	
	char buffer[100];
	
	int n = 2;
	pid_t childs[n];
	pid_t parent = getpid();
    printf( "[Szülő] pid: %d\n",  parent);
	
	int i;
    for ( i = 0; i < n; i++ )
	{
		pid_t pid = fork();
		if(getpid() == parent) childs[i] = pid;
        if ( pid == 0 )
        {
			if(i == 0)
			{
				printf( "[Kevin] pid: %d szülő: %d\n", getpid(), getppid() );
				srand(time(NULL));
				int r = 20+rand()%50;
				char uzenet[15];
				sprintf(uzenet, "%d", r);
				int db=mq_send(mq1,uzenet,15, 5 ); 
				printf("[kevin]: uzenet elkuldve! %s\n", uzenet);
			}
			if(i == 1)
			{
				printf( "[Betoro] pid: %d szülő: %d\n", getpid(), getppid() );
				db=mq_receive(mq1,buffer, 15, 5);
				printf( "[Betoro] hanadt esek: %s", buffer);
			}
            exit( 0 );
        }
	}
    for ( i = 0; i < n; i++ )
	{
		waitpid( childs[i] );
	}
	mq_close(mq1);
	mq_unlink(mqname);
	printf("[Szülő] leáll\n");
}

