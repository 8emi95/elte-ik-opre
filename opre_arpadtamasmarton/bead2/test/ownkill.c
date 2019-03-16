#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // DateTime kezeles
#include <fcntl.h> // Rendszerhivasok file
#include <unistd.h> // fork
#include <sys/types.h> // pid_t típus
#include <sys/wait.h> // wait, waitpid
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

void HandlerParent(int signalNumber){
	printf("Parent received signal: %d\n", signalNumber);
}

int main (void)
{
	signal(SIGUSR1, HandlerParent);
	signal(SIGUSR2, HandlerParent);
	
	kill(getpid(), SIGUSR1);
	sleep(3);
	return 0;
}
