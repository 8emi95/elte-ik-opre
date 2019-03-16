#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>


int main()
{
	int status;
	pid_t  child = fork();

	if(child>0) {
		fork();
		waitpid(child,&status,0);
		wait(&status);
	}
	printf("Hello %i %i\n", getpid(), getppid());
	return 0;
}