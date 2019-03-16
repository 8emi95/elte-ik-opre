#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 


int gvar=5;
int main()
{
 int status;
 int lvar=5;
// printf("The value is %i before forking \n",notacommonvalue);
 printf("The PID number of the process:%i\n",getpid());
 
 pid_t  child=fork();//fork makes a copy of variables

 if (child<0) { perror("Error..\n"); exit(1);}
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {  
    printf("Szulo %i\n",getpid());
    waitpid(child,&status,0);
    //returns the PID number of the child, that has stopped
    //1.parameter - the pid number of the child for which we have to wait - 
    //(-1) means, that it waits for any of the child processes  
    //the status of child process will be in status variable, 
    //0 means, it really waits for the end of child process - 
    //WNOHANG means, that it doesn't wait, but examine if there is a stopped child
    //waitpid(-1,&status,0) is the same as wait(&status)
     printf("The values are %i, %i in parent process (remain the original) \n",lvar,gvar);
    //printf("The PID number of the process:%i, the parent's PID %i\n",getpid(),getppid());
    //which process can be the parent of the parent??
 }
 else //child process
 {
    lvar=10;gvar=10;     
    printf("The values are %i, %i in child process \n",lvar,gvar);
    printf("The PID number of the process:%i, the parent's PID %i\n",getpid(),getppid());
 }
 //printf("Hello %i\n",getpid());
return 0;
}
