#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  char word1[20];
  char word2[20];
  //char buffer[100] = { 0 };
  char textToChange[] = "hey my name is john, bye and hey again!";

  if(argc == 3) {
    if(strlen(argv[2])>strlen(argv[1])) {
      strncpy(word1, argv[2], sizeof word1 - 1);
      strncpy(word2, argv[1], sizeof word2 - 1);
    }
    else {
      strncpy(word1, argv[1], sizeof word1 - 1);
      strncpy(word2, argv[2], sizeof word2 - 1);
    }
  }
  
  printf("%s  :  %s \n", word1, word2); 
  int i;
  int j = 0;
  for( i = 0;i<strlen(textToChange);i = i + 1) {
    if(textToChange[i] == word2[j]) {
      j++;    
    }
    else j = 0;
    if(j == strlen(word2)) {
      int p;
      int q = 0;
      for(p = i-j+1;p<i+1; p = p + 1)  {
        textToChange[p] = word1[q];
        q++;
        
      }
    } 
  }

  printf("%s \n", textToChange);
  return 0;

}
