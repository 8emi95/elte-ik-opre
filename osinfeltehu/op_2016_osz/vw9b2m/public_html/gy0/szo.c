#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 

{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 
 for (i=0;i<argc;i++){
	int out=0;
	out = atoi(argv[i])+1;
	printf("%i. argument is %s.,%i\n",i,argv[i],out);
 }
 return 0;
 //passwd régi jelszo ketszer uj
}