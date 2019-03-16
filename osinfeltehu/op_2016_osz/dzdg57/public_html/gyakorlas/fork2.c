#include <stdio.h>
#include <unistd.h>

int main()
{
	int x = 1;

	// fork returns twice and both blocks of code of the if and else get executed
	if(fork() == 0) { 
	// only child process executes this
		printf("Child, x = %d\n", ++x);
	} else {
		//only parent executes this
		printf("Parent, x = %d\n", --x);
	}
	// parent and child execute this
	printf("Exiting with x = %d\n", x);
	
	return 0;
}