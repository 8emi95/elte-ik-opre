#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 char s[1024]="";	
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]);
  sprintf(s, "%s%s",s, argv[i]);
 }
 printf(" %s",s);
 return 0;
}