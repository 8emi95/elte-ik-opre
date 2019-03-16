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

struct Question { 
     long mtype;
     char mQuestion[100]; 
}; 

int mqSend(int mq, struct Question qu)
{ 
     int status;
     status = msgsnd( mq, &qu, sizeof(qu) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct Question* qu) 
{ 
     int status; 
     status = msgrcv(mq, qu, sizeof(*qu), 0, 0 ); 
	 printf("%ld %s\n",qu->mtype, qu->mQuestion);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void ParentProc(int childPid, int childtwoPid, int messageQueue)
{
	struct Question qu = { 2, "Kerdes?" };
	mqSend(messageQueue, qu);
	int status;
	waitpid(childPid,&status,0);
	
	mqSend(messageQueue, qu);
	waitpid(childtwoPid,&status,0); 
	
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
    if ( status < 0 ) 
        perror("msgctl"); 
	
	
}

void ChildProc(int messageQueue)
{
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf(" 1 A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	sleep(0.5);
}

void ChildTwoProc(int messageQueue)
{
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf("2 A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	sleep(0.5);
}

int main(int argc, char* argv[])
{
    int messageQueue, status; 
    key_t key;
	
	key = ftok(argv[0],2); 
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	
	pid_t child = fork(); 
	
	if (child < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child > 0)
	{
		pid_t childtwo = fork();
		if (childtwo < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(childtwo > 0)
		{	
			ParentProc(child, childtwo, messageQueue);
		}
		else
		{
			ChildTwoProc(messageQueue);		
		}
	}
	else
	{
		ChildProc(messageQueue);
	}
	
}