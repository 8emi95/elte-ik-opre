#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
	/*
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]);
 }
 */
 
 //char *str1="";
 char *str1=malloc( sizeof(argv[1])+sizeof(argv[2]) );
 //char str1[80];
 
 //strcpy (str1,"");
 
 if (argc==3) 
 {
	printf("teszt: %s\n",str1); 
	 
	strcat(str1, argv[1]); 
	strcat(str1, argv[2]); 
	printf("a bemenetek osszeadva: %s\n",str1); 	
 }
 else
 {
	printf("nem megfeleloek a bemeno paramterek!\n"); 
 }	 
 
 
 return 0;
}