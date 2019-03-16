#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{

 // pid_t en=getpid(;
  pid_t child=fork(); //forks made copy of variables
  
  /*if(child==-1) {perror("...");exit(1);}
  if(child>0){
    int status;
    waitpid(child,&status,0); //vár a gyerek végére
    printf("szulo\n");
  }
  else{
    printf("gyerek\n");
  }
  int i;
  for(i=1;i<4;++i)
  {*/
    fork();
  //}
  
 printf("Hello %i/n",getpid(),getppid());
 fflush(stdout);
  return 0;
}