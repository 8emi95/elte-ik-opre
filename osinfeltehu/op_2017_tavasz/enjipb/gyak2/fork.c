#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 pid_t  child=fork(); //forks make a copy of variablesC
 fork();
 printf("Hello %i \n", getpid());
 return 0;
}
