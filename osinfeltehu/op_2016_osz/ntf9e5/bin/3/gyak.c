#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void feladat1(){
	int x = 2;
	pid_t child = fork();
	if (child < 0){
		// hiba kiiras
		printf("hiba\n");
	} else if (child > 0){
		pid_t child2 = fork();
		
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("my pid: %i\n", mypid);
	} else {
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("my pid: %i\n", mypid);
	}
	
}

int main(){
	int value = 2;
	
	// ettol tobb szalu a program
	pid_t child = fork();
	
	if (child < 0){
		// hiba kiiras
		printf("hiba\n");
	} else if (child > 0){
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("parent process: %i\n", mypid);
		printf("my parent is: %i\n", parentpid);
	} else {
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("child process: %i\n", mypid);
		printf("my parent is: %i\n", parentpid);
	}
	
	printf("-------- Feladat 1 -----------\n");
	feladat1();
	return 0;
}