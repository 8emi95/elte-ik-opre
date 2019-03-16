#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length(char * str)
{
	int l=0;
	while (*str++!=0)l++;
	return l;
}
int main()
{
	char str1[]="Hello world";
	printf("The content of variable str1: \'%s\'\n",str1); 

	printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n",str1,length(str1),strlen(str1));

	char* str2;
	str2=str1;
	printf("The content of variable str2 \'%s\' is the same as str1 \'%s\' \n(pointing the same memory place)\n",str2,str1);
	str2=(char*)malloc(80*sizeof(char));
	str2[0]='O';
	str2[1]='S';
	str2[2]=0;
	printf("We've given value character by character -  str2: \'%s\'\n",str2);

	strcpy(str2,"New content by using strcpy");   
	printf("The new content of variable str2: \'%s\'\n",str2);

	free(str2);
	
	return 0;
}
