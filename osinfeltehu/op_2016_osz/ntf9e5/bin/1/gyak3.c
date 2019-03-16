#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){
	int i;
	for (i = 0; i < argc; ++i){
		printf("%i. argument: %s\n", i, argv[i]);
	}
	
	for (i = argc-1; i != -1; --i){
		printf("%i. argument: %s\n", i, argv[i]);
	}
	
	//strlen("asd");
	int l;
	char* str;
	str = "alma";
	l = strlen(str);
	printf("length: %i\n", l);
	
	int sum;
	for (i = 0; i < argc; ++i){
		sum += strlen(argv[i]);
		printf("%i. argument: %s\n", i, argv[i]);
	}
	printf("Lenght of arguments: %i\n", sum);
		
		
	if (argc >= 3){
		printf("Has 2 arguments\n");
		for (i = 1; i<argc-1; ++i){
			strcmp(argv[i],argv[i+1]);
		}
	}
	
	if (argc == 3){
		
		printf("Exactly 2 params");
		char* strconcat;
		strconcat = 
		
		
	}
	return 0;
}