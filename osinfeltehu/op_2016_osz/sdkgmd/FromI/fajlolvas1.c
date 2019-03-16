#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
 // ez egy egysoros megjegyzes
 char sz[30]="elso.c";
 char nev[50];
 int szamok[20]={1,2,3,4,5};
 printf("Add meg a fajl nevet:");
 scanf("%s",nev);
 // a scanf az elso helykozig, tab, enterig olvas
 char c=getchar();	// az enter beolvasása
 printf("A fajl neve: %s\n",nev);
 //teljes sor olvasasa, [] jelek kozott megadhato az
 // elfogadhato karakterek, a ^ karakter után a nem elfogadhato
 // karakter(ek) szerepelhetnek, \n ujsor
 printf("Adjon meg egy mondatot!\n");
 scanf("%[^\n]", sz);
 printf("A beírt mondat: %s\n",sz);
 // 
 int fp=open(nev,O_RDONLY);
 if (fp==-1) 
  {
   printf("Hiba, talan nincs ilyen fajl! A hiba kodja: %d \n", errno);
   exit(1);
  }
 while (read(fp,szamok,1))
  {
    
    printf("A beolvasott karakter:%c, Kodja:%d\n",szamok[0],szamok[0]);
    
  }
  close(fp);
  return 1;
}
