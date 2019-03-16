#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{
	fork();
	
	srand(getpid());
	int r = rand() % 100;	
	printf("Random number %i\n", r);
	
	return 0;
}