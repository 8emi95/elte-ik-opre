#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char word[80];

  printf("Enter a word: ");
  scanf("%s", word);  

  int length = strlen(word);
  printf("Length of string is %d \n ",length);

  return 0;
}
