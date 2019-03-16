#include <stdio.h>

int main(int n, char ** argv){
	int i;
	for(i=1;i<n;++i)
	{
		printf("%s ",argv[i]);
	}
	printf("\n");
	return 0;
}