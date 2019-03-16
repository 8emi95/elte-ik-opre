#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc, char **argv) 
//char** means an array of character arrays = array of strings
{
	printf("Number of command line arguments are: %i\n", argc);
	
	int i;	
	for (i = 1; i < argc; i++)
		printf("%d\n", atoi(argv[i]));
	
	return 0;
}
