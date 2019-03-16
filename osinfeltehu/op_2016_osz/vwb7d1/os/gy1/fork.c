#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

int main()
{
	int status;
	pid_t child = fork(); // A fork() utáni kód minden folyamatban benne lesz
	
	if (child > 0) {
		printf("Szulo\n");
		
		waitpid(child, &status, 0);
	} else {
		printf("Gyerek\n");
	}

	printf("PID: %i\n", getpid());
	
	return 0;
}