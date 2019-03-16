#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>

#define STR_SIZE 256

typedef struct {
	long mtype;
	char mtext[STR_SIZE];
} msg_t;

void handler(int signo, siginfo_t *info, void *ptr)
{
	printf("(P) A szerelveny jelzese: %d\n", info->si_value.sival_int);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Helytelen parameterezes!\n");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	int max_szerelveny = atoi(argv[1]);
	
	key_t key = ftok(argv[0], 1);
	int mq = msgget(key, 0600 | IPC_CREAT);
	if (mq < 0)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	pid_t child = fork();
	if (child == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if (child > 0)	// parent
	{
		msg_t msg;
		msg.mtype = 1;
		int rnd = rand() % max_szerelveny + 1;
		sprintf(msg.mtext, "%d", rnd);
		
		if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
			perror("msgsnd #1");
		
		sprintf(msg.mtext, "%d", max_szerelveny);
		msg.mtype = 2;
		
		if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
			perror("msgsnd #2");

		struct sigaction sigact;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = SA_SIGINFO;
		sigact.sa_sigaction = handler;
		sigaction(SIGUSR1, &sigact, NULL);
		
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset, SIGUSR1);
		sigsuspend(&sigset);
		
		if (msgrcv(mq, &msg, STR_SIZE, 3, 0) < 0)
			perror("msgrcv #3");
		
		printf("(P) A szerelveny %s\n", msg.mtext);
		
		wait(NULL);
		
		if (msgctl(mq, IPC_RMID, NULL) < 0)
			perror("msgctl");
	}
	else // child
	{
		msg_t msgrec;
		int szerelveny_szam, szerelveny_max;
		
		if (msgrcv(mq, &msgrec, STR_SIZE, 1, 0) < 0)
			perror("msgrcv #1");
		
		szerelveny_szam = atoi(msgrec.mtext);
		
		if (msgrcv(mq, &msgrec, STR_SIZE, 2, 0) < 0)
			perror("msgrcv #2");
		
		szerelveny_max = atoi(msgrec.mtext);
		
		int rnd = rand() % 10;
		char reason[STR_SIZE];
		if (rnd == 0)
			strcpy(reason, "elcsuszott egy bananhejon");
		else if (rnd == 1)
			strcpy(reason, "lefagyott a zord telben");
		else
			strcpy(reason, "sikeresen teljesitette az uzemet");
		
		printf("(C) %d. szerelveny, van meg %d db, a szerelveny %s.\n", szerelveny_szam, szerelveny_max - szerelveny_szam, reason);
		
		union sigval value;
		value.sival_int = rnd > 1;
		sigqueue(getppid(), SIGUSR1, value);
		
		msgrec.mtype = 3;
		strcpy(msgrec.mtext, reason);
		if (msgsnd(mq, &msgrec, STR_SIZE, 0) < 0)
			perror("msgsnd #3");
	}
	
	return 0;
}