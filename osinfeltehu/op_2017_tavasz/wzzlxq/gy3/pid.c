#include <sys/types.h>
#include <stdio.h>
int main(){
	printf("pid=%i ppid=%i", getpid()+1, getpid+2);
	pid_t = fork();
	printf("pid=%i ppid=%i", pid, ppid);	
	
	
}
