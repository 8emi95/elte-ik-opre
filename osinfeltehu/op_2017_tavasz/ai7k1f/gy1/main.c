#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time
#include <signal.h> //sigval
	
void handler(int sign){
}

int main(int argc, char* argv[]){
	
	int i;
	char cim[512];
	int pipes[2];
	char msg[512];
	
	if(argc<2){
		printf("Add meg a lakcimet\n");
		exit(1);
	}
	
	strcpy(cim,"");
	for (i=1; i<argc;++i){
		strcat(cim,argv[i]);
	}
	pid_t parentID = getpid();
	pid_t pid;
	
	signal(SIGUSR1,handler);
	
	if (pipe(pipes) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(1);
	}
	
	pid=fork();
	if(pid > 0) { 
		pause();
		printf("A kozpont jelzest kapott!\n");
		close(pipes[1]);  
	    printf("Az uzenet feldolgozasa!\n");
        read(pipes[0],msg,sizeof(msg)); 
		printf("A megerkezett uzenet: %s \n",msg);
		close(pipes[0]);   
		int status;
		wait(&status);
		printf("A cim feldolgozasa veget ert!\n");
	}
	else {
		sleep(2);
		kill(getppid(),SIGUSR1);
		close(pipes[0]); 
		write(pipes[1],cim,sizeof(cim));
		close(pipes[1]); 
		printf("Egy utas megadta a cimet!\n");
		printf("Az utas befejezte a küldest! (%d)\n");
	}
	
	return 0;
}