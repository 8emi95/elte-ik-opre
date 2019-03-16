#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

void handler(int sig){
	printf("Child signaling parent that it is ready to start!\n");
	signal(sig, handler);
}

int main(int argc, char *argv[]){

	signal(SIGUSR1, handler);
	
	pid_t child;
	
	int pipe1[2]; //unnamed pipe
	
	if(pipe(pipe1)==-1){
		perror("Error in creating pipe!");
		exit(1);
	}
	
	child=fork();
	
	if(child==0){
		kill(getppid(), SIGUSR1); //child send signal to parent
		close(pipe1[0]); //child closing the reading end
		
		char text[256]="Hello parent!\n";
		
		write(pipe1[1], &text, sizeof(text));
		close(pipe1[1]); //child closing the writing end
		exit(0);
	}else{
		pause(); //parent waiting for signal from child
		close(pipe1[1]); //parent closing writing end
		
		char text[256];
		
		read(pipe1[0], &text, sizeof(text));
		
		printf("Parent received this text from child: %s\n", text);
		
		close(pipe1[0]); //parent closing reading end
	
	}
	
	return 0;
}
