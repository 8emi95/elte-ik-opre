#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// http://os.inf.elte.hu/~pzh5fq/elso_gyak/
int length(char * str) {
	int l=0;
	while(*str++!=0)l++;
	return l;
}

//int main(int argc, char** argv) {
int main() {
	char * str1 = (char*)malloc(20*sizeof(char));
	char * str2 = str1;
	
	printf("%s - %s", str1, str2);
	//if(str1 == str2) printf("Ugyanoda foglalt");
	//else printf("Mashova foglalt");
	
	printf("\n");
	return 0;
}