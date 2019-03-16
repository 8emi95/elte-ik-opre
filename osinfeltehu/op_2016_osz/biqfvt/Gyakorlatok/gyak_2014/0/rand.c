#include <stdio.h>
#include <stdlib.h> //srand
#include <time.h>

int main()
{ 
  srand(time(NULL)); //the starting value of random number generation (use seconds)
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}