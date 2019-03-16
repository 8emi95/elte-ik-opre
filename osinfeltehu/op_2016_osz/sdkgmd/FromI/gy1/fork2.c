#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int *x=(int*)malloc(sizeof(int));
 *x=5;
 printf("The *X value is %i before forking \n",*x);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) // parent
 {
    printf("A gyerekem azonositoja: %i.\n", child);
	waitpid(child,&status,0); 
    printf("The *X value is %i in parent process (remain the original) \n",*x);
 
 }
 else //child process
 {
    int pid=getpid();
    printf("En vagyok a gyerek, az azonositom: %i.\n",pid);
    *x+=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",*x);
 }
 return 0;
}
