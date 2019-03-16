#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
	int i;
	
	printf("The magic number is: %i\n", (atoi(argv[1]) + 1));
	
	return 0;
}