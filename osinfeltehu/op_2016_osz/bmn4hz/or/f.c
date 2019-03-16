#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	pid_t child = fork();
	//int child = getpid();
	/*for(int i = 0; i < 5; i++){
		if(getpid() == id){
			fork();
		}
	}*/
	if(child == -1){
		perror("..");
		exit(1);
	}
	/*if(0 < child){
		int status;
		waitpid(child,&status, 0);
		printf("Szulo \n");
	}else{
		printf("Gyerek \n");
	}*/
	printf("Hello %i vagyok %i gyereke!\n", getpid(),getppid());
	fflush(stdout);
	return 0;
}