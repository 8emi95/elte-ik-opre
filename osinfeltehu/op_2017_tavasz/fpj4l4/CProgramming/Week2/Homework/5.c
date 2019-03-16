#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* removeSpaces(char input[]) {
  char* output = input; 
  while(*output == ' ') {
    output++;
  } 
  return output;
}


int main() {
  char word[80];
  printf("Give a word: \n");
  scanf("%s",word);

  char* firstChar = removeSpaces(word);  
  printf("%s \n",firstChar);
  return 0;
}
