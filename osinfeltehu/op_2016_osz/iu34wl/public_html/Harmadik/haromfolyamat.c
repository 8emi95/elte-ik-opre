#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 
 pid_t child = fork(); //forks make a copy of variables

 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) 
 {
	pid_t child2 = fork();
	pid_t mypid = getpid();
	pid_t parentpid = getppid();
	
	printf("pidem: %i\n", mypid);
	//printf("(Parent1): Parentem: %i\n", parentpid);
	//printf("(Parent1): Childom: %i\n", child);
 }
 else //child process
 {
	pid_t mypid = getpid();
	pid_t parentpid = getppid();
	
	printf("pidem: %i\n", mypid);
	//printf("(Child1): Parentem: %i\n", parentpid);
 }
 
 return 0;
}