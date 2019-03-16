#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking %i\n",notacommonvalue,getpid());
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); //wait(&status) 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process %i (remain the original) \n",notacommonvalue,getpid());
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process %i \n",notacommonvalue,getpid());
 }
 return 0;
}