#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	//valami task szerű lel
	int i;
	for(i=1;i<4;i++)
		fork();
	printf("Hello %i - parent: %i\n ",getpid(),getppid());
	
	
	return 0;
}