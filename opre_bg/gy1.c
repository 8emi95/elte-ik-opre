#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 
 pid_t  child=fork(); //forks make a copy of variables
 fork();
 printf("\nHello %i\n", getpid());
 printf("Szülő: %i\n", getppid());

 return 0;
}