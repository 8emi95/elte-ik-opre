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
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	char pMessage[] = "-2";
	printf("%d", atoi(pMessage) + atoi(pMessage));
	return 0;
}
