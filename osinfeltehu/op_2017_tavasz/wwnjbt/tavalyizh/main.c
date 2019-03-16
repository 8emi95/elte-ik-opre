// linekeléshez: -lrt

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
#define MSG_LENGTH 500

int pipefd[2];
char* mqname = "/msgq";
struct mq_attr attr;
mqd_t mq;

void handler(int signumber) { }

int main (int argc, char* argv[])
{
	srand(time(NULL));
	signal(SIGUSR1, handler);
    pid_t parentID = getpid();
	pid_t pid, pids[STUDENTS];
    int status;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MSG_LENGTH;

    mq_unlink(mqname);
    mq = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    if (mq < 0)
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
		// kill(pids[0], SIGUSR1);
		// kill(pids[1], SIGUSR1);
		
		// Receiving the answers
		for (i = 0; i < STUDENTS; ++i)
		{
			pause();
			int db = mq_receive(mq, buffer, 1, 0);
			printf("P: Message received: %s\n", buffer);
			kill(pids[i], SIGUSR1);
		}
		
		for(i = 0; i < STUDENTS; ++i)
		{
			int st;
			wait(&st);
		}
	}
	else
	{
		printf("Hahahaa");
		// pause();
		char buffer[MSG_LENGTH];
		int db = mq_receive(mq, buffer, MSG_LENGTH, 0);
		
		int myPid = getpid();
		
		printf("CH%i: Question received: %s\n", myPid, buffer);
		usleep(3000);
		
		int randnum = rand() % 4 + 1;
		sprintf(buffer, "%i", randnum);
		db = mq_send(mq, buffer, strlen(buffer), 4);
		printf("CH%i: Message sent: %s\n", myPid, buffer);
		kill(parentID, SIGUSR1);
		
	}
	
	mq_close(mq);
	mq_unlink(mqname);
    return 0;
}