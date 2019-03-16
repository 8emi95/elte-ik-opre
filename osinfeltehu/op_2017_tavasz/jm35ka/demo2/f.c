#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
   printf("parent  %i\n", getpid());
   
   int i;
   
   for(i=0; i<4; i++){
    
      pid_t  child=fork(); //forks make a copy of variables
  } 
   
   printf("Hello %i %i \n",getpid(),getppid());
   return 0;
   
}