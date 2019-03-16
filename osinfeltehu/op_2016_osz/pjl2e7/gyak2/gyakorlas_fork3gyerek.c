#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 pid_t child = fork();
 if(child > 0){
  int status;
  waitpid(child, &status, 0);
  pid_t sibling = fork();
  if(sibling > 0) waitpid(sibling, &status, 0);
 }else{
     int status;
     pid_t grandchild = fork();
     if(grandchild > 0)
     waitpid(grandchild, &status, 0);
 }
 
 printf("Hello %i %i \n", getpid(), getppid());
 fflush(stdout);
 
 
 return 0;
}