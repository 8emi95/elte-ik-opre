#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int dice(int sides) {
  return rand()%sides+1;
}

int main()
{ 
  srand(time(NULL)); //the starting value of random number generation
  printf("Random number %i\n",dice(6)+dice(6));
  return 0;
}
