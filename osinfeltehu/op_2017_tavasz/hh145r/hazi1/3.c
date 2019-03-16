#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
 if(argc--==3){
  printf("%s \n",  strcat(argv[argc-1], argv[argc]));
 }
 return 0;
}