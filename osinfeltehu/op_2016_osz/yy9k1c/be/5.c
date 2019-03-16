#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

struct TrainNumber
{
	long mtype;
	int mnumber;
};

struct Cause
{
	long mtype;
	char cause[100];
};

int signalHandler(int signum)
{
	if(signum == SIGUSR1)
	{
		printf("Bevontattak\n");
	}
	else if(signum == SIGUSR2)
	{
		printf("Beertem\n");
	}
}

int mqSend(int mq, struct TrainNumber tn)
{ 
     int status;
     status = msgsnd( mq, &tn, sizeof(tn) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct TrainNumber* tn) 
{ 
     int status; 
     status = msgrcv(mq, tn, sizeof(*tn), 0, 0 ); 
	 //printf("%ld %d\n",tn->mtype, tn->mnumber);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
}

int sendCause(int mq, struct Cause c)
{
	int status;
	status = msgsnd(mq, &c, sizeof(c), 0);
	if ( status < 0 )
          perror("msgsnd");
     return 0;
}

int receiveCause(int mq, struct Cause* c)
{
	int status;
	status = msgrcv(mq, c, sizeof(*c),0,0);
	if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
}

void ParentProc(int childPid, int messageQueue, int max)
{
	srand((unsigned int)time(NULL));
	int no;
	no = rand() % max + 1;
	struct TrainNumber tn = { 2, no };
	mqSend(messageQueue, tn);
	int status;
	waitpid(childPid,&status,0); 
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
    if ( status < 0 ) 
        perror("msgctl"); 
}

void Parent2(int childpid, int messageQueue)
{
	struct Cause ok;
	receiveCause(messageQueue, &ok);
	printf("A szerelvenyem emiatt allt le: %s", ok.cause);
}

void ChildProc(int messageQueue1, int messageQueue2, int max)
{
	srand((unsigned int)time(NULL));
	struct TrainNumber tn;
	mqReceive(messageQueue1, &tn);
	int trainsleft  = max - tn.mnumber;
	int success;
	success = rand() % 100;
	printf("Sorszamom:  %d\n", tn.mnumber);
	printf("Utanam meg %d darab szerelvény fog érkezni.\n", trainsleft);
	if(success <= 10)
	{
		printf("%d: Sajnos nem sikerult teljesitenem az uzemet :(\n", success);
		kill(getppid(), SIGUSR1);
		struct Cause ok = { 1, "Bananhej"};
		sendCause(messageQueue2, ok);
	}
	else
	{
		printf("%d: Sikeresen teljesitettem az uzemet!\n", success);
		kill(getppid(), SIGUSR2);
	}
	sleep(1);
}

int main(int argc, char* argv[])
{
	//signal(SIGUSR1,signalHandler);
	//signal(SIGUSR2,signalHandler);
	int maximum;
	if(argc == 2)
	{
		maximum = atoi(argv[1]);
	}
	
	int messageQueue, messageQueue2, status;
    key_t key;
	
	key = ftok(argv[0],2); 
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    }
    
    key = ftok(argv[0],2);
    messageQueue2 = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue2 < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	
	pid_t child1 = fork(); 
	
	if(child1 < 0)
	{
		perror("fork");
		exit(1);
	}
	else if(child1 > 0) //parent
	{
		ParentProc(child1, messageQueue, maximum);
		Parent2(child1, messageQueue2);
	}
	else //child
	{
		ChildProc(messageQueue, messageQueue2, maximum);
	}
}
