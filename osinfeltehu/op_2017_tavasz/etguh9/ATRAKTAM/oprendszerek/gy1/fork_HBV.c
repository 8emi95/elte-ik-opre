#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
  pid_t child=fork();
  fork();   //m�g egy m�solat (ez �gy m�r �sszesen 4, mivel mindk�t el�z�ben lefut a fork)
  printf("Hello %i %i \n", getpid(), getppid());   //ha a getppid() 1-et ad  ->  a sz�l� m�r nem fut

  return 0;
}