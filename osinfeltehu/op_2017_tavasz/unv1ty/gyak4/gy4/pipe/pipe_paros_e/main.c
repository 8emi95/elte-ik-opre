#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//szülő küld 1 számot pipe-on gyereknek, aki visszaküldi másik pipeon hogy páros-e

int main(void)
{
  int n, k, pfd1[2], pfd2[2];
  printf("kerem a szamot:\n");            /* az n bekérése */
  scanf("%d", &n);
  pipe(pfd1);                             /* a két pipe fájl létrehozása */
  pipe(pfd2);
  if (fork())                             /* itt be lehet tenni még hibaellenőrzést is */
  {                                       /* szülő */
    close(pfd1[0]);                       /* fölösleges pipe-ok bezárása */
    close(pfd2[1]);
    write(pfd1[1], &n, sizeof(int));      /* az n elküldése a pfd1 pipe-on keresztül */
    close(pfd1[1]);
    read(pfd2[0], &k, sizeof(int));       /* az eredmény kiolvasása a pfd2-n keresztül */
    if (k == 0)
      printf("a szam paros\n");           /* az eredmény kiírása */
    else
      printf("a szam paratlan\n");
    exit(0);
  }
  else                                    /* gyerek */
  {
    close(pfd1[1]);
    close(pfd2[0]);
    read(pfd1[0], &n, sizeof(int));       /* az n kiolvasása a pfd1 pipe-ból */
    close(pfd1[0]);
    k = n%2;                              /* k=1, ha az n páratlan, k=0, ha n páros */
    write(pfd2[1], &k, sizeof(int));      /* az eredmény elküldése a pfd2 pipe-on */
    close(pfd2[1]);
    exit(0);
  }
}


