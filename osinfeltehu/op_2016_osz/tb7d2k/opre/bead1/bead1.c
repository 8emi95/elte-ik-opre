#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

int hossz(char seged[]);
int sorszam();

void vjel()
{
  int handle;
  handle = open("./vendeg.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  lseek(handle, 0, SEEK_END);
  if (handle < 0)
  {
      printf("Fajl hiba.\n");
      return;
  }
  char buffer[80];
  
  printf("\nAdja meg a nevet: ");
  fgets(buffer, sizeof(buffer), stdin);
  write(handle, &buffer, hossz(buffer));
  write(handle, " ", 1);
  printf("Adja meg a mail cimet: ");
  fgets(buffer, sizeof(buffer), stdin);
  write(handle, &buffer, hossz(buffer));
  write(handle, " ", 1);
  printf("Adja meg a rendezveny azonositojat: ");
  fgets(buffer, sizeof(buffer), stdin);
  write(handle, &buffer, hossz(buffer));
  write(handle, " ", 1);
  time_t most = time(0);
  write(handle, ctime(&most), 24);
  write(handle, "\n", 1);
  int sszam = sorszam();
  printf("\nA megadott vendeg sorszama: %i\n\n", sszam);
  close(handle);
}

int sorszam()
{
   FILE * handle;
   handle = fopen("vendeg.txt", "r");
   if (handle == NULL)
   {
      return 0;
   }
   char buffer[80];
   int i = 0;
   while(fgets(buffer, sizeof(buffer), handle) != 0)
   {
	   for(int j = 0; j < 80; j++)
	   {
		   if(buffer[j] == '\n')
		   {
			   i++;
		   }
	   }	   
   }
   fclose(handle);
   return i;
}

int hossz(char seged[])
{
  int i = 0;
  while(seged[i] != '\n')
  {
	i++;
  }
  return i;
}

void jellist()
{
   FILE * handle;
   handle = fopen("vendeg.txt", "r");
   if (handle == NULL)
   {
      return;
   }
   char buffer[80];
   printf("\n");
   while(fgets(buffer, sizeof(buffer), handle) != 0)
   {
	   printf(buffer);
   }
   fclose(handle);
   printf("\n");
}

void vmod()
{
  FILE * handle;
  handle = fopen("vendeg.txt", "r");
  int handle2;
  handle2 = open("./vendeg2.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  char buffer[80], buffer2[80], buffer3[80];
  printf("\nAdja meg a modositando nevet, mail cimet, rendezveny azonositot szokozzel elvalasztva: ");
  fgets(buffer, sizeof(buffer), stdin);
  int poz = 0;
  bool volt = false;
  while(fgets(buffer2, sizeof(buffer2), handle) != 0 && hossz(buffer) != 0)
   {
		   int j = 0;
		   while(buffer[j] == buffer2[j] && j < hossz(buffer))
		   {
			   j++;
		   }
		   if(j == hossz(buffer) && buffer2[j] == ' ')
		   {
			   volt = true;
			   fclose(handle);
			   handle = fopen("vendeg.txt", "r");
			   int pozig = 0;
			   while(fgets(buffer2, sizeof(buffer2), handle) != 0 && pozig < poz)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
					pozig++;
				}
				
			   printf("Adja meg az uj nevet: ");
			   fgets(buffer3, sizeof(buffer3), stdin);
			   write(handle2, &buffer3, hossz(buffer3));
			   write(handle2, " ", 1);
			   printf("Adja meg az uj mail cimet: ");
			   fgets(buffer3, sizeof(buffer3), stdin);
			   write(handle2, &buffer3, hossz(buffer3));
			   write(handle2, " ", 1);
			   printf("Adja meg az uj rendezveny azonositot: ");
			   fgets(buffer3, sizeof(buffer3), stdin);
			   write(handle2, &buffer3, hossz(buffer3));
			   write(handle2, " ", 1);
			   time_t most = time(0);
			   write(handle2, ctime(&most), 24);
			   write(handle2, "\n", 1);
			   while(fgets(buffer2, sizeof(buffer2), handle) != 0)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
				}
			   j = 0;
			   printf("\nMegtortent a modositas.\n");
		   }
		poz++;	   
   }
   
  fclose(handle);
  close(handle2);
  
  if(!volt)
  {
	  printf("\nNincs vendeg ilyen parameterekkel.\n");
	  remove("./vendeg2.txt");
  }
  else
  {
	  remove("./vendeg.txt");
	  rename("./vendeg2.txt", "./vendeg.txt");
  }
  
  
  printf("\n");
}

