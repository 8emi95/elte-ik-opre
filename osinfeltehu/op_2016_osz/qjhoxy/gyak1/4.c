#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char** argv)
{
	int sum = 0, i;
	for (i = 1; i < argc; ++i) sum += strlen(argv[i]);
	printf("%d\n", sum);
	return 0;
}
