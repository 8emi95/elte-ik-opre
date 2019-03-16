#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{ 
  srand(time(NULL)); //the starting value of random number generation
  int r1=rand()%6+1; //number between 0-99
  int r2=rand()%6+1;
  printf("Random number %i\n",r1+r2);
  return 0;
}