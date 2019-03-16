#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int main()
{
	char *s1 = "[MEGFIGYELO";
	char *s2 = "[MEGFIGYELO3]";

	int ret = strncmp(s1, s2, 10);
	printf("eredmeny: %d\n", ret);
	srand((unsigned) time(NULL));
	printf("Random: %d\n", (rand() % 2));
}
