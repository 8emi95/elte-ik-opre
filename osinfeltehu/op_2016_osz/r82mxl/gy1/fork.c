#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 int *x=(int*)malloc(sizeof(int));
 *x=5;
 printf("The *X value is %i before forking \n", *x);
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    printf("A gyermekem azonosítója: %i.\n", child);
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
    printf("The *X value us %i in parent process (remain the original) \n", *x);
 
 }
 else //child process
 {
    int pid=getpid();
    printf("Én vagyok a gyerek, az azonosítóm: %i", pid);
    *x+=5; //it changes the value of the copy if the variable
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
 }
 return 0;
}
