#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test(char* str)
{
	char * str2=str;
	while (*str!=0){	
		printf("%c\n",*str);
		str++;
	}
}

int main(){
char b[100]="Fradi";
printf("%s\n",b);
b[3]='K';
printf("%s\n",b);
int x = 5;
int* y;
y=&x;
printf("%i\n",*y);
char a[150]="Asfsdgasrzrbdkotbnubcfcgcfgjcv\0";
printf("%s\n",a);
test(a);
return 0;
}
