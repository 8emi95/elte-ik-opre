#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>

#define STR_SIZE 256

typedef struct {
	long mtype;
	char mtext[STR_SIZE];
} msg_t;

void initrand()
{
	struct timeval time; 
	gettimeofday(&time, NULL);
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int main(int argc, char **argv)
{
	key_t key = ftok(argv[0], 1);
	int mq = msgget(key, 0600 | IPC_CREAT);
	if (mq < 0)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	pid_t child1 = fork();
	if (child1 < 0)
	{
		perror("fork #1");
		exit(EXIT_FAILURE);
	}

	if (child1 > 0)
	{
		pid_t child2 = fork();
		if (child2 < 0)
		{
			perror("fork #2");
			exit(EXIT_FAILURE);
		}
		
		if (child2 > 0) // parent
		{
			
			wait(NULL);
		}
		else // child2, Kevin
		{
			initrand();
			int rnd = rand() % 31 + 20;
			
			printf("(Kevin) %d apro jatekot szorok szet!\n", rnd);
			
			msg_t msg;
			msg.mtype = 1;
			sprintf(msg.mtext, "%d", rnd);
			if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
				perror("msgsnd #1");
			
			if (msgrcv(mq, &msg, STR_SIZE, 2, 0) < 0)
				perror("msgrcv #2");
			
			printf("(Kevin) Betoro mondja: %s\n", msg.mtext);
			
			exit(0);
		}
		
		wait(NULL);
	}
	else // child1, betörõ
	{
		msg_t msg;
		if (msgrcv(mq, &msg, STR_SIZE, 1, 0) < 0)
			perror("msgrcv #1");
		
		printf("(Betoro) Hanyatt esek %s jatekon!\n", msg.mtext);
		
		printf("(Betoro) Megfenyegetem Kevint!\n");
		msg.mtype = 2;
		strcpy(msg.mtext, "csak varj, amig a kezunk koze nem kerulsz");
		if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
			perror("msgsnd #2");
		
		
		exit(0);
	}
	
	if (msgctl(mq, IPC_RMID, NULL) < 0)
		perror("msgctl");
}