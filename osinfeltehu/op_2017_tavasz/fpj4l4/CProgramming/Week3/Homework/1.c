#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{ 
  pid_t child = fork();
  printf("Hello world \n");
   
  return 0;
}
