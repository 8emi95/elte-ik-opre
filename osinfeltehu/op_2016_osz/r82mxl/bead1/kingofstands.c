#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 64
#define TRUE (1)
#define FALSE (0)

void menu();
void jelentkezes();
void listazas();
void jelentkezesmod();
void jelentkezesdel();
void rendezveny();
void rendezvenydel();

int main(int argc, char *argv[])
{
	menu();
	return 0;
}

void menu()
{
	int menu;
	while(1)
	{	
		printf("\n1: Uj Jelentkezés\n");
		printf("2: Vendégek listázása\n");
		printf("3: Vendég adatainak módosítása\n");
		printf("4: Jelentkezés törlése\n");
		printf("5: Új rendezvény\n");
		printf("6: Rendezvény törlése\n");
		printf("7: Kilépés\n");
		printf("\nNyomj egy gombot: \t");
		scanf("%d", &menu);
		
		switch(menu)
		{
		case 1: jelentkezes();
			break;
		case 2: listazas();
			break;
		case 3: jelentkezesmod();
			break;
		case 4: jelentkezesdel();
			break;
		case 5: rendezveny();
			break;
		case 6: rendezvenydel();
			break;
		case 7: exit(0);
			break;
		default: printf("Hibás választás!\n");
			exit(0);
			break;
		}
	}
}

int isdotdash ( const char c )
{
  switch ( c )
  {
    case '.':
    case '-':
      return TRUE;
  }
  return FALSE;
}

int email_isvalid(const char *address) 
{
  int        count = 0;
  const char *c, *domain;
  static char *rfc822_specials = "()<>@,;:\\\"[]";

  c = address;

  if ( ! isalpha ( *c ))
    return 0;

  for ( c = address + 1;  *c && *c != '@';  c++ ) 
  {
    if ( isdotdash ( *c ))
    {
      if ( isdotdash ( *( c - 1 )))
        return 0;
    }
    else if ( ! isalnum ( *c ))
      return 0;
  }

  if ( *c != '@' )
    return 0;

  if ( ! isalnum ( *(c - 1 )))
    return 0;
  
  if (!*(domain = ++c)) return 0;
  do 
  {
    if (*c == '.') 
    {
      if (c == domain || *(c - 1) == '.') return 0;
      count++;
    }
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  } while (*++c);
  return (count >= 1);
}

void jelentkezes()
{
	FILE *kingofstands;
	kingofstands = fopen("kingofstands.txt","a+");
	
	char line[MAX_SIZE];
	char nev[MAX_SIZE];
	char mail[MAX_SIZE];
	int azonosito;

	printf("Név:\t");
	scanf("%s",nev);
	printf("\nE-mail cím:\t");
	scanf("%s",mail);
	while (email_isvalid(mail)!=TRUE)
	{
		printf("Hibás e-mail cím formátum!\n");
		printf("E-mail cím:\t");
		scanf("%s",mail);
	}
	printf("\nRendezvény azonosítója:\t");
	scanf("%d",&azonosito);
	time_t current_time;
        char* c_time_string;
        current_time = time(NULL);
        c_time_string = ctime(&current_time);
	
	int j = 0;
	while(fgets(line,sizeof(line),kingofstands) != NULL)
        {
		sscanf("%s",line);
		j++;
	}
	j = (j / 4) + 1;
	printf("Te vagy a %d. jelentkező.\n",j);

	fprintf(kingofstands,"%s\n%s\n%d\n%s", nev, mail, azonosito, c_time_string);

	fclose(kingofstands);
}

void listazas()
{
	FILE *kingofstands;
	
	kingofstands = fopen("kingofstands.txt","r");
	
	int j = -1;	
	char kiir[MAX_SIZE];
	printf("\nEddigi jelentkezések:\n");
	while(fgets(kiir,100,kingofstands) != NULL)
	{
		printf("%s",kiir);
		j++;
	};
	
	fclose(kingofstands);
}

