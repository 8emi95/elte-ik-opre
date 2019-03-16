#include <stdio.h>
#include <unistd.h>

int main()
{
	int status;
	int notacommonvalue = 1;
	printf("The value is %i before forking.\n", notacommonvalue);
	
	pid_t child1 = fork();

	if (child1 == 0)
	{
		notacommonvalue = 11;
		printf("The value is %i in process child1.\n", notacommonvalue);
	}
	else if (child1 > 0)
	{
		pid_t child2 = fork();
		
		if (child2 == 0)
		{
			notacommonvalue += 21;
			printf("The value is %i in process child2.\n", notacommonvalue);
		}
		else if (child2 > 0)
		{
			waitpid(child1, &status, 0);
			waitpid(child2, &status, 0);
			printf("The value is %i in parent process (remain the original).\n", notacommonvalue);
		}
	}
}