#include <stdio.h>

int main(int argc, char** argv)
{
  char scanFInput[30];
  scanf("%s", scanFInput); // nem olvassa be az entert
  
  char* fgetsInput[30];
  fgets(fgetsInput, 30, stdin); // beolvassa az entert
  
  printf(scanFInput);
  printf(fgetsInput);
  
  return 0;
}