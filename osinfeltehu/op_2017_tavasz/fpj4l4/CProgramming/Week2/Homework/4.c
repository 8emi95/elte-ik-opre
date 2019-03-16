#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char word[80];
  size_t size = sizeof(word)/sizeof(word[0]);
  int cnt = 0;
    
  printf("Enter a word: ");
  scanf("%s", word); 

  while((word[cnt]) != '\0') {
    cnt++;
  }   

  //int length = strlen(word);

  printf("Length of word is %d \n",cnt);
  printf("size: %d \n", size); 
  return 0;
}
