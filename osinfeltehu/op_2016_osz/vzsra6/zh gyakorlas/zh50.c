#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <string.h>

int pipefd1[2];

static void actionhandler (int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_value.sival_int == 0)
		write (pipefd1[1], "bevontattak\n", sizeof ("bevontattak"));
	else
		write (pipefd1[1], "beert\n", sizeof ("beert"));
}


int main (int argc, char* argv[])
{
	pid_t child1;
	int current = 1;
	int max = atoi (argv[1]);
	char buf[160];
	int status;
	
	if (pipe (pipefd1) == -1)
		perror ("1. pipe");
	
	child1 = fork ();
	
	if (child1 == -1)
		perror ("fork");
	
	if (child1 > 0) { //parent
	
		struct sigaction sa;
		sigemptyset (&sa.sa_mask);
		sa.sa_sigaction = actionhandler;
		sa.sa_flags = SA_SIGINFO;
		sigaction (SIGUSR1, &sa, NULL);
		
		write (pipefd1[1], &current, sizeof (current));
		sleep (1);
		read (pipefd1[0], buf, sizeof (buf));
		printf (buf);
		
		waitpid (child1, &status, 0);
		
	} else { //kid
	
		int resp1;
		read (pipefd1[0], &resp1, sizeof (resp1));
		srand (time (NULL) ^ (getpid () << 16));
		int rand1 = rand () % 10;
		
		if (rand1 == 2) {
			
			printf ("elcsuszott a %d. szerelveny es meg ennyi lesz utanam: %d\n", resp1, (current - 1));
			union sigval sv;
			sv.sival_int = 0;
			sigqueue (getppid (), SIGUSR1, sv);
			
		} else {
			
			printf ("sikeresen teljesitette az uzemet\n");
			
		}
	}

	return 0;
}