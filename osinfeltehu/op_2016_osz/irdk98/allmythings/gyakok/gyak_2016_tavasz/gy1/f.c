#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int i;
 pid_t ch=fork();
 if (ch>0){
   fork();
 } 
 printf("Hello %i %i\n",getpid(),getppid());
 return 0;
}