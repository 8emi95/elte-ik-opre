#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
  pid_t child=fork();
  fork();   //még egy másolat (ez így már összesen 4, mivel mindkét elõzõben lefut a fork)
  printf("Hello %i %i \n", getpid(), getppid());   //ha a getppid() 1-et ad  ->  a szülõ már nem fut

  return 0;
}