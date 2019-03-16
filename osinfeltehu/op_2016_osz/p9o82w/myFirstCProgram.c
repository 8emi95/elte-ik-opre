#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length(char* str) {
	int l = 0;
	while(*str++ != 0) {
		l++;
	}	
	return l;
}

// strcmp 
int compare(char* s1, char* s2) {
	while( s1!=0 || s2!=0) {
		if( s1 != s2) {
			return 1;
		} else {
			s1++;s2++;
		}
	}
	return 0;
}

// 3. Feladat
// Foglaljunk le egy területet egy char*-nak, 
// adjuk értékül egy másiknak majd ellenőrizzük hogy nem ugyanoda vannak foglalva

void feladat3() {
	char* string1 = malloc(sizeof(char)*5);
	char* string2 = malloc(sizeof(char)*5);
	strcpy(string1, string2);
	
	printf("%i", &string1);
	printf("\n");
	printf("%i", &string2);
	printf("\n");
	printf("%i", &string1 == &string2);
	
    free(string1);
	free(string2);
}

// 4. Feladat Irjuk ki egymás után a program bemeneti paramétereit! Ugyanez fordított sorrendben

void feladat4(int argc, char** argv) {
	int i = 0;
	while(i<argc) {
		printf("%s", *argv++);
		printf("\n");
		i++;
	}
}
void feladat4b(int argc, char** argv) {
	int i = argc-1;
	while(i >= 0) {
		printf("%s", *(argv+i));
		printf("\n");
		i--;
	}
}

// 5. Feladat Irjuk ki egy programnak a bemeneti parametereinek a hosszanak az osszeget
void feladat5(int argc, char** argv) {
	int sum = 0;
	int i = 0;
	while(i < argc) {
		sum+=strlen(*argv++);
		i++;
	}
	printf("%i", sum);
}

// 6. Feladat Irjuk ki, ha egy programnak legalabb ket parametere van, es ezek megegyeznek
void feladat6(int argc, char** argv) {
	if(argc >= 2) {
		if(strcmp(*(argv+2), *(argv+3))) {
			printf("----------\n");			
			printf("%i", strcmp(*(argv+1), *(argv+2)));
			printf("A paramterek megegyeznek");
		}
	}
}

int main(int argc, char** argv) {
	char* str1 = "Hello world";
	char* str2 = "!!!";	
		 
	printf("%i", length(str1));
	printf("\n");
	feladat3();
	printf("\n");
	feladat4(argc, argv);
	feladat4b(argc, argv);
	feladat5(argc, argv);
	feladat6(argc, argv);
	
	return 0;
	
}