#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{		
		pid_t child=fork();
		fork();
		printf("Hello %i, %i! \n",getpid(),getppid());
		if( child != 0)
			sleep(1);
	return 0;
}