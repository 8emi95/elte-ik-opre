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

void initrand()
{
	struct timeval time; 
	gettimeofday(&time, NULL);
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void handler2(int signumber, siginfo_t *info, void *nonused)
{
	int ar = info->si_value.sival_int * 300 + 700;
	printf("(Taxis) Utas kuldte a tavolsagot (%d), az ar ez alapjan: %d\n", info->si_value.sival_int, ar);
}

int main(int argc, char **argv)
{
	sigset_t sigset;
	sigfillset(&sigset);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	pid_t child = fork();
	
	if (child > 0) // parent
	{
		initrand();
		
		union sigval val;
		val.sival_int = random() % 20 + 1;
		sigqueue(child, SIGUSR1, val);
		
		wait(NULL);
	}
	else // child
	{
		struct sigaction sigact;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = SA_SIGINFO;
		sigact.sa_sigaction = handler2;
		sigaction(SIGUSR1, &sigact, NULL);
			
		sigdelset(&sigset, SIGUSR1);
		sigsuspend(&sigset);
	}
}