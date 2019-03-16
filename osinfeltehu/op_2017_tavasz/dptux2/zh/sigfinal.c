#include <stdio.h>
#include <stdlib.h>//exit
#include <sys/msg.h> 
#include <sys/types.h>
#include <signal.h>
#include <string.h> 
#include <unistd.h> //fork,pipe


void handler(int signumber){
	printf("Signal with number %i has arrived\n",signumber);
}


int main(int argc, char *argv[]){
	
	//Block procmask
	sigset_t block_mask;
	sigemptyset (&block_mask);
	sigaddset (&block_mask, SIGUSR1);
	sigprocmask (SIG_SETMASK, &block_mask, NULL);
	//Critical procmask
	sigset_t critical_mask;
	sigfillset(&critical_mask);
	sigdelset(&critical_mask,SIGUSR1);
	
	//Handler setup
	signal(SIGUSR1,handler);
	
	pid_t child = fork();
	
	if(child < 0){
		perror("The fork calling was not successful\n");
		exit(1);
	}
	else if(child > 0){
		sleep(4);
		sigsuspend(&critical_mask);
		printf("arrived to parrent\n");
		kill(child,SIGUSR1);
		printf("parrent sent sig\n");
		wait();
		printf("parrent end\n");
		
	}	
	else{
		kill(getppid(),SIGUSR1);
		printf("child sent sig\n");
		sleep(8);
		sigsuspend(&critical_mask);
		printf("arrived to child\n");
		printf("child end\n");
	}
	return 0;
};