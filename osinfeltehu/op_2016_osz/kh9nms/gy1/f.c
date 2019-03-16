#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
int main(){
	
	//pid_t en=getpid();
	pid_t child=fork();
	fork();
	printf("Hello %i %i\n",getpid(), getppid());
	fflush(stdout);
	return 0;
}