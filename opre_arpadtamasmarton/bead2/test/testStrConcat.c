#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

char* ConcatenateStrings(char *a, char *b)
{
	char *result = (char *) malloc(1 + strlen(a)+ strlen(b));
	strcpy(result, a);
    strcat(result, b);
	return result;
}


int main(void)
{
	char a[5];
	sprintf(a, "%d", 500);
	printf("result: %s\n", a);
	return 0;
}
