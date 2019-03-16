#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
  char* str;
  str = (char*)malloc(80*sizeof(char));
  scanf("", str);
  printf("%s \n", str);
  //fgets(str);
  //printf("%s \n", str);
  return 0;
}