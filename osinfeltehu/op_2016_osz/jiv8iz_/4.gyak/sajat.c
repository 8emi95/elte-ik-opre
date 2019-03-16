#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //perror, errno
#include <unistd.h> // fork
#include <sys/wait.h> //waitpid

int main() {
	int value = 2;
	pid_t child = fork();
	
	if(child<0){
		//errorvalami nem sikerült a forknál
		printf("Valami hiba\n");
	} else if(child >0) {  // gyerek folyamatban vagyunk
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("En vagyok a parent: %i\n", mypid);
		printf("(Parent) Parentem:: %i\n",parentpid);
		printf("(Parent): Childom: %i\n", child);
		
	}else {  // chiild =0 akkor nem vagyok a gyerekben
		
		pid_t mypid = getpid();
		pid_t parentpid = getppid();
		printf("En vagyok a child: %i\n", mypid);
		printf("(Child) Parentem:: %i\n",parentpid);
		
	}
	
	return 0;
}
