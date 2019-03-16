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

struct Uzenet
{
	long mtype;
	int mnumber;
};

struct childUzenet
{
	long mtype;
	char childUzenet[100];
};

int signalHandler(int signum)
{
	if(signum == SIGUSR1)
	{
		printf("\nBevontattak\n");
	}
	else if(signum == SIGUSR2)
	{
		printf("\nBeertem\n");
	}
}

int mqSend(int mq, struct Uzenet tn)
{ 
     int status;
     status = msgsnd( mq, &tn, sizeof(tn) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct Uzenet* tn) 
{ 
     int status; 
     status = msgrcv(mq, tn, sizeof(*tn), 0, 0 ); 
	 //printf("%ld %d\n",tn->mtype, tn->mnumber);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
}

int sendCause(int mq, struct childUzenet c)
{
	int status;
	status = msgsnd(mq, &c, sizeof(c), 0);
	if ( status < 0 )
          perror("msgsnd");
     return 0;
}

int receiveCause(int mq, struct childUzenet* c)
{
	int status;
	status = msgrcv(mq, c, sizeof(*c),0,0);
	if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
}

void Szulo(int childPid, int messageQueue, int max)
{
	srand((unsigned int)time(NULL));
	int no;
	no = rand() % max + 1;
	struct Uzenet tn = { 2, no };
	mqSend(messageQueue, tn);
	int status;
	waitpid(childPid,&status,0); 
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
    if ( status < 0 ) 
        perror("msgctl"); 
    sleep(1);
    struct childUzenet ok;
	receiveCause(messageQueue, &ok);
	printf("A szerelvenyem emiatt allt le: %s", ok.childUzenet);
}

void Gyerek(int messageQueue1, int messageQueue2, int max)
{
	srand((unsigned int)time(NULL));
	struct Uzenet uz;
	mqReceive(messageQueue1, &uz);
	int maradek  = max - uz.mnumber;
	int success;
	success = 5;//rand() % 100;
	printf("Az en szerelvenyszamom:  %i\n", uz.mnumber);
	printf("Utanam meg %i darab szerelveny van.\n", maradek);
	if(success <= 10)
	{
		printf("%i: Az uzemet nem sikerult teljesitenem\n", success);
		kill(getppid(), SIGUSR1);
		struct childUzenet ok = { 1, "bananhej"};
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
	int maxSzerelveny;
	if(argc >= 2)
	{
		maxSzerelveny = atoi(argv[1]);
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
    
    //key = ftok(argv[0],2);
    messageQueue2 = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue2 < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	
	pid_t child = fork(); 
	
	if(child < 0)
	{
		perror("fork");
		exit(1);
	}
	else if(child > 0) //parent process
	{
		Szulo(child, messageQueue, maxSzerelveny);
	}
	else //child process
	{
		Gyerek(messageQueue, messageQueue2, maxSzerelveny);
	}
}
