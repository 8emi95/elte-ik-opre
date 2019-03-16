#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main(){
	pid_t child = fork();
	fork();
	printf("Hello\n");
	return 0;
}
