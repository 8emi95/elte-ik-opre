#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    fork();
	//waiteknek ez a sorrend kell!
	waitpid(child,&status,0);//szülőfolyamat ezáltal bevárja a gyerekfolyamat végét 
    wait(&status);
    printf("szulo %i\n", getpid());
	fflush(stdout);
 
 }
 else //child process
 {
    
    printf("Gyerek %i %i\n",getpid(), getppid());
 }

 return 0;
}