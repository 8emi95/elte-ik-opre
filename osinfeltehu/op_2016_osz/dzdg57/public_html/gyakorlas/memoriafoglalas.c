#include <stdio.h>
#include <stdlib.h>

int main() {
	// a malloc void* -ot ad vissza, amit int* -g� konvert�lunk
	int * x = (int*) malloc(sizeof(int));
	// bek�r�nk egy int-et:
	scanf("%d", x);
	printf("A szam: %d\n", *x);
	
	// a malloc-kal lefoglalt dolgokat mindig fel kell szabad�tani a program v�g�n a free() f�ggv�nnyel
	free(x);
	return 0;
}