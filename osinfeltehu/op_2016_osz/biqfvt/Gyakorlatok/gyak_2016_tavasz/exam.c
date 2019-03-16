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
	pid_t child2=fork();
	if (child2<0){perror("The fork calling was not succesful\n"); exit(1);} 
     if (child2>0){ 
	   waitpid(child2,&status,0);
       waitpid(child,&status,0); //wait(&status) 
       printf("The value is %i in parent process %i (remain the original) \n",notacommonvalue,getpid());
     }else 
	 {
		 //second child
	 }
 }
 else 
 {//1st child process
 }
 return 0;
}