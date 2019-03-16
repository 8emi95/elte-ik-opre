#include <stdio.h>
#include <stdlib.h>


int main(int argc,char ** argv) 

{
	char c1 [10];
	char c2 [10];
	fgets(c1,10,stdin);
	scanf("%s",c2);
	
	printf("%s\n",c1);
	printf("%s\n",c2);
	
	return 0;
	
}


