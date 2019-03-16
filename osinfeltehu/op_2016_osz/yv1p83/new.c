#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// Többszálú program C-ben:
int main()
{
	int value = 2;
	
	// új folyamat. A változók lemásolódnak.
	pid_t child = fork(); 
	if(child < 0)
	{
		// error
	}
	else if(child > 0)
	{
		pid_t mypid = getpid(); // így lehet megkapni a folyamat pid-jét
		pid_t parentpid = getppid(); //így az őt elindító folyamat pid-jét
		printf("En vagyok a parent: %i\n",mypid);
		printf("Parentem:: %i\n",parentpid);
		printf("childom: %i\n",child);
	}
	else
	{
		pid_t mypid = getpid(); // így lehet megkapni a folyamat pid-jét
		printf("En vagyok a parent: %i\n",mypid);
	}
	
	return 0;
}