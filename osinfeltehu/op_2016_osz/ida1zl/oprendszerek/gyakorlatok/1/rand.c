#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//véletlenszám generálás

int main()
{ 
  fork(); // még itt is egyformát ad
  //srand(time(NULL)); //the starting value of random number generation
  srand(getpid(); //ez biztosan különböző, innen indul a generálás
  //fork(); //mindig ugyanaz a két szám lesz, szülőben és gyerekben is
  int r=rand()%100; //number between 0-99
  printf("Random number %i\n",r);
  return 0;
}