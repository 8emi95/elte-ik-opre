#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
	int i;
	for (i=0; i < atoi(args[1]); i++){
		printf("%s\n",args[0]);
	}

 return 0;
}