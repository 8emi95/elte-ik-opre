#include<stdio.h>
#include<sys/types.h>


int main(){
	pid_t pid, pid2;	

	pid = fork();
	pid2 = fork();
	
	printf("pid= %i getpid= %i getppitf= %i\n", pid, getpid(),getppid());
	printf("pid2=%i getpid= %i getppid= %i\n",pid2,getpid(),getppid());

	



}
