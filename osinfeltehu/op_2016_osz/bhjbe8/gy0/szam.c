#include <stdio.h>
#include <stdlib.h>
 

int main(int argc,char ** argv) 
{
	int i;

	for(i=1;i<argc;i++)
	{
		printf("%i -> %i \n",atoi(argv[i]), atoi(argv[i])+1);
	}
return 0;
}