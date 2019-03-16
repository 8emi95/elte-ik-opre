#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	pid_t child1 = fork();	//ezzel lehet tobb folyamatot inditani pid_t
							//adja vissza a gyerekfolyamat PID-jet
	if (child1 < 0)
	{
		
	}
	else if (child1 > 0)
	{
		pid_t second_child = fork(); //ha kivulre teszem akkor 4 gyerek lesz
		pid_t mypid = getpid(); // sajat processz ID-ja
		printf("En vagyok a parent: %i\n", mypid);
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
