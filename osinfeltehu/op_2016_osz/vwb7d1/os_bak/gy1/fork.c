#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

int main()
{
	pid_t child = fork();
	
//	if (child > 0) {
//		printf("Szulo\n");
//	} else {
//		printf("Gyerek\n");
//	}
	
	fork();
	printf("PID: %i\n", getpid());
	
	return 0;
}