#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	// int i;
	// for(i=1;i<4;i++)
	// {
		// fork();
	// }
	//fork();
	//fork();
	// printf("Hello: %i - %i\n", getpid(), getppid());
	//getppid() -> ha 1-et ad vissza, akk a szuloje mar befejezodott
	
	//fel1:
	//Feladat, indítsunk el 3 process-t



	 int status;
	
	 int parent_pid=getpid();
	 pid_t child=fork();
	 if(getpid()==parent_pid){
		 pid_t child2=fork();
		 waitpid(child,&status,0);
		waitpid(child2,&status,0);
	 }
	 printf("Hello %i %i\n",getpid(),getppid());


	return 0;
}