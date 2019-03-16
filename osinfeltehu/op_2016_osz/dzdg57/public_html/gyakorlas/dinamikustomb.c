#include <stdio.h>
#include <stdlib.h>

void scantomb(int tomb[], const int size);
void printtomb(int tomb[], const int size);

int main() {
	const int size = 10;
	int* tomb = (int*) malloc(sizeof(int)*size);
	
	scantomb(tomb,size);
	printtomb(tomb,size);
	
	free(tomb);
	return 0;
}

void scantomb(int tomb[], const int size) {
	int i;
	for(i=0; i<size; i++) {
		// !!!
		scanf("%d", &tomb[i]);
	}
	return;
}

void printtomb(int tomb[], const int size) {
	int i;
	for(i=0; i<size; i++) {
		printf("tomb[%d] = %d\n", i, tomb[i]);
	}
	return;
}