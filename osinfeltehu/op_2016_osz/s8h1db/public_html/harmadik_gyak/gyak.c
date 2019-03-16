#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	int status;
	int value = 2;
	pid_t child = fork();
	if (child < 0) {
		printf("Not successful fork\n");
	}
	else if (child > 0) {
		pid_t parentpid = getpid();
		pid_t parent2pid = getppid();
		printf("Parent process: %d, my parent's pid: %d, my child's pid: %d\n", parentpid, parent2pid, child);
		waitpid(child, &status, 0);
	}
	else {
		pid_t childschild = fork();
		if (childschild < 0) {
			printf("Not successful fork #2\n");
		}
		else if (childschild > 0) {
			pid_t childpid = getpid();
			pid_t parentpid = getppid();
			printf("Child#1 process: %d, my parents's pid: %d, my child's pid: %d\n", childpid, parentpid, childschild);
		}
		else {
			pid_t childpid = getpid();
			pid_t parentpid = getppid();
			printf("Child#2 process: %d, my parents's pid: %d\n ", childpid, parentpid);
		}
	}
	return 0;
}
