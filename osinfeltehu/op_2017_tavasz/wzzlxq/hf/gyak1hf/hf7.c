//Ellen�rizz�k, hogy egy programnak pontosan k�t bemen� param�tere van,
//majd �rjuk ki ezeknek konkaten�ltj�t!
//szerintem k�zel ugyanaz mint egy kor�bbi, sz�lj ha t�vedek

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc==3){
		int h=strlen(argv[1])+strlen(argv[2]);
		char *out = (char*) malloc(h*sizeof(char*));
		strcat(out, argv[1]);
		strcat(out, argv[2]);
		printf("2 db arg van ezek osszege: %s\n", out);
		free(out);
	}
	else{printf("Nem 2 db arg van");}
}