#include <stdio.h>
#include <stdlib.h>

int equal(char *str1,char *str2){
while(*str1){
if(*str1++!=*str2++)return 0;
}
return *str2==0?1:0;
}

int main(int argc,char ** argv) 
{
 if(argc-->=3){
  while(equal(argv[argc],argv[argc-1]) && argc-1)argc--;
  if(argc==1) printf("there are at least 2 arguments and their contents are equal \n");
 }
 return 0;
}
