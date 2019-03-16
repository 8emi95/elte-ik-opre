#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{ 
  srand(time(NULL)); //the starting value of random number generation
  // rand generates number between 0 and RAND_MAX (~32768)
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}
