#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main()
{
	int status;
 	char str1[30];
	scanf("%s",str1);

 	pid_t  child=fork(); //forks make a copy of variables
 	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 	if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 	{
    		 waitpid(child,&status,0); 
 	  	 printf("The end of parent process\n");
 	}
 	else //child process
 {
}