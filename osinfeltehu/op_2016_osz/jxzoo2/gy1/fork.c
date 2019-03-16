#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 //int notacommonvalue=1;
 int* x = (int*)malloc(sizeof(int)); //muszaj castolni, altalanos tipusmentes malloc
 //printf("The value is %i before forking \n",notacommonvalue);
  printf("The *x value is %i before forking \n",*x);

 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {//printf("A gyerek azonositoja: %i, child);
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    //printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 
 }
 else //child process
 {
int pid = getpid();
printf("En vagyok a gyerek, az azonositom: %i.\n",pid);
 //   notacommonvalue=5; //it changes the value of the copy of the variable
*x+=5;
 //   printf("The value is %i in child process \n",notacommonvalue);
  printf("The value is %i in child process \n",*x);
 }
 return 0;
}