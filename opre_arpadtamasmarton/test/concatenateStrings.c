#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

char* concatenateStrings(int argc, char *argv[])
{
	int i, length = 0;
	for(i = 0; i < argc; i++)
	{
		length += strlen(argv[i]);
	}
	
	char *result = (char*) malloc(length + 1);
	for(i = 0; i < argc; i++)
	{
		strncat(result, argv[i], strlen(argv[i]));
	}
	strncat(result, "\0", 1);

	return result;
}

int main()
{
	char *temp[3] = {"alma", "korte", "barack"};
	char *tmp = concatenateStrings(3, temp);
	tmp = concatenateStrings(4, temp);
	printf("Ennyi: %s\n", tmp);
	return 0;
}
