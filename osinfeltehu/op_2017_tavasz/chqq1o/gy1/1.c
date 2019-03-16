#include <stdio.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();

	printf("pid=%i getpid=%i getppid=%i\n", pid, getpid(), getppid());
}
