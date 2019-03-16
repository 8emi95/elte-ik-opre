#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int *notacommonvalue = (int*)malloc(sizeof(int));

 *notacommonvalue = 5;

 printf("The value is %i before forking \n",*notacommonvalue);
 
 pid_t  child=fork();

 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0)
 {
    waitpid(child,&status,0); 
    printf("The value is %i in parent process (remain the original) \n",*notacommonvalue);
 
 }
 else
 {
    *notacommonvalue=1;
    printf("The value is %i in child process \n",*notacommonvalue);
 }
 return 0;
}
