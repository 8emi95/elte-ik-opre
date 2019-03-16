//gcc arg.c
//gcc arg.c -o arg.exe

//C ben string helyett karaktertombok vannak
//ezert char* lenne a karaktertomb
//	 es a char** a karaktermutatokra mutato mutatok tombje,
//	 mivel tobb parameter van

//kiiro fuggveny	printf
//elso parameter %i ugy kell tekinteni mint egy egesz szamot
// %s



#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
	printf("Number of command line arguments are: %i\n",argc);
	for (i=0;i<argc;i++){
		printf("%i. argument is %s\n",i,argv[i]);
	}
 return 0;
}