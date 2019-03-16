#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  int i;
  for (i = 1; i < argc; i++)
  {
    printf("%i\n", atoi(argv[i])*2);
  }
  return 0;
}