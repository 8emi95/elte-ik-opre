#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h>

int main(){

	pid_t child = fork();

	if(child < 0){
		perror("The fork calling was not succesful\n");
		exit(1);
	}
	if(child > 0){
		//parent
		 waitpid(child,&status,0); 
   		 printf("The end of parent process\n");
	}else {
		//child
		execv("/mydir_OS/c_vezbi", "gcc test.c -o test");
		 printf("It never returns to child process (except on error)- 
    	the content of the whole child process will be changed to the new one");
		 execv("/mydir_OS/c_vezbi", "./test 4 test");
		  printf("It never returns to child process (except on error)- 
    	the content of the whole child process will be changed to the new one");
	}
	return 0;
}