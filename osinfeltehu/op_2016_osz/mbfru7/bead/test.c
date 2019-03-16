#include <stdlib.h>
#include <stdio.h>

void foo (int* num){
	printf("num értéke: %d\n", *num);
}

int main() {
	int hehe = 10;
	foo(&hehe);

	return 0;
}
