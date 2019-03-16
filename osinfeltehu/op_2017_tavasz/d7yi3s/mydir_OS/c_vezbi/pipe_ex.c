#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for pipe
#include <string.h>
#include <signal.h>

//unamed pipe example
void handler1(int sig){
	printf("Parent finished writing!\n");
	signal(sig, handler1);
}

int main(){
	signal(SIGUSR1, handler1);

	int pipefd[2]; //unnamed pipe file descriptor array
	pid_t pid;
	char rp[100];

	if(pipe(pipefd) == -1){
		perror("Oppening error");
		exit(1);
	}

	pid = fork(); //creating parent-child processes
	if(pid < 0){
		perror("Fork error!");
		exit(1);
	}

	if(pid == 0){
		//child process
		close(pipefd[1]); //closing write end
		printf("Child starts to read from the pipe\n");
		read(pipefd[0], rp, sizeof(rp)); //reading max of 100 char
		printf("Child read the message: %s\n", rp);
		printf("\n");
		close(pipefd[0]); //finaly we close the user read end

		pause();

		//now child can write
		write(pipefd[1], "hello child writing", 19);
		close(pipefd[1]);



	}else {
		//parent process
		printf("Parent starts\n");
		close(pipefd[0]);
		write(pipefd[1], "Hello child thro pipe!", 22);
		close(pipefd[1]); //closing write descriptor
		printf("parent wrote the msg to the pipe\n");
		printf("Parent finished\n");
		kill(pid, SIGUSR1);

		read(pipefd[0], rp, sizeof(rp));
		printf("parent read the message: %s\n", rp);
		printf("\n");

		close(pipefd[0]);



	}

	exit(0);
}


