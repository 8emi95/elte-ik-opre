#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int IntComparator(const void *aa, const void *bb)
{
    const int *a = aa, *b = bb;
    return (*a < *b) ? -1 : (*a > *b);
}

int main(void)
{
	int countBikes = 3;
	int bikeIds[] = {4,12,1};
	qsort(bikeIds, countBikes, sizeof(int), IntComparator);
	printf("%d, %d, %d", bikeIds[0], bikeIds[1], bikeIds[2]);
	return 0;
}
