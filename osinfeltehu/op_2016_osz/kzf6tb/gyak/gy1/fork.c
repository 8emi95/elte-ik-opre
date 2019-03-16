#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	/*
	pid_t child = fork();
	fork();
	fork();
	
	
	if (child == -1) {
		perror("hiba!");
		exit(1);
	}
	if (child>0) {
		int status;
		waitpid(child,&status,0);
		printf("szulo\n");
	}
	else {
		printf("gyerek %i. p: %i\n",getpid(), getppid());
		fflush(stdout);
	}

 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
 }*/
 
	pid_t child1 = fork();
	if (child1>0) {
		fork();
		int s;
		waitpid(child1,&s,0);
	}
	
	printf("asd\n");
 return 0;
}