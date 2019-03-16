#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int repCount = atoi(argv[2]);
	for(int i=0; i<repCount; ++i)
	{	
		printf("%s\n", argv[1]);
	}
	return 0;
}