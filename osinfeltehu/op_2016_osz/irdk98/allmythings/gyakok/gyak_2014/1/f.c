#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 

pid_t childs[3];
void onlychilddoes(){
  printf("I am a new born child %i \n",getpid());
//  printf("I know the PIDs as follows (childs[]): %i,%i,%i\n",childs[0],childs[1],childs[2]);
}
int main()
{
 int n=3;
 pid_t parent=getpid();
 int i=0;
 while ((i<n)&&(parent==getpid())){
   childs[i]=fork();
   if (childs[i]==0) onlychilddoes();
   sleep(1); //to see the childs[] 
   i++;
 }
 
// printf("Hello %i\n",getpid());
// if (parent==getpid()){
  printf("I am %i, the children's PID as follows (childs[]): %i,%i,%i\n",getpid(),childs[0],childs[1],childs[2]);
  for (i=0;i<n;i++){
     wait(NULL);  //waits for all the 3 children
  }
  printf("The END\n");
 
//}
return 0;
}