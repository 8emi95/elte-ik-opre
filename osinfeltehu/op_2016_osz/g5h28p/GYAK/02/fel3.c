#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
int main(void)
{
  time_t now;
  time(&now);
 
  printf("%s", ctime(&now));
 
  return EXIT_SUCCESS;
}