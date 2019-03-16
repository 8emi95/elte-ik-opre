#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	int value = 2;
	pid_t child = fork();	//ezzel lehet tobb folyamatot inditani pid_t
							//adja vissza a gyerekfolyamat PID-jet
	if (child < 0)
	{
		// error
	}
	else if (child > 0)
	{
		pid_t mypid = getpid(); // sajat processz ID-ja
		pid_t parentpid = getppid(); //visszaadja a szulo pidjet
		printf("En vagyok a parent: %i\n", mypid);
		printf("Parent Parentem: %i\n", parentpid); //szulo pidjet kiirni
		printf("Parent Childom: %i\n", child); //parent ki tudja iratni a childjat
	}
	else
	{
		pid_t mypid = getpid(); // sajat processz ID-ja
		pid_t parentpid = getppid(); //visszaadja a szulo pidjet
		printf("En vagyok a child: %i\n", mypid);
		printf("Child Parentem: %i\n", parentpid);
	}
	return 0;
}
