#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>

#define STR_SIZE 256

int elromlott = 0;

typedef struct {
	long mtype;
	char mtext[STR_SIZE];
} msg_t;

void handler(int signo, siginfo_t *info, void *ptr)
{
	elromlott = !info->si_value.sival_int;
	printf("(P) A szerelveny jelzese: %d\n", info->si_value.sival_int);
}

int randreason(char *reason)
{
	int rnd = rand() % 10;
	if (rnd == 0)
		strcpy(reason, "elcsuszott egy bananhejon");
	else if (rnd == 1)
		strcpy(reason, "lefagyott a zord telben");
	else
		strcpy(reason, "sikeresen teljesitette az uzemet");
	
	return rnd;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Helytelen parameterezes!\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	int jaratok = atoi(argv[1]);
	
	key_t key = ftok(argv[0], 1);
	int mq = msgget(key, 0600 | IPC_CREAT);
	if (mq < 0)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	int i, count = 0;
	for (i = 0; i < jaratok; i++)
	{
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
			sprintf(msg.mtext, "%d", i + 1);
			
			if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
				perror("msgsnd #1");
			
			sprintf(msg.mtext, "%d", jaratok);
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
			
			printf("(P) A(z) %d. szerelveny %s\n", i + 1, msg.mtext);
			
			wait(NULL);
			
			if (elromlott)
			{
				count++;
				pid_t pot = fork();
				if (pot == -1)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}
				
				if (pot > 0) // parent
				{
					msg.mtype = 4;
					strcpy(msg.mtext, "ez egy potszerelveny");
					if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
						perror("msgsnd #4");
					
					msg.mtype = 5;
					sprintf(msg.mtext, "%d", i + 1);
					if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
						perror("msgsnd #5");
					
					wait(NULL);
				}
				else // child
				{
					char txt[STR_SIZE];
					int jarat;
					
					msg_t msgrec;
					if (msgrcv(mq, &msgrec, STR_SIZE, 4, 0) < 0)
						perror("msgrcv #4");
					
					strcpy(txt, msgrec.mtext);
					
					if (msgrcv(mq, &msgrec, STR_SIZE, 5, 0) < 0)
						perror("msgrcv #5");
					
					jarat = atoi(msgrec.mtext);
					
					printf("(C) Kozpont mondja: %s, potolt jarat: %d\n", txt, jarat);
					
					char reason[STR_SIZE];
					randreason(reason);
					
					printf("(C) A szerelveny %s\n", reason);
					
					exit(0);
				}
				
				elromlott = 0;
			}
		}
		else // child
		{
			struct timeval time; 
			gettimeofday(&time, NULL);
			srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
			
			msg_t msgrec;
			int szerelveny_szam, szerelveny_max;
			
			if (msgrcv(mq, &msgrec, STR_SIZE, 1, 0) < 0)
				perror("msgrcv #1");
			
			szerelveny_szam = atoi(msgrec.mtext);
			
			if (msgrcv(mq, &msgrec, STR_SIZE, 2, 0) < 0)
				perror("msgrcv #2");
			
			szerelveny_max = atoi(msgrec.mtext);

			char reason[STR_SIZE];
			int rnd = randreason(reason);
			
			printf("(C) %d. szerelveny, van meg %d db, a szerelveny %s.\n", szerelveny_szam, szerelveny_max - szerelveny_szam, reason);
			
			union sigval value;
			value.sival_int = rnd > 1;
			sigqueue(getppid(), SIGUSR1, value);	
			
			msgrec.mtype = 3;
			strcpy(msgrec.mtext, reason);
			if (msgsnd(mq, &msgrec, STR_SIZE, 0) < 0)
				perror("msgsnd #3");

			exit(0);
		}
	}
	
	if (msgctl(mq, IPC_RMID, NULL) < 0)
		perror("msgctl");
	
	printf("A nap folyaman %d mentesito jaratot kellett inditani.\n", count);
	
	return 0;
}