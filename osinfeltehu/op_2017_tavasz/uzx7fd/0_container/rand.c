#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main() {
	srand(time(NULL));
	int r = rand() % 100;
	printf ("%i\n",r);
}