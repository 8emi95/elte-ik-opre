#include <stdio.h>
#include <sys/types.h>

int main()
{
	pid_t pid1;
	pid_t pid2;

	pid1 = fork();

	if(pid1>0)
	{
		pid2 = fork();
	}

	if(pid1>0 && pid2>0)
	{
		printf("szülő getpid = %i, elso gyerek pid-je: %i, masodik gyerek pid-je %i\n", getpid(), pid1, pid2);
	}else
	{
		printf("gyerek getpid = %i, szülő getppid=%i\n", getpid(), getppid());
	}
}
