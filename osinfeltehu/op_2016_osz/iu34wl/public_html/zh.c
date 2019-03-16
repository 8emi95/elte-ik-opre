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

struct Message 
{ 
     long mtype;
     int actNumber;
	 int max;
	 char text [500];
}; 

int mqSend(int mq, struct Message msg)
{ 
     int status;
     status = msgsnd( mq, &msg, sizeof(msg) , 0 );
     if ( status < 0 )
          perror("msgsnd");
     return 0;
} 

int mqReceive(int mq, struct Message* msg) 
{ 
     int status; 
     status = msgrcv(mq, msg, sizeof(*msg), 0, 0 ); 
     if ( status < 0 )
          perror("msgrcv"); 
	 return 0;
} 

void handler(int signumber)
{
	
}

void Pot(int messageQueue)
{
	struct Message msg;
	mqReceive(messageQueue, &msg);
	printf("Pot: %s %d\n", msg.text, msg.actNumber);
	exit(1);
}

void ParentProc(int child1Pid, int messageQueue, int max, int metroNumber)
{
	srand(time(NULL) + 100 * getpid());
	int r = (rand() % max) + 1;
	int szer = r;
	struct Message msg = { 2, r, max };
	mqSend(messageQueue, msg);
	pause();
	mqReceive(messageQueue, &msg);
	printf("Szulo: A szerelveny uzenete: %s\n", msg.text);
	if(msg.actNumber == -1)
	{
		printf("Szulo: Pot szerelveny inditasa\n");
		pid_t child = fork(); 
		if (child < 0)
		{
			perror("fork"); 
		}
		else if(child > 0)
		{
			//ParentProc(child, messageQueue, max, metroNumber, potdb);
			struct Message msg = { 2, szer, max, "Pot szerelveny" };
			//msg.actNumber = r;
			//strcpy(msg.text, "Pot szerelveny");
			mqSend(messageQueue, msg);
			int status;
			waitpid(child1Pid,&status,0);
		}
		else
		{
			Pot(messageQueue);
			exit(1);
		}
	}
	int status;
	waitpid(child1Pid,&status,0);
}

void Child1Proc(int messageQueue)
{
	srand(time(NULL) + 100 * getpid());
	struct Message msg;
	mqReceive(messageQueue, &msg);
	
	int r = (rand() % 100) + 1;
	if(r <= 10)
	{
		printf("Szerelveny: Hanyadik vagyok: %d, Ennyi van meg: %d. Sikertelen voltam!\n", msg.actNumber, msg.max -  msg.actNumber);
		strcpy(msg.text, "Be kellett vontatni!");
		msg.actNumber = -1;
	}
	else
	{
		printf("Szerelveny: Hanyadik vagyok: %d, Ennyi van meg: %d. Sikeres voltam!\n", msg.actNumber, msg.max -  msg.actNumber);
		strcpy(msg.text, "Sikeres voltam!");
		msg.actNumber = 0;
	}
	kill(getppid(), SIGUSR1);
	mqSend(messageQueue, msg);
	exit(1);
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Keves arg!!!!\n");
		exit(1);
	}
	int max = atoi(argv[1]);
	int metroNumber = atoi(argv[2]);
	
	signal(SIGUSR1,handler);
	
	int messageQueue, status; 
    key_t key;
	key = ftok(argv[0],1);
	messageQueue = msgget( key, 0600 | IPC_CREAT ); 
    if ( messageQueue < 0 ) 
	{ 
		perror("msgget"); 
		return 1; 
    } 
	int i = 0;
	int is_parent = 0;
	while(i++ < metroNumber && is_parent == 0)
	{
		printf("i: %d\n", i);
		pid_t child1 = fork(); 
		if (child1 < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(child1 > 0)
		{
			ParentProc(child1, messageQueue, max, metroNumber);
		}
		else
		{
			is_parent = 1;
			Child1Proc(messageQueue);
		}
		sleep(1);
	}
}