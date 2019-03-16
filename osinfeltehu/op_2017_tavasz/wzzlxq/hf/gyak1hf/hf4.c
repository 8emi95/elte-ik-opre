//Írjuk ki egymás után a program bemeneti paramétereit! Ugyanez fordított sorrendben.

#include <stdio.h>

int main(int argc, char *argv[]){
	int i = 1;
	while(i<argc){
		printf("%s\n", argv[i]);
		i++;
	}
	while(i>1){
		printf("%s\n", argv[i-1]);
		i--;
	}
}