#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void check_input(int argc,char* argv[]){
	if(argc==1) {
		printf("Elso parameterben meg kell adni, mennyi megfigyeles kell!\n");
		exit(1);
	}
	if(argc==2) {
		printf("Masodik parameterben meg kell adni, mennyi ideig kell figyelni!\n");
		exit(1);
	}
}
void handler(int signumber){
	pid_t my_id = getpid();
	printf("I arrived to the city (%i,%i).\n",my_id,signumber);
}

int main(int argc, char* argv[])
{
	check_input(argc,argv);
	int n = atoi(argv[1]);
	int time = atoi(argv[2]);
	int i=0;
	
	
	struct sigaction sigact;
	sigact.sa_handler=handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags=0;
	sigaction(SIGTERM,&sigact,NULL);
	sigaction(SIGUSR1,&sigact,NULL);
	
	pid_t parendID = getpid();
	pid_t child;
	for(i=0;i<n;++i){
		if(getpid() == parendID){
			child = fork();
		}
	}
	
	if( child < 0) {
		perror("Something went wrong!!!!!!!!!\n");exit(1);
	}
	else if(child>0){
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGTERM);
		sigsuspend(&sigset);
		
		int status;
		waitpid(child,&status,0);
		printf("Parent: I'm here\n");
		printf("Parent ended\n");
	}
	else {
		printf("Are you there, ID%i?\n", child);
		printf("Sending my signal back -> %i\n",SIGTERM);
		sleep(3);
		kill(getppid(),SIGTERM);
		printf("Child ended!\n");
	}
	
	
    return 0;
}
