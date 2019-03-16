#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{ 
  fork();
  // srand-nak paraméterként ne az időt adjuk!! Mert az olyan gyorsan lefut, hogy a gyerek is ugyan azt a "random " számot fogja generálni
  
  srand(getpid()); //the starting value of random number generation
  
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}