void vtorl()
{
  FILE * handle;
  handle = fopen("vendeg.txt", "r");
  int handle2;
  handle2 = open("./vendeg2.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  char buffer[80], buffer2[80], buffer3[80];
  printf("\nAdja meg a torlendo nevet, mail cimet, rendezveny azonositot szokozzel elvalasztva: ");
  fgets(buffer, sizeof(buffer), stdin);
  int poz = 0;
  bool volt = false;
  while(fgets(buffer2, sizeof(buffer2), handle) != 0 && hossz(buffer) != 0)
   {
		   int j = 0;
		   while(buffer[j] == buffer2[j] && j < hossz(buffer))
		   {
			   j++;
		   }
		   if(j == hossz(buffer) && buffer2[j] == ' ')
		   {
			   volt = true;
			   printf("\nMegtortent a modositas.\n");
			   fclose(handle);
			   handle = fopen("vendeg.txt", "r");
			   int pozig = 0;
			   while(fgets(buffer2, sizeof(buffer2), handle) != 0 && pozig < poz)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
					pozig++;
				}

			   while(fgets(buffer2, sizeof(buffer2), handle) != 0)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
				}
			   j = 0;
		   }
		poz++;	   
   }
   
  fclose(handle);
  close(handle2);
  if(!volt)
  {
	  printf("\nNincs vendeg ilyen parameterekkel.\n");
	  remove("./vendeg2.txt");
  }
  else
  {
	  remove("./vendeg.txt");
	  rename("./vendeg2.txt", "./vendeg.txt");
  }
  
  printf("\n");
}

void ujrend()
{
  int handle;
  handle = open("./rendezveny.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  lseek(handle, 0, SEEK_END);
  if (handle < 0)
  {
      printf("Fajl hiba.\n");
      return;
  }
  char buffer[80];
  printf("\nAdja meg a rendezveny nevet: ");
  fgets(buffer, sizeof(buffer), stdin);
  write(handle, &buffer, hossz(buffer));
  write(handle, " ", 1);
  printf("Adja meg a rendezveny azonosito szamat: ");
  fgets(buffer, sizeof(buffer), stdin);
  write(handle, &buffer, hossz(buffer));
  write(handle, "\n", 1);
  close(handle);
  printf("\n");
}

void rendtorl()
{
  FILE * handle;
  handle = fopen("rendezveny.txt", "r");
  int handle2;
  handle2 = open("./rendezveny2.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  char buffer[80], buffer2[80], buffer3[80];
  printf("\nAdja meg a torlendo rendezvenynevet, rendezveny azonositot szokozzel elvalasztva: ");
  fgets(buffer, sizeof(buffer), stdin);
  int poz = 0;
  bool volt = false;
  while(fgets(buffer2, sizeof(buffer2), handle) != 0 && hossz(buffer) != 0)
   {
		   int j = 0;
		   while(buffer[j] == buffer2[j] && j < hossz(buffer))
		   {
			   j++;
		   }
		   if(j == hossz(buffer) && buffer2[j] == ' ')
		   {
			   volt = true;
			   printf("\nMegtortent a modositas.\n");
			   fclose(handle);
			   handle = fopen("rendezveny.txt", "r");
			   int pozig = 0;
			   while(fgets(buffer2, sizeof(buffer2), handle) != 0 && pozig < poz)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
					pozig++;
				}

			   while(fgets(buffer2, sizeof(buffer2), handle) != 0)
				{
					write(handle2, &buffer2, hossz(buffer2));
					write(handle2, "\n", 1);
				}
			   j = 0;
		   }
		poz++;	   
   }
   
  fclose(handle);
  close(handle2);
  if(!volt)
  {
	  printf("\nNincs rendezveny ilyen parameterekkel.\n");
	  remove("./rendezveny2.txt");
  }
  else
  {
	  remove("./rendezveny.txt");
	  rename("./rendezveny2.txt", "./rendezveny.txt");
  }
  
  printf("\n");
}

int main(int argc,char** argv){
 char valaszt[10];
 int valasztas = 0;
 printf("\n");
 while (valasztas != 7)
 {
	 printf("1 - Vendeg jelentkezes\n2 - Jelentkezesek listazasa\n3 - Vendeg adatainak modositasa\n4 - Vendeg torlese\n5 - Uj rendezveny hozzadasa\n6 - Rendezveny torlese\n7 - Kilepes\n");
	 fgets(valaszt, sizeof(valaszt), stdin);
	 valasztas = atoi(valaszt);
	 switch(valasztas)
	 {
		 case 1:
			vjel();
			break;
		 case 2:
			jellist();
			break;
		 case 3:
			vmod();
			break;
		 case 4:
			vtorl();
			break;
		 case 5:
			ujrend();
			break;
		 case 6:
			rendtorl();
			break;
	 }
 }
 return 0;
}