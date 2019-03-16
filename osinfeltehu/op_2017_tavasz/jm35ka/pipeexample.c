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
	
	int pipe1[2], pipe2[2]; //unnamed pipe
	
	if(pipe(pipe1)==-1 || pipe(pipe2)==-1){
		perror("Error in creating pipe!");
		exit(1);
	}
	
	child=fork();
	
	if(child==0){
		kill(getppid(), SIGUSR1); //child send signal to parent
		close(pipe1[0]); //child closing the reading end
		close(pipe2[1]); //child closing writing end of pipe2
		
		char text[256]="Hello parent!\n";
		char text2[256];
		
		write(pipe1[1], &text, sizeof(text));
		read(pipe2[0], &text2, sizeof(text2));
		
		printf("Child received this text from parent: %s\n", text2);

		close(pipe1[1]); //child closing the writing end
		close(pipe2[0]); //child closing reading end of pipe2
		exit(0);
	}else{
		pause(); //parent waiting for signal from child
		close(pipe1[1]); //parent closing writing end
		close(pipe2[0]); //parent closing read end of pipe 2
		
		char text[256];
		char text2[256]="Hello child!\n";
		
		read(pipe1[0], &text, sizeof(text));
		write(pipe2[1], &text2, sizeof(text2));
		
		printf("Parent received this text from child: %s\n", text);
		
		close(pipe1[0]); //parent closing reading end
		close(pipe2[1]); //parent closing writing end of pipe2 
	
	}
	
	return 0;
}
