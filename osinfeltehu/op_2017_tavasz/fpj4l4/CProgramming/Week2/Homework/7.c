#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int n;
  int i;

  char input[20];
  if(argc == 2) {
    strcpy(input, argv[1]);
    n = atoi(input);
  }

  for(i = 0;i<n;i = i + 1) {
    printf("hello\n");
  }

  return 0;
}
