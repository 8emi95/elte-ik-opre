#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if(argc == 2) {
    printf("%s was given \n", argv[1]);
  }
  else if(argc == 3) {
    printf("%s, and %s was given \n", argv[1], argv[2]);
  }
  else if(argc < 2) {
    printf("not enough arguments given \n");
  }
 else
    printf("too many arguemnts given \n");


  return 0;
}
