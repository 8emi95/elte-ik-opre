#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
	printf("Signal with number %i has arrived\n",signumber);
}

int main(){
	
	signal(SIGTERM,handler);
	
	pid_t child=fork();
	
	
	if (child>0){
		pause();
		printf("Signal arrived\n",SIGTERM);
		int status;
		wait(&status);
		printf("Parent process ended\n");
	}

	else{
		printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
		sleep(3);
		kill(getppid(),SIGTERM); 
		printf("Child process ended\n");
	}
	
	return 0;
}