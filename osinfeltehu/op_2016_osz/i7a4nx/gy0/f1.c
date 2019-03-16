//gcc arg.c
//gcc arg.c -o arg.exe

//C ben string helyett karaktertombok vannak
//ezert char* lenne a karaktertomb
//	 es a char** a karaktermutatokra mutato mutatok tombje,
//	 mivel tobb parameter van

//kiiro fuggveny	printf
//elso parameter %i ugy kell tekinteni mint egy egesz szamot
// %s szovegkent kezeljuk


#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 i = atoi(argv[1]);
 i = i+1;
	printf("%i", i);

 return 0;
}