#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{ 
  int status;
  pid_t child = fork();

  if(child != 0) {
    pid_t child = fork();
  }
  
  printf("Terminating \n");

  return 0;
}