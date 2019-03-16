#include <stdio.h>

int main(int argc, char *argv[]){
	int k;
	k = 24;
	int sznum = 4; 
	char c = 'c';

	float mynum = 24.5f;
	printf("Hello Word!\n");
	printf("Value of k = %d and sznum = %d\n", k, sznum);
	printf("This is a char: %c\n", c);
	printf("This is the value of the char: %d\n", c);
	printf("Mynum = %f\n", mynum);

	printf("Size of int = %d\n", sizeof(int));
	return 0;
}
