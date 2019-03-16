#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char ** argv)
{
	int len = 0;
	int i;
	for(i=1;i<argc;i++)
	{
		len = len + strlen(argv[i]);
	}
	
	printf("%d\n",len);
	
	return 0;
}