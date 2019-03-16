#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{ 
  int status;
  pid_t child = fork();
  if(child == 0) {
    printf("CHILD SPEAKING \n");
  }
  else {
    printf("PARENT SPEAKING \n");
  }
  
  if(child > 0) {
    waitpid(child,&status,0);
  }
  
  //printf("%i \n", getpid());

  if(child == 0) { printf("CHILD IS TERMINATING \n"); }
  else if(child > 0) { printf("PARENT IS TERMINATING \n"); }

  return 0;
}
