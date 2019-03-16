#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 
 

 pid_t child1 = fork();
 if(child1 > 0){
  int status;
  //waitpid(child, &status, 0);
  pid_t child2 = fork();
  waitpid(child1, &status, 0);
  wait(&status);
  printf("called from: %i", getpid());
 }
 printf("Hello %i %i \n", getpid(), getppid());
 //fflush(stdout);
 
 
 return 0;
}