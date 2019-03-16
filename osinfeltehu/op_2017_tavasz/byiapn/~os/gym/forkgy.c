#include <sys/types.h>
#include <stdio.h>

int main()
{
	pid_t pid1, pid2;

	pid1 = fork();

	if(pid1)
	{
		pid2 = fork();
		if(pid2)
		{
			printf("children: child1=%i, child2=%i\n", pid1, pid2); 
		}
		else
		{
			printf("pid=%i, ppid=%i\n", getpid(), getppid());
		}

	}
	else
	{
		printf("pid=%i, ppid=%i\n", getpid(), getppid());
	}
}