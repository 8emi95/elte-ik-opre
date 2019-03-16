#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
  
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0)
 {
	int status;
   	fork(); 
	waitpid(child, &status, 0);
	wait(&status);
 }

 printf("I'm a process (%i) \n", getpid());

 return 0;
}