#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


char* enter_question(void) {
  char *input;
  char *yn;
  printf("PLease enter question title:\n");
  while(1) { 
    scanf("%s",input);
    printf("Is input correct? yes/no");
    scanf("%s", yn);
    if(strcmp(yn,"yes") == 0) {
      break;
    }
    else {
      printf("Please reenter question.\n");
    }
  }
  
  
}


int main() {
  
  


}