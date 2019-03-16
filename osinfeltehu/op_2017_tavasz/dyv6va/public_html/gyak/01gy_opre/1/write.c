#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int i;
	printf("number of command line arguments are: %i\n", argc);
	for( i = 0; i<argc; i++)
	{
		printf("%i argument is %s\n", i, argv[i]);
	}
	int numberOfChildren = 2, v;
	for(v = 0; v<numberOfChildren; v++)
	{
		pid_t child = fork();
		if (child == 0) //child
		{
			int j;
			for(j = 0; j<10; j++)
				printf("PID: %i, Loop variant: %i, Random number:%i\n", getpid(), j, rand());
		}
		else //parent
		{
			int status;
			waitpid(child, &status, 0);
			printf("My children are dead!\n");
		}
	}
}