#include <stdio.h>
#include <unistd.h>

int main()
{
	//gcc -Wall -Werror fork.c -o fork
	int x = 1;
	pid_t returnValue = fork();
	// fork returns twice and both blocks of code of the if and else get executed
	if(returnValue == 0) { 
	// only child process executes this
		printf("Child, x = %d, returnValue: %d\n", ++x, returnValue);
	} else {
		//only parent executes this
		printf("Parent, x = %d, returnValue: %d\n", --x, returnValue);
	}
	
	return 0;
}