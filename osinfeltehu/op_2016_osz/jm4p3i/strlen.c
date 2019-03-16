#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length(char * str) {
	int l=0;
	while(*str++!=0)l++;
	return l;
}

int strequals(char * a, char * b) {
	if(length(a) != length(b)) return 0;
	
	int equals = 1;
	int l = 0;
	while(*a!=0 && *b!=0) {
		if(a[l] != b[l]) {
			return 0;
		}
		l++;
	}
	return 1;
}
// ./a.out arg1 arg2 etc 
// 0. argument -> a.out
// 1. argument -> arg1
//int main(int argc, char** argv) {
int main() {
	char str1[]="Hello world\0";
	char str2[]="!!";
	/* args kiirasa
	int i;
	for (i=0;i<argc;i++) {
		printf("%i. argument is %s\n",i, argv[i]);
	}
	*/
	//str1 + str2;
	str1[0] == 'H';
	str1[0] = 'A'; //karakter felulirva
	str1[1] = '\0'; //hamarabb lezrja
	//char * str;
	//printf("Hello world %s\n", str1);
	char tmp_str1[] ="alma";
	char tmp_str2[] ="alma";
	
	/*printf("%s es %s ", tmp_str1, tmp_str2);
	if(strequals(tmp_str1,tmp_str2) == 1) printf("megegyeznek");
	else printf("nem egyeznek meg");
	*/
	/*
	str2 = (char*)malloc(80*sizeof(char)); // mem allocation
	free(str2); //memoria felszabaditasa
	
	*/
	
	printf("\n");
	return 0;
}