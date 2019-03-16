#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

int main()
{
 int status;
 pid_t child = fork();
 if (child < 0) {perror("The fork calling was not succesful\n"); exit(1);}
 if (child > 0) //parent process
 {
  pid_t child2 = fork();
  if (child < 0) {perror("Problem with second child\n"); exit(1);}
  if (child2 > 0)
  {
   while(x==0){}
   waitpid(child, &status, 0);
  }
 }
}