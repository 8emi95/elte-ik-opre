/*
Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)! 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Events {
	int customerID;
	int eventID;
	char eMail[50];
	char name[50];
	char date[25];
};

int maxID(struct Events *datas, int n)
{
   if (n==0) return 0;
	int max = datas[0].customerID;
	for (int i = 1; i < n; ++i)
	{
		if (datas[i].customerID > max)
		{
			max = datas[i].customerID;
		}
	}	
	return max;
}

void dateToString(char *str)
{
	time_t ti = time(0);
	struct tm t;
	t = *localtime( &ti );
	strftime(str, 25, "%Y.%m.%d-%H:%M", &t);
}

void fajlbaIras(struct Events *datas, int n)
{
	int tenyleges = 0;
	for (int i = 0; i < n; ++i) 
	{
		if (datas[i].customerID != 0)
		{
			++tenyleges;
		}
	}
	FILE* file = fopen("datas.txt", "w");
	fprintf(file, "%d\n", tenyleges);
	for (int i = 0; i < n; ++i)
	{
		if (datas[i].customerID != 0)
			fprintf(file, "%d %d %s %s %s\n", datas[i].customerID, datas[i].eventID, datas[i].eMail, datas[i].name, datas[i].date);
	}
	
	fclose(file);
}

void newCustomer(struct Events** datas, int *n)
{
	struct Events tmp;
	
	tmp.customerID = maxID(*datas, *n) + 1;
	printf("Rendezveny azon:");
	scanf("%d", &tmp.eventID);
	printf("E-mail cim: ");
	scanf("%s", tmp.eMail);
	printf("Nev: ");
	scanf("%s", &tmp.name);
	dateToString(tmp.date);
	
	struct Events* ujdatas = malloc( (*n+1) * sizeof(struct Events) );
	memcpy(ujdatas, *datas, *n * sizeof(struct Events));
	memcpy(ujdatas+*n, &tmp, sizeof(struct Events));
	
	*n = *n+1;
	
	*datas = ujdatas;

	fajlbaIras(*datas, *n);
	
	printf("Hozzaadva %d. emberkent\n", tmp.customerID);
}

void deleteCustomer(struct Events* datas, int n)
{
	int bool = 0;
	int i = 0;
	int id;
	
	printf("Torlendo ember: ");
	scanf("%d", &id);
	
	while (i < n && bool!=1)
	{
		if (datas[i].customerID == id)
		{
			bool = 1;
			datas[i].customerID = 0;
		}
		else
		{
			++i;
		}
	}
	if (bool)
	{
		fajlbaIras(datas, n);
		printf("Torolve a(z) %d. ember", id);
	}
}

void editCustomer(struct Events* datas, int n)
{
	int id;
	int i = 0;
	int bool = 0;
	
	printf("Modositando ember: ");
	scanf("%d", &id);
	
	while (i < n && bool != 1)
	{
		if (datas[i].customerID == id)
		{
			bool = 1;
			printf("Rendezveny azon:");
			scanf("%d", &datas[i].eventID);
			printf("E-mail cim: ");
			scanf("%s", datas[i].eMail);
			printf("Nev: ");
			scanf("%s", datas[i].name);
		}
		else
		{
			++i;
		}
	}
	fajlbaIras(datas, n);

   puts("Sikeresen modositva");	
}
void listCustomer(struct Events* datas, int n)
{
	printf("%10s %11s %17s %21s" , "VendegAzon", "EsemenyAzon", "E-mail", "Nev\n");
	for (int i = 0; i < n; i++)
	{
		if (datas[i].customerID != 0)
		{
			printf("%4d %12d %30s %15s\n", datas[i].customerID, datas[i].eventID, datas[i].eMail, datas[i].name);
		}
	}
}
int main () {
	FILE* file = fopen("datas.txt", "r");
	if (!file) {
		file = fopen("datas.txt", "w");
		fprintf(file, "0\n");
		fclose(file);
		file = fopen("datas.txt", "r");
	}
	
	int n;
	fscanf(file, "%d", &n);
	
	struct Events* datas = malloc(n*sizeof(struct Events));
	
   for (int i = 0; i < n; ++i)
	{
		fscanf(file, "%d %d %s %s %s", &datas[i].customerID, &datas[i].eventID, datas[i].eMail, datas[i].name, datas[i].date);
	}
	
	fclose(file);
   int menu;
   
	do
      {
      printf("\n1. Uj vendeg\n"\
             "2. Jelentkezettek listazasa\n"\
             "3. Vendeg adatainak modositasa\n"\
             "4. Vendeg torlese\n"\
             "0. Kilepes\n"\
             "Valasztott: ");
             
      scanf("%d", &menu);   
      
      switch(menu)
      {
         case 1:
            newCustomer(&datas, &n);
            break;
         case 2:
            listCustomer(datas, n);
            break;
         case 3:
            editCustomer(datas, n);
            break;
         case 4:
            deleteCustomer(datas, n);
            break;
      }
   }
   while (menu != 0);

	
	return 0;
}