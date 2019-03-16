#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}
void president()
{
  pause();
  pause();  
}
void checker()
{
  kill(getppid(),SIGTERM);
}
void sealer()
{
  kill(getppid(),SIGTERM);
}
int main()
{
 int status;
 signal(SIGTERM,handler);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
	pid_t child2=fork();
	if (child2<0){perror("The fork calling was not succesful\n"); exit(1);} 
     if (child2>0){
       president(); 		 
	   waitpid(child2,&status,0);
       waitpid(child,&status,0); //wait(&status) 
     }else 
	 {
		 //second child
		 sealer();
	 }
 }
 else 
 {//1st child process
    checker();
 }
 return 0;
}