#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main(){

	int status;
	
	pid_t child=fork();
	//pid_t child2=fork();
	
	if (child<0){
		perror("The fork calling was not succesful\n");
		exit(1);
	} 
	
	if (child>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		waitpid(child,&status,0); 
		printf("The end of parent process\n");
	}
	else //child process
	{
		printf("The child process\n");
	}
	
	return 0;
	
}