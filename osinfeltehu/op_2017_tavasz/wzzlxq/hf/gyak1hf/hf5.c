//Írjuk ki, egy program bemeneti paramétereinek hosszát és összegét!

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	int h = 0;
	int i = 1;
	while(i<argc){
		int k=strlen(argv[i]);
		h=h+k;
		printf("%d |", k);
		i++;
	}
	i = 1;
	h = h * (sizeof(char*));
	char* out = (char*)malloc(h);
	while(i<argc){
		strcat(out, argv[i]);
		i++;
	}
	printf("%s",out);
	free(out);
}