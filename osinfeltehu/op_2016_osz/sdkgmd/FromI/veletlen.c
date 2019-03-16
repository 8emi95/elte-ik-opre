#include <stdio.h>
#include <stdlib.h>

void main()
{
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes
 printf("Hajra Fradi!\n");
 srandom(time(NULL));
 int x=random();
 printf("A veletlen szám: %d, a maximum: %d\n",x,RAND_MAX);
 srandom(time(NULL));
 x=random();
 printf("A veletlen szám: %d, a maximum: %d\n",x,RAND_MAX);
}
