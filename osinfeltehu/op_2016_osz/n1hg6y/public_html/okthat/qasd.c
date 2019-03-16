#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
	int value=2;
	pid_t child=fork();
	if (child <0)
	{
		//error
	}
	else if(child> 0)
	{
		/*
		pid_t mypid=getpid();
		pid_t parentpid=getppid();
		printf("En vagyok a parent(kft)%i\n",mypid);
		printf("(parent)parentem: %i\n",parentpid);
		printf("(parent:Childom: %i\n",child);
		*/
		pid_t child2=fork();
	}
	else
	{
		/*
		pid_t mypid =getpid();
		pid_t parentpid =getppid();
		printf("En vagyok a child: %i\n",mypid);
		printf("(Child) Parentem: %i\n",parentpid);
		*/
	}
	pid_t mypid =getpid();
	pid_t parentpid =getppid();
	printf("ez vagyok en: %i  ,ez meg a papi %i\n",mypid,parentpid);
	return 0;
}