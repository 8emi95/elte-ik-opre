#include <stdio.h> 

int main() {
	int status;
	pid_t child;
	
	child = fork();
	
	if (child < 0) {
		perror("Fork hiba");
		exit(-1);
	}
	
	return 0;
}