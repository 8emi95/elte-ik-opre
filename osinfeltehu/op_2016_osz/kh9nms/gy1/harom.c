#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
	fork();
    waitpid(child,&status,0); 
	wait(&status);
 
 }
 else //child process
 {

 }
 
 printf("Hello %i \n",getpid());
 return 0;
}