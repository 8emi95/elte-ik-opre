#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(){
	int value = 2;
	pid_t child = fork();
	if(child < 0){
		//error
	}
	else if(child > 0){
		pid_t mypid= getpid();
		pid_t myparentid = getppid();
		printf("Parent: %i, Parentem: %i\n", mypid, myparentid);
	}
	else{
		pid_t mypid= getpid();
		pid_t myparentid = getppid();
		printf("Child: %i, Parentem: %i\n", mypid, myparentid);
	}
	return 0;
}