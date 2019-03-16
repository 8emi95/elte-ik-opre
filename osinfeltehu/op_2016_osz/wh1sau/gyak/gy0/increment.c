#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
 int i;
 for(i=1; i<argc; i++)
 {
   int value = atoi(argv[i])+1;
   printf("%s increases to %i\n",argv[i],value);
 }  
 return 0;
}