#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main(){
	int status;
	int value = 1;
	printf("The value is %i before forking \n",value);

	pid_t  child = fork(); 

	//parent
	if (child > 0) {
		printf("Parent process id: %d\n",getpid());
		waitpid(child,&status,0); 
		printf("The value is %i in parent process (remain the original) \n",value);
	}

	//child
	else{
		printf("Child process id: %d\n",getpid());
		value = 5; 
		printf("The value is %i in child process \n",value);
	}
	return 0;
}