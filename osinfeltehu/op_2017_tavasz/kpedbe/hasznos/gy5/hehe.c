#include <stdio.h>

int main() {
	char c = 'c';
	char * p = &c;
	while(1) {
		printf("%c",*p);
		p++;
	}
	return 0;
}