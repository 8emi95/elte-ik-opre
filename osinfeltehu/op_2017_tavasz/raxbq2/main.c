
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Questions 
{
	int questionnumber;
	int answnumber;
	char question[50];
	char answoption[50];
	char date[20];
};

void dateToString(char *str)
{
	time_t ti = time(0);
	struct tm t;
	t = *localtime( &ti );
	strftime(str, 25, "%Y.%m.%d-%H:%M", &t);
}

void fajlbaIras(struct Questions *datas, int n)
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

void newQuestion(struct Questions** datas, int *n)
{
	struct Questions tmp;
	
	tmp.customerID = maxID(*datas, *n) + 1;
	printf("Kerdes: ");
	scanf("%s", &tmp.question);
	printf("Valaszlehetosegek szama (2-4): ");
	scanf("%d", tmp.answnumber);
	int i = 0;
	while (i < (tmp.answnumber))
	{
		printf("A(z) %d .valaszlehetoseg: \n", i);
		scanf("%s", &tmp.answoption);
	}
	dateToString(tmp.date);
	
	struct Questions* ujdatas = malloc( (*n+1) * sizeof(struct Questions) );
	memcpy(ujdatas, *datas, *n * sizeof(struct Questions));
	memcpy(ujdatas+*n, &tmp, sizeof(struct Questions));
	
	*n = *n+1;
	
	*datas = ujdatas;

	fajlbaIras(*datas, *n);
}

void deleteQuestion(struct Questions* datas, int n)
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

void listQuestions(struct Questions* datas, int n)
{
	printf("%10s %11s %17s %21s" , "Kerdes", "EsemenyAzon", "E-mail", "Nev\n");
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
      printf("\n1. Uj kerdes felvetele\n"\
             "2. Kerdes modositasa\n"\
             "3. Kerdes torlese\n"\
             "4. Kerdesek listazasa\n"\
			 "5. Kerdesek veglegesitese\n"\
             "0. Kilepes\n"\
             "Valasztott: ");
             
      scanf("%d", &menu);   
      
      switch(menu)
      {
        case 1:
            newQuestions(&datas, &n);
            break;
        case 2:
            editQuestion(datas, n);
            break;
        case 3:
            deleteQuestion(datas, n);
            break;
        case 4:
            listQuestions(datas, n);
            break;
		case 5:
			deleteQuestion(datas, n);
			break;
      }
	}
	while (menu != 0);
	
	return 0;
}