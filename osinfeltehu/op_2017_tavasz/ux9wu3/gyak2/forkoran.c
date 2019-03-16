#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  int ori = getpid();
  pidfork();
  if (ori==getpid()) fork();
  printf("f");
  waitpid()
  return 0;
}
