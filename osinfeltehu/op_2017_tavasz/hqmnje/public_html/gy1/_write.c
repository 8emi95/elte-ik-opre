#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	int i;
	printf("Number of command line arguments are: %i\n",i,argv[i]);
	for(i=0;i<argc;i++){
		printf("%i. argument is %s\n",i,argv[i]);
	}
	return 0;
}