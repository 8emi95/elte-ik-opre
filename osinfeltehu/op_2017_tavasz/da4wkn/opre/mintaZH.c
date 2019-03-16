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

struct mymsg { 
     long mtype;
     char mytext[1000]; 
}; 

int mqSend(int mq, struct mymsg msg)
{ 
     int status;
     status = msgsnd( mq, &msg, sizeof(msg) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct mymsg* msg) 
{ 
     int status; 
     status = msgrcv(mq, msg, sizeof(*msg), 0, 0 ); 
	 //printf("%ld %s\n",msg->mtype, msg->mytext);
     if ( status < 0 ) 
          perror("msgrcv"); 
	 return 0;
} 

void ParentProc(int child1Pid, int child2Pid, int messageQueue, int* child1Pipe, int* child2Pipe)
{
	//parent
	
	//messagequeue:
	struct mymsg msg = { 2, "Kerdes?" };
	mqSend(messageQueue, msg);
	mqSend(messageQueue, msg);
	//pipe:
	close(child1Pipe[1]);
	close(child2Pipe[1]);
	int answer1, answer2;
	read(child1Pipe[0], &answer1, sizeof(answer1));
	read(child2Pipe[0], &answer2, sizeof(answer2));
	printf("Valaszok: %d, %d\n", answer1, answer2);
	//waiting for the childs to terminate:
	int status;
	waitpid(child1Pid,&status,0); 
	waitpid(child2Pid,&status,0); 
	//delete mq:
	status = msgctl( messageQueue, IPC_RMID, NULL ); 
	if ( status < 0 ) 
		perror("msgctl"); 
}

void Child1Proc(int messageQueue, int* child1Pipe)
{
	//seed
	srand(time(NULL) + 100 * getpid());
	//messagequeue
	struct mymsg msg;
	mqReceive(messageQueue, &msg);
	printf("Child 1: A kapott uzenet kodja: %ld, szovege:  %s\n", msg.mtype, msg.mytext);
	//pipe
	close(child1Pipe[0]);
	int r = (rand() % 4) + 1;
	write(child1Pipe[1], &r, sizeof(r));
	close(child1Pipe[1]);
	
	sleep(1);
}

void Child2Proc(int messageQueue, int* child2Pipe)
{
	//seed
	srand(time(NULL) + 100 * getpid());
	//messagequeue
	struct mymsg msg;
	mqReceive(messageQueue, &msg);
	printf("Child 2: A kapott uzenet kodja: %ld, szovege:  %s\n", msg.mtype, msg.mytext);
	//pipe
	close(child2Pipe[0]);
	int r = (rand() % 4) + 1;
	write(child2Pipe[1], &r, sizeof(r));
	close(child2Pipe[1]);
	
	sleep(1);
}

int main(int argc, char* argv[])
{
    int messageQueue, status; 
    key_t key;
	//get key
	key = ftok(argv[0],2); 
	//create mq
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	//create pipe
	int child1Pipe[2];
	if (pipe(child1Pipe) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid_t child1 = fork(); 
	
	if (child1 < 0)
	{
		perror("fork"); 
        return 1; 
	}
	else if(child1 > 0)
	{
		//second pipe
		int child2Pipe[2];
		if (pipe(child2Pipe) == -1) 
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
		pid_t child2 = fork();
		if (child2 < 0)
		{
			perror("fork"); 
		}
		else if(child2 > 0)
		{
			//parent
			ParentProc(child1, child2, messageQueue, child1Pipe, child2Pipe);
		}
		else
		{
			//child2
			Child2Proc(messageQueue, child2Pipe);
		}
		
	}
	else
	{
		//child1
		Child1Proc(messageQueue, child1Pipe);
	}
	
}
