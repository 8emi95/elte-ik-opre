#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes[12~
 char sz[30]="elso.c";
 char nev[50];
 int szamok[20]={1,2,3,4,5};
 printf("Hajra Fradi!\n");
 printf("Add meg a fajl nevet:");
 scanf("%s",nev);
 printf("A fajl neve: %s\n",nev);
 //int n=6,f=1,i=1;
 /*
 for(i=1;i<=n;i++)
  f*=i;
  printf("A %d faktorialis erteke: %d\n",n,f);
  printf("A szoveg hossza: %d",strlen(sz));
 */
 FILE* fp=fopen(nev,"r");
 if (fp==NULL) 
  {
   printf("Nincs ilyen fajl!\n");
   exit(1);
  }
 while (!feof(fp))
  {
    int kar=getc(fp);
    printf("A karakter:%c, Kodja:%d\n",kar,kar);
    
  }
  fclose(fp);
  return 1;
}