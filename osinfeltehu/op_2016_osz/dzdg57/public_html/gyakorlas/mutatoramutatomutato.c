#include <stdio.h>
#include <stdlib.h>

int main() {
	int x=5;
	int* pX = &x;
	int** pPX = &pX;
	printf("Ertek: %d\n", **pPX);
	return 0;
}