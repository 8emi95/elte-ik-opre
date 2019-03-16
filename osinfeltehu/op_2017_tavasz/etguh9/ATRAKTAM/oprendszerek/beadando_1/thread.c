#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void my_Handler()
{
	//
}

int main()
{
	int p = fork();
	if (p > 0)
	{
		4
	}
	printf("\np = %i\n", p);
	struct sigaction sigact;
	sigemptyset(&sigact.sa_mask); //no blocked signals only the one, which arrives
	sigact.sa_handler = my_Handler;
	sigact.sa_flags = 0;
	sigaction(SIGALRM, &sigact, NULL);

	return 0;
}
