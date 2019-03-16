#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main(){
 int status;
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0);
    printf("The end of parent process\n");
 }
 else //child process
 {
	pid_t  child2=fork();
	waitpid(child2,&status,0);
	pid_t  child3=fork();
	waitpid(child3,&status,0);		
	char * command;
	/*char * arg1;
	char * arg2;*/
    printf("Command to run:\n");
    scanf("%s",command);
	/*printf("1. Argument:\n");
    scanf("%s",arg1);
	printf("2. Argument:\n");
    scanf("%s",arg2);*/
	char * arg[]={command,NULL}; 
    execv(command,arg);
    printf("It never returns to child process (except on error)- the content of the whole child process will be changed to the new one");
 }
 return 0;
}