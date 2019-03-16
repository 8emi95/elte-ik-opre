#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%i. arguments is %i\n", i, 2 * atoi(argv[i]));
	}
	return 0;
}
