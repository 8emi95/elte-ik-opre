#include <stdio.h>
#include <time.h>

int main ()
{
	int i = 5;
	int *p = &i;
	*p = 0;
	printf("%i", *p);
}