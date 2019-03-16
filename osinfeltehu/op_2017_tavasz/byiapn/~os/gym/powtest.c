#include <math.h>
#include <stdio.h>

void main(void){
	double k; int l;
	double i = 2;
	//while(i <= 8){
		k = pow(10, i);
		printf("using pow basically:\nk = pow(10, %f) => %f\n", i, k);
		printf("converted into int after:\n(int)k => %d\n", (int)k);
		l = (int)pow(10, i);
		printf("\nusing pow with (int) auto-conversion:\nl = (int)pow(10, %f) => %d\n", i, l);
	//i = i * 2;}
}