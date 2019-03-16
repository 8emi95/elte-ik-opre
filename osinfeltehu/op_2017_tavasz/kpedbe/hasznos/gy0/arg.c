#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
     int num = atoi(argv[i]);
     printf("%i-nek 2szeres: %i\n",num,num*2);
 }
 return 0;
}
