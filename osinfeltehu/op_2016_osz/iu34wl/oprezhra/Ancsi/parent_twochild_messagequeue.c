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
     char mQuestion[1000]; 
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
	 //printf("%ld %s\n",qu->mtype, qu->mQuestion);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void ParentProc(int child1Pid, int child2Pid, int messageQueue)
{
	//parent
	struct Question qu = { 2, "Kerdes?" };
	mqSend(messageQueue, qu);
	mqSend(messageQueue, qu);
	int status;
	waitpid(child1Pid,&status,0); 
	waitpid(child2Pid,&status,0); 
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
}

void Child1Proc(int messageQueue)
{
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf("Child 1: A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	sleep(1);
}

void Child2Proc(int messageQueue)
{
	struct Question qu;
	mqReceive(messageQueue, &qu);
	printf("Child 2: A kapott uzenet kodja: %ld, szovege:  %s\n", qu.mtype, qu.mQuestion);
	sleep(1);
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
	
	pid_t child1 = fork(); 
	
	if (child1 < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child1 > 0)
	{
		pid_t child2 = fork();
		if (child2 < 0)
		{
			perror("fork"); 
		}
		else if(child2 > 0)
		{
			//parent
			ParentProc(child1, child2, messageQueue);
		}
		else
		{
			//child2
			Child2Proc(messageQueue);
		}
		
	}
	else
	{
		//child1
		Child1Proc(messageQueue);
	}
	
}