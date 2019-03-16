#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>




void doSomeWork(char *name)
{
	
	//hello
	const int NUM_TIMES = 5;

	int i = 0;
	for(i; i < NUM_TIMES; i++)
	{
		sleep(rand() % 4);
		printf("Done pass %d for %s\n",i, name);
	} 
}

int main(int argc, char *argv[])
{
	printf("I am: %d \n", (int) getpid());



	pid_t pid = fork();
	srand((int)pid);
	printf("fork returned: %d\n", (int) pid);

	if (pid == 0)
	{
		printf("I am the child with pid :%d\n", (int) getpid());
		doSomeWork("Child");
		
		exit(42);
	} 
	

	//We must be the parent cos of exit(0)
		printf("I am the parent waiting for child to end.\n");
		doSomeWork("Parent");


		int status = 0;
		pid_t childpid = wait(&status);  //Will wait for the child to end! 
		printf("Parent knows child %d is finished with status %d.\n", (int) childpid, status);

		int childReturnValue = WEXITSTATUS(status);//Will mask off only the return value of the child exit thing
		printf("Retun value was %d\n",childReturnValue );

	return 0;
}



