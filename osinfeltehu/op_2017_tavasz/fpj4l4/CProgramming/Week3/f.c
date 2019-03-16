#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
  printf("parent %i\n",getpid());
  int i;
  for(i = 1; i < 4; i++)
    pid_t child = fork();

  printf("Hello %i %i \n", getpid(), getppid());
  return 0;
}

