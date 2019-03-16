#include <stdio.h>
#include <stdlib.h>
int main()
{
	char* asd;
	asd=(char*)malloc(30);
	char* hehe=*asd;
	printf("%d\n",hehe==asd);
	return 0;
}

