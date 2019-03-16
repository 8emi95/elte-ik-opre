#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <semaphore.h>

int main(int argc, char *argv[]){
	
	
	int b = atoi(argv[1]);
	printf("%d", b);
	
	return 0;
}