void jelentkezesmod()
{
	FILE *kingofstands;
	kingofstands = fopen("kingofstands.txt","r");

	char sor[MAX_SIZE];
	char line[MAX_SIZE];
	char menu[MAX_SIZE];
	char ujadat[MAX_SIZE];
	int i, j, ujazonosito = 0;
	char kos[MAX_SIZE][MAX_SIZE];

	printf("Módosítandó regiszrációhoz tartozó név: ");
	scanf("%s",sor);
	strcat(sor,"\n");
        while(fgets(line,sizeof(line),kingofstands) != NULL)
        {
                sscanf("%s",line);
                j++;
		strcpy(kos[j],line);
        }

	fclose(kingofstands);
	kingofstands = fopen("kingofstands.txt","w+");

	for (i=1;i<=j;i++)
	{
		if (strcmp(sor,kos[i]) == 0)
		{
			printf("Mit szeretnél módosítani?\n");
                        printf("Név: (n)\n");
                        printf("E-mail cím: (e)\n");
                        printf("Rendezvény azonosítója: (r)\n");

                        scanf("%s",menu);

			if(strcmp(menu,"n") == 0)
                        {
                                printf("Add meg az nevet:\t");
                                scanf("%s",ujadat);
                                fprintf(kingofstands,"%s\n",ujadat);
                        }
                        else if(strcmp(menu,"e") == 0)
                        {
                                fputs(kos[i],kingofstands);
				i++;
				printf("Add meg az uj e-mail cimet:\t");
                                scanf("%s",ujadat);
				while (email_isvalid(ujadat)!=TRUE)
        			{
                			printf("Hibás e-mail cím formátum!\n");
                			printf("E-mail cím:\t");
                			scanf("%s",ujadat);
        			}
                                fprintf(kingofstands,"%s\n",ujadat);
                        }
                        else if(strcmp(menu,"r") == 0)
                        {
				fputs(kos[i],kingofstands);
                                i++;
				fputs(kos[i],kingofstands);
                                i++;
                                printf("Add meg az új rendezvényazonosítót:\t");
                                scanf("%d",&ujazonosito);
                                fprintf(kingofstands,"%d\n",ujazonosito);
                        }
		}
		else
		{
			fputs(kos[i],kingofstands);
		}
	}

	fclose(kingofstands);
}

void jelentkezesdel()
{
	char name[MAX_SIZE];
	printf("Add meg a nevet, amelyikhez tartozó regisztrációt torolni szeretned:\t");
	scanf("%s",name);
	FILE *kingofstands;
	kingofstands = fopen("kingofstands.txt","r");
        strcat(name,"\n");
	char line[MAX_SIZE];
	int i, j = 0;
	char kos[MAX_SIZE][MAX_SIZE];

        while(fgets(line,sizeof(line),kingofstands) != NULL)
        {
                sscanf("%s",line);
                j++;
                strcpy(kos[j],line);
        }
        fclose(kingofstands);

	kingofstands = fopen("kingofstands.txt","w+");
	
	for (i=1;i<=j;i++)
        {
                if (strcmp(name,kos[i]) == 0)
                {
                	i = i + 3;
                }
                else
                {
                        fputs(kos[i],kingofstands);
                }
        }
	fclose(kingofstands);
}

void rendezveny()
{
	FILE *events;
        events = fopen("events.txt","a+");
	char event[MAX_SIZE];
        int azonosito;

        printf("Rendezvény:\t");
        scanf("%s",event);
        printf("Rendezvény azonosítója:\t");
        scanf("%d",&azonosito);

        fprintf(events,"%s\n%d\n", event, azonosito);
        fclose(events);
}

void rendezvenydel()
{
	char event[MAX_SIZE];
        printf("Add meg a rendezvényt, amelyiket törölni szeretnéd:\t");
        scanf("%s",event);
        FILE *events;
        events = fopen("events.txt","r");
        strcat(event,"\n");
        char line[MAX_SIZE];
        int i, j = 0;
        char eve[MAX_SIZE][MAX_SIZE];

        while(fgets(line,sizeof(line),events) != NULL)
        {
                sscanf("%s",line);
                j++;
                strcpy(eve[j],line);
        }
        fclose(events);
        events = fopen("events.txt","w+");

        for (i=1;i<=j;i++)
        {
                if (strcmp(event,eve[i]) == 0)
                {
                        i++;
                }
                else
                {
                        fputs(eve[i],events);
                }
        }
        fclose(events);
}
