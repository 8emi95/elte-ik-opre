#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
	printf("signal %i arrived\n",signumber);
}

int main(){
	
	//signal(SIGTERM,handler);
	
	struct sigaction sigact;
	sigact.sa_handler=handler;
	
	
	pid_t ch=fork();
	
	if(ch==0){
		
		
		kill(getppid(),SIGTERM);
		printf("CH: signal %i sended\n",SIGTERM);
		printf("CH: process ended\n");
		
	}else{
		pause();
		printf("P: process ended\n");
	}
	
	return 0;
}