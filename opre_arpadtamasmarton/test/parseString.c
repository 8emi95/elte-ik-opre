#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


int main ()
{
	// printf("HERE\n");
	
	const char *cities[8] = {"BUDAPEST", "DEBRECEN", "MISKOLC", "SOPRON", "GYOR", "SZEGED", "ETYEK", "PECS"};
	printf("-%s-\n", cities[6]);
	
	char test[] = "MEGFIGYELO1;ETYEK;0;0;0;2015:05:11 02:37:23";
	
	char *result = strtok(test, ";");
	while(result != NULL)
	{
		printf("-%s-\n", result);
		result = strtok(NULL, ";");
	}
	
	return 0;
}
