#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>


int main()
{
  int status = 0;
  pid_t  child=fork(); //forks make a copy of variables
  if (child < 0){perror("The fork calling was not succesful\n"); exit(1);}
  if (child > 0) //the parent process, it can see the returning value of fork - the child variable!
  {
    fork();
    waitpid(child, &status, 0);
    wait(&status); // bármelyik első gyereket bevárja
  }
  else
  {
    printf("i am child %i\n", child);
  }
  printf("hello %i\n", getpid());
}                                   