#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <errno.h>

int main(){
	int i;
	for (i=1; i<4; i++){
		fork();
	}
	printf("Hello %i\n", getpid());
	return 0;
}