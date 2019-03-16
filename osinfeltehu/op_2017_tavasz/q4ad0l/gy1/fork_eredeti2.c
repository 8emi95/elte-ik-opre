#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int status2;
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0)
 {
    pid_t  child2=fork();
    waitpid(child,&status,0); 
    waitpid(child,&status2,0);  
 }
printf("Hello child %i, parent %i \n", getpid(), getppid());
 return 0;
}