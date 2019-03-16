#include "stdio.h"
#include "stdlib.h"

int main()
{
	char* str1 = (char*)malloc(10);
	char* str2 = str1;
	printf("%d\n", str1==str2);

}
