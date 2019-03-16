#include <stdio.h>
#include <stdlib.h>

int main() {
	// a malloc void* -ot ad vissza, amit int* -gá konvertálunk
	int * x = (int*) malloc(sizeof(int));
	// bekérünk egy int-et:
	scanf("%d", x);
	printf("A szam: %d\n", *x);
	
	// a malloc-kal lefoglalt dolgokat mindig fel kell szabadítani a program végén a free() függvénnyel
	free(x);
	return 0;
}