#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int i;
 pid_t parent = getpid();

 for(i = 0; i < 4; i++) {
  if(getpid() == parent) {
   pid_t  child=fork(); //forks make a copy of variables
   if(child > 0) {
    printf("szulo %i\n", getpid());
   } else {
    printf("gyerek %i\n", getpid());
   }
  }
 }

 return 0;
}
