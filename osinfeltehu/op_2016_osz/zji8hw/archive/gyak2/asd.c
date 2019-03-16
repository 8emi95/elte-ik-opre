#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void handler(int n)
{
	int i;
	for (i = 0; i < 200; i++)
		printf("The program was alarmed by alarm function\n");
}

int main()
{
	struct sigaction sigact;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = handler;
    sigact.sa_flags = 0;
    sigaction(SIGALRM, &sigact, NULL);
	
    alarm(2);
    pause();
	
	int j;
	for (j = 0; j < 200; j++)
		printf("kecske\n");
    return 0;
}