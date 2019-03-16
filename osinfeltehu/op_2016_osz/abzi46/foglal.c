#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
	char * str1;
	str1=(char*)malloc(80*sizeof(char));
	char * str2;
	//char * str2 = str1;
	strcpy(str2, str1);
	
	if(*str1 == *str2){
		printf("asd\n");
	}
	
	free(str1);
	
	return 0;
	
}