#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
  int value =2;
  pid_t child = fork();
  
  if(child < 0)
  {
    //error
  }
  else (child > 0)
  {
    pid_t second_child= frok();
    pid_t mypid=getpid();
    pid_t parentpid= getppid();
    printf("En vagyok a parent: %i\n", mypid);
    printf("(parent): Parentem: %i\n", parentpid);
    printf("(Parent): Childom: %i\n", child);
  }
  else
  {
    pid_t mypid=getpid();
    pid_t parentpid= getppid();
    printf("En vagyok a child: %i\n", mypid);
    printf("(Child): Parentem: %i\n", parentpid);
  }
  return 0;
  
}