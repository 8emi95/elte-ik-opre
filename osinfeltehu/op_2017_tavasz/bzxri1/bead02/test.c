#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "unistd.h"
#include "string.h"

int main()
{
	char t1[1][2][3];
	//t1[0][1][1] = "asd";
	char t2[1][2][3];
	//t2 = t1;
	//strcpy(t2,t1);
	printf("%d",sizeof(t1[0]) / sizeof(char));
	//printf(t2[1][2]);
}
