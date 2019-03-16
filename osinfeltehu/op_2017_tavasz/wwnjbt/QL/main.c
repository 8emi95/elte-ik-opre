// linekeléshez: -lrt
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#define STUDENTS 2
#define MSG_LENGTH 50

int pipefd[2];

char* mqname = "/msgqq";
char* childmqname = "/childmsgqq";
struct mq_attr attr;
mqd_t mq, childmq;

void handler(int signumber) { }

int main (int argc, char* argv[])
{
	srand(time(NULL));
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
    pid_t parentID = getpid();
	pid_t pid, pids[STUDENTS];
    int status;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MSG_LENGTH;

    mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    childmq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
	if (mq < 0 || childmq < 0)
    {
        printf("Error opening message queue: %d\n", errno);
        return 1;
    }
	
	int i;
	for(i = 0; i < STUDENTS; ++i)
	{
		if(parentID == getpid())
		{
			pid = fork();
			if(pid < 0)
			{
				printf("fork fail (%d)", i);
				return 1;
			}
			pids[i] = pid;
		}
	}
	
	if (pid > 0) // Prof
	{
		int i;
		char buffer[MSG_LENGTH] = "Are you all here?";
		if (mq_send(mq, buffer, strlen(buffer), 5))
		{
			printf("Error sending message: %d\n", errno);
			return 1;
		}
		printf("P: Question sent: %s\n", buffer);
		kill(pids[0], SIGUSR1);
		pause();
		int result;
		read(pipefd[0], &result, sizeof(result));
		printf("P: Message received: %i\n", result);
		if (mq_send(mq, buffer, strlen(buffer), 5))
		{
			printf("Error sending message: %d\n", errno);
			return 1;
		}
		kill(pids[1], SIGUSR1);
		pause();
		read(pipefd[0], &result, sizeof(result));
		printf("P: Message received: %i\n", result);
		
		
		// Receiving the answers
		/*
		for (i = 0; i < STUDENTS; ++i)
		{
			pause();
			int db = mq_receive(mq, buffer, 1, 0);
			printf("P: Message received: %s\n", buffer);
			kill(pids[i], SIGUSR1);
		}
		*/
		
		for(i = 0; i < STUDENTS; ++i)
		{
			int st;
			wait(&st);
		}
	}
	else
	{
		int st;
		wait(&st);
		printf("started\n");
		char buffer[MSG_LENGTH];
		mq_receive(mq, buffer, MSG_LENGTH, 0);
		
		int myPid = getpid();
		
		printf("CH%i: Question received: %s\n", myPid, buffer);
		
		int randnum = rand() % 4 + 1;
		write(pipefd[1], &randnum, sizeof(randnum));
		printf("CH%i: Message sent: %i\n", randnum)	;
		kill(parentID, SIGUSR1);
		if (getpid() != pids[1])
			kill(pids[1], SIGUSR1);
	}
	
	mq_close(mq);
	mq_unlink(mqname);
    return 0;
}