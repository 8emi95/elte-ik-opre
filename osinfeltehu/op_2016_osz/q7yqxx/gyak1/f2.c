/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

int main(){
	int status;
	pid_t child = fork();

	if (child>0){
		waitpid(child,&status,0); 
		wait(&status,0); //egy gyerek ha befejeződik, akkor lép tovább
		printf("Process ID = %i \n",getpid());
	}
	else{
		fork();
		printf("Process ID = %i %i\n",getpid(), getppid());
	}
	return 0;
}

//vagy

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

int main(){
	int status;
	int parent_pid = getpid();
	pid_t child = fork();

	if (getpid() == parent_pid){
		pid_t child2 = fork();
		waitpid(child,&status,0);
		waitpid(child2,&status,0);
	}
	printf("Hello %i %i\n",getpid(),getppid());
	return 0;
}*/