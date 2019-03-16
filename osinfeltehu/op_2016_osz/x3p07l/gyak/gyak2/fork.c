#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child1=fork(); //forks make a copy of variables
 if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child1>0) //the parent process, it can see the returning value of fork - the child variable!
 {
  pid_t child2=fork();
  waitpid(child1,&status,0);
  waitpid(child2,&status,0);
  if (child2>0){
   printf("szulo\n");
  }
  else{
   printf("gyerek\n");
  }
/*    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);*/
 }
 else
 {
 printf("gyerek\n");
 }
 return 0;
}