#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <signal.h>

#define STR_SIZE 256

typedef struct {
	long mtype;
	char mtext[STR_SIZE];
} msg_t;

void handler(int signo)
{
	printf("(Utas) Taxis jelzest kuldott!\n");
}

void handler2(int signumber, siginfo_t *info, void *ptr)
{
	printf(
		"(Taxis) Utas kuldte a tavolsagot, az ar ez alapjan: %d\n",
		info->si_value.sival_int * 300 + 700
	);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Helytelen parameterezes!\n");
		exit(EXIT_FAILURE);
	}
	
	int pipefd[2];
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	key_t key = ftok(argv[0], 1);
	int mq = msgget(key, 0600 | IPC_CREAT);
	if (mq < 0)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	sigset_t sigset;
	sigfillset(&sigset);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	pid_t child = fork(); 
	if (child < 0)
	{
		perror("fork #1");
		exit(EXIT_FAILURE);
	}
	
	if (child > 0) // parent
	{
		close(pipefd[1]);
		
		char buf[STR_SIZE];
		read(pipefd[0], &buf, STR_SIZE);
		
		printf("(Kozpont) Utas lakcime: %s\n", buf);
		
		pid_t taxis = fork();
		if (taxis < 0)
		{
			perror("fork #2");
			exit(EXIT_FAILURE);
		}
		
		if (taxis > 0) // parent
		{
			printf("(Kozpont) Utas adatainak kuldese a taxisnak...\n");
			
			msg_t msg;
			msg.mtype = 1;
			strcpy(msg.mtext, argv[1]);
			if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
				perror("msgsnd #1");
			
			msg.mtype = 2;
			sprintf(msg.mtext, "%d", getpid());
			if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
				perror("msgsnd #2");
			
			if (msgrcv(mq, &msg, STR_SIZE, 3, 0) < 0)
				perror("msgrcv #3");
			
			printf("(Kozpont) Taxis mondja: %s\n", msg.mtext);
			
			wait(NULL);
		}
		else // child, taxis
		{
			char addr[STR_SIZE];
			int phone;
			
			msg_t msg;
			if (msgrcv(mq, &msg, STR_SIZE, 1, 0) < 0)
				perror("msgrcv #1");
			
			strcpy(addr, msg.mtext);
			
			if (msgrcv(mq, &msg, STR_SIZE, 2, 0) < 0)
				perror("msgrcv #2");
			
			phone = atoi(msg.mtext);
			
			printf("(Taxis) Utas cime: %s, telefonszama: %d\n", addr, phone);
			
			strcpy(msg.mtext, "elindultam az utasert");
			msg.mtype = 3;
			if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
				perror("msgsnd #3");
			
			sleep(2);
			
			printf("(Taxis) Megerkeztem, utas hivasa...\n");
			kill(child, SIGUSR1);
			
			struct sigaction sigact;
			sigemptyset(&sigact.sa_mask);
			sigact.sa_flags = SA_SIGINFO;
			sigact.sa_sigaction = handler2;
			sigaction(SIGUSR1, &sigact, NULL);
			
			sigdelset(&sigset, SIGUSR1);
			sigsuspend(&sigset);
			
			exit(EXIT_SUCCESS);
		}

		wait(NULL);
		close(pipefd[0]);
	}
	else // child, utas
	{
		close(pipefd[0]);

		char addr[STR_SIZE];
		strcpy(addr, argv[1]);
		
		printf("(Utas) Lakcim kuldese a kozpontnak...\n");
		write(pipefd[1], &addr, STR_SIZE);
		
		struct sigaction sigact;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = 0;
		sigact.sa_handler = handler;
		sigaction(SIGUSR1, &sigact, NULL);
		
		sigdelset(&sigset, SIGUSR1);
		sigsuspend(&sigset);
		
		printf("(Utas) Tavolsag kuldese a taxisnak...\n");
		union sigval value;
        value.sival_int = rnd() % 20 + 1;
		sigqueue(taxis, SIGUSR1, value);

		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	
	if (msgctl(mq, IPC_RMID, NULL) < 0)
		perror("msgctl");
}