#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char c1[10];
	char c2[10];
	fgets(c1,10,stdin);
	scanf("%s",c2);
	//fgets(c1,10,stdin);
	
	printf("%i\n",strlen(c1));
	printf("%i\n",strlen(c2));
}