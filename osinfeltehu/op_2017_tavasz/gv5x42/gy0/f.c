#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
  printf("parent %i\n",getpid());
  pid_t child=fork();
  pid_t child2=fork();
  printf("Hello %i %i \n",getpid(),getppid());
  return 0;
}