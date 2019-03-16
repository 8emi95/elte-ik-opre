//Ellenõrizzük, hogy egy programnak pontosan két bemenõ paramétere van,
//majd írjuk ki ezeknek konkatenáltját!
//szerintem közel ugyanaz mint egy korábbi, szólj ha tévedek

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