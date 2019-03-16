#include <stdio.h>

int main()
{
   printf("1) Kerdessor megadasa /n");
   printf("2) Uj kerdes felvetele /n");
   printf("3) Kerdes modositasa /n");
   printf("4) Kerdes torlese/n");
   printf("5) Kerdesek listazasa/n");
   printf("6) Kerdes veglegesitese/n");
   printf("q) Kilepes /n");
   printf("/n");
   printf("/n");
   printf("/n");
   
   char str[3];
   scanf("%s", str);
   
   int tmp = atoi(str);
   switch(tmp) {
   case 1: printf("Kerdessor megadasa /n"); break;
   case 2: printf("Uj kerdes felvetele /n"); break;
   case 3: printf("Kilepes /n"); return 0; break;
   default: printf("Ilyen menupont nincs /n");
   }
   
   
   
   return 0;


}