#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length(char * str) {
	int l=0;
	while(*str++!=0)l++;
	return l;
}
// ./a.out arg1 arg2 etc 
// 0. argument -> a.out
// 1. argument -> arg1
int main(int argc, char** argv) {
	int i,l; 
	l=0;
	for(i=0;i<argc;i++) {
		printf("%i. argumentum: %s\n", i, argv[i]);
		l+= length(argv[i]);
	}
	for(i=argc-1;i>=0;i--)
		printf("%i. argumentum: %s\n", i, argv[i]);
	
	printf("\n");
	printf("Az osszes argumentum hossza: %i\n", l);
	
	int argsequal = 1;
	if(argc >= 2) {
		for(i=2; i < argc; i++) {
			if(strcmp(argv[1], argv[i])!=0) {
				argsequal = 0;
				break;
			}
		}
	}
	if(argsequal == 1) printf("Argumentumok megegyeznek! \n");
	else printf("Argumentumok nem egyeznek meg! \n");
	
	if(argc==2) printf("%s%s", argv[1],argv[2]);
	printf("\n");
	printf("\n");
	return 0;
}
