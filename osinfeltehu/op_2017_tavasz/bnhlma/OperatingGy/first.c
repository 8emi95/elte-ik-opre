#include <stdlib.h>
#include <stdio.h>
int main(int argc,char** argv){
  int i;
  for(i=0;i<argc;i++){
  printf("the number of arguments is %i\n",argc);
  printf("the %i argument is %s \n",i,argv[i]);
  }
  return 0;
}
