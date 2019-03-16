#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
 int i=0;
 while(--argc){
  while(*argv[argc]++){
   i++;
  }
 }
 printf("arguments' size is %i\n",i);
 return 0;
}
