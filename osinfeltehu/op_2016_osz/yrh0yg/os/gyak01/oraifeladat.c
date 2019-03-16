#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{	
	int status;

	int parent_pid=getpid();
	pid_t child=fork();
	if(getpid() == parent_pid) {
		pid_t child2 = fork();
		waitpid(child,&status,0);
		waitpid(child2,&status,0);
	}
	printf("Hello %i %i\n", getpid(), getppid());
	return 0;

}