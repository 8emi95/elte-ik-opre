#include <stdio.h>
#include <string.h>

int main()
{
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes
 char sz[30]="Almafa";
 int szamok[20]={1,2,3,4,5};
 printf("Hajra Fradi!\n");
 int n=6,f=1,i=1;
 for(i=1;i<=n;i++)
  f*=i;
  printf("A %d faktorialis erteke: %d\n",n,f);
  printf("A szoveg hossza: %d",strlen(sz));
  return 1;
}