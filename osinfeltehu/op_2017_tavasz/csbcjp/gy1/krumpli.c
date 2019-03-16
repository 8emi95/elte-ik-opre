#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 


int main()
{
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    child = fork();
 }
 if (child == 0) //child process
 {
    printf("gyerek: getpid=%i getppid=%i\n",getpid(), getppid());
 }
 return 0;
}