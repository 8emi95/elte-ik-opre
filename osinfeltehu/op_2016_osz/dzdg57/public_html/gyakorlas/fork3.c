#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit()
#include <sys/wait.h> // wait()

int main()
{
	
	pid_t childPIDorZero = fork();
	
	if(childPIDorZero < 0) {
		perror("fork()error");
		exit(-1);
	}
	
	if(childPIDorZero != 0) {
		printf("I'm the parent %d, my child is %d\n", getpid(), childPIDorZero);
		wait(NULL); // wait for child process to join with this parent
	} else {
		printf("I'm the child %d, my parent is %d\n", getpid(), getppid());
		execl("/bin/echo", "echo", "Hello, World", NULL);
	}
	
	return 0;
}