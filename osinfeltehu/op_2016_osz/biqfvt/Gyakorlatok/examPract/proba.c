#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

#include <signal.h>
#include <sys/types.h>

volatile counter=0;

void handler(int signumber){
//  printf("Signal with number %i has arrived\n",signumber);
// printf("handler  %i\n",counter);
 counter++;
}
int numvoter;
void president()
{
//  pause();
//  pause();
 // printf("I am the president\n");fflush(stdout);
  while (counter!=2){
    //usleep(100);
    ;
  }  
}
void checker()
{
//  printf("I am the checker\n");
  kill(getppid(),SIGUSR1);
  
}
void sealer()
{
  //printf("I am the sealer\n");	
  kill(getppid(),SIGUSR2);
}
int main(int argc, char ** argv)
{
 int status;
 signal(SIGUSR1,handler);
 signal(SIGUSR2,handler);          		   
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