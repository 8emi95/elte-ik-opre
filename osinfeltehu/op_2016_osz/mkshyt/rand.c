#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
int main()
{ 
/*  fork();
  srand(getpid())); //the starting value of random number generation
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
  */
  char string1 [20];
  char string2 [20];
  scanf("%s", string1);
  fgets( string2,10,stdin);
  printf(string1);
  printf(string2);
  return 0; 
}