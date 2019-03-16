#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int value = 2;
 pid_t child = fork();
 if(child < 0)
 {
  //error
 }
 else if(child>0)
 {
  pid_t mypid = getpid();
  pid_t parentpid = getppid();
  printf("en vagyok a parent: %i. ", mypid);
  printf("parentem: %i. ", parentpid);
  printf("childom: %i\n", child);
 }
 else
 {
  pid_t child2 = fork();
  pid_t mypid = getpid();
  pid_t parentpid = getppid();
  printf("en vagyok a child: %i. ", mypid);
  printf("parentem: %i. ", parentpid);
  printf("childom: %i\n", child2);
 }
 
 return 0;
}
