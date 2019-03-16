#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

typedef int bool;
#define true 1
#define false 0

struct Guest
{
	char name[100];
	char email[100];
	int eventAzon;
	time_t registrationTime;
};

struct Event
{
	struct Guest guests[100];
	int registeredGuests;
	int azon;
};

struct Event events[100];
int eventNumber = 0;

void Load()
{
	DIR* d;
	struct dirent * dp;
	
	d = opendir(".");
	
	if(d < 0)
		printf("Nem sikerult a fajlbol betoltes!\n");
	
	while (dp = readdir(d))
	{
		int len = strlen(dp->d_name);
		if(len > 9)
		{
			const char* lastFour = &dp->d_name[len - 4];
			char* firstFive = (char*)malloc(5*sizeof(char));
			strncpy(firstFive, dp->d_name, 5);
			if(strcmp(firstFive, "event\0") == 0 && strcmp(lastFour, ".txt\0") == 0)
			{
				struct Event ev;
				
				FILE* file= fopen(dp->d_name, "rb");
				if (file != NULL) 
				{
					fread(&ev, sizeof(ev), 1, file);
					events[eventNumber++] = ev;
				}
			}
		}
	}
}

int ExistingEventAzon(int value)
{
    int i;
    for (i = 0; i < eventNumber; i++)
	{
        if (events[i].azon == value)
			return i;
    }
	printf("Hibas azonosito! ");
    return -1;
}

void RegisterNewGuest()
{
	char name[100];
	char email[100];
	int eventAzon;
	time_t registrationTime;
	if(eventNumber == 0)
	{
		printf("\nNincs meg regisztralva egy esemeny sem. Nincs mire bejelentkezni!\n\n");
		return;
	}
	printf("Adja meg a nevet: ");
	scanf("%s", name);
	printf("Adja meg az e-mail cimet: ");
	scanf("%s", email);
	
	int aktAzon = -1;
	do
	{
		printf("Adja meg a rendezveny azonositot: ");
		scanf("%d", &eventAzon);
		if(eventAzon > 0)
		{
			aktAzon = ExistingEventAzon(eventAzon);
		}
	}while(aktAzon == -1);
	time(&registrationTime);
	
	struct Event* ev = &events[aktAzon];

	strcpy(ev->guests[ev->registeredGuests].name, name);
	strcpy(ev->guests[ev->registeredGuests].email, email);
	ev->guests[ev->registeredGuests].eventAzon = events[aktAzon].azon;
	ev->guests[ev->registeredGuests].registrationTime = registrationTime;
	
	printf("On a %d. bejelentkezett vendeg\n\n", ev->registeredGuests + 1);
	++(ev->registeredGuests);
}

int ReadEventAzon()
{
	int eventAzon;
	int aktAzon = -1;
	
	do
	{
		printf("Adja meg a rendezveny azonositot: ");
		int readResult = scanf("%d", &eventAzon);
		if(readResult == EOF)
		{
			printf("Nem adott meg azonositot! ");
		}
		else if(readResult == 0)
		{
			while (fgetc(stdin) != '\n');
			printf("Hibás bemenet! ");
		}
		else if(eventAzon > 0)
		{
			aktAzon = ExistingEventAzon(eventAzon);
		}
		else
		{
			printf("Hibás bemenet! ");
		}
	}while(aktAzon == -1);
	
	return aktAzon;
}

void ListRegistered()
{
	if(eventNumber == 0)
	{
		printf("\nNincs meg regisztralva egy esemeny sem. Nincs mit listazni!\n\n");
		return;
	}
	
	int eventAzon = ReadEventAzon();
	
	struct Event* ev = &events[eventAzon];
	printf("\nAz esemenyre bejelentkezett szama: %d\n\n", ev->registeredGuests);
	int i;
	for(i = 0; i < ev->registeredGuests; ++i)
	{
		printf("%d. jelentkezett neve: %s, e-mail: %s, idobelyegje: %d\n",
				i + 1, ev->guests[i].name, ev->guests[i].email, ev->guests[i].registrationTime);
	}
	printf("\n");
}

void ModifyGuestData()
{
	char name[100];
	char email[100];
	char newName[100];
	char newEmail[100];
	
	if(eventNumber == 0)
	{
		printf("\nNincs meg regisztralva egy esemeny sem, amire lenne feljelentkezve vendeg. Nincs mit modositani!\n\n");
		return;
	}
	
	int eventAzon = ReadEventAzon();
	
	printf("Adja meg a keresett vendeg nevet: ");
	scanf("%s", &name);
	printf("Adja meg a keresett vendeg e-mailjet: ");
	scanf("%s", &email);
	
	struct Event* ev = &events[eventAzon];
	
	int i = 0;
	while(i < ev->registeredGuests && strcmp(ev->guests[i].name, name) != 0 && strcmp(ev->guests[i].email, email) != 0)
	{
		++i;
	}
	if(i >= ev->registeredGuests)
	{
		printf("Nem letezik ilyen vendeg!\n\n");
		return;
	}
	
	printf("Adja meg az uj nevet: ");
	scanf("%s", &newName);
	printf("Adja meg az uj e-mailt: ");
	scanf("%s", &newEmail);
	
	strcpy(ev->guests[i].name, newName);
	strcpy(ev->guests[i].email, newEmail);
	
	printf("\nAz uj adatok: %s %s %d\n\n", ev->guests[i].name, ev->guests[i].email, ev->guests[i].registrationTime);	
}

void DeleteGuestData()
{
	char name[100];
	char email[100];
	
	if(eventNumber == 0)
	{
		printf("\nNincs meg regisztralva egy esemeny sem, amire lenne feljelentkezve vendeg. Nincs mit modositani!\n\n");
		return;
	}
	
	int eventAzon = ReadEventAzon();
	
	printf("Adja meg a keresett vendeg nevet: ");
	scanf("%s", &name);
	printf("Adja meg a keresett vendeg e-mailjet: ");
	scanf("%s", &email);
	
	struct Event* ev = &events[eventAzon];
	
	int i = 0;
	while(i < ev->registeredGuests && strcmp(ev->guests[i].name, name) != 0 && strcmp(ev->guests[i].email, email) != 0)
	{
		++i;
	}
	if(i >= ev->registeredGuests)
	{
		printf("Nem letezik ilyen vendeg!\n\n");
		return;
	}
	
	for(; i < ev->registeredGuests - 1; ++i)
	{
		strcpy(ev->guests[i].name, ev->guests[i + 1].name);
		strcpy(ev->guests[i].email, ev->guests[i + 1].email);
		ev->guests[i].registrationTime = ev->guests[i + 1].registrationTime;
	}
	--ev->registeredGuests;
}

void NewEvent()
{
	events[eventNumber].registeredGuests = 0;
	events[eventNumber].azon = eventNumber + 1;
	printf("\nUj rendezveny letrehozva %d azonositoszammal.\n\n", eventNumber + 1);
	++eventNumber;
}

void DeleteEvent()
{
	if(eventNumber == 0)
	{
		printf("\nNincs meg regisztralva egy esemeny sem, amire lenne feljelentkezve vendeg. Nincs mit modositani!\n\n");
		return;
	}
	int eventAzon = ReadEventAzon();
	int pos = 0;
	
	while(eventAzon != events[pos].azon)
	{
		++pos;
	}
	
	int az = events[pos].azon + 1;
	
	for(; pos < eventNumber - 1; ++pos)
	{
		int i;
		for(i = 0; i < events[pos].registeredGuests; ++i)
		{
			events[pos].guests[i] = events[pos + 1].guests[i];
		}
		events[pos].azon = events[pos + 1].azon;
		events[pos].registeredGuests = events[pos + 1].registeredGuests;
	}
	
	char filename[] = "event";
	char fileEnding[10];
	sprintf(fileEnding, "%d.txt\0", az);
	strcat(filename, fileEnding);
	if(remove(filename) == 0)
	{
		printf("Torolve!\n");
	}
	else
	{
		printf("Hiba!\n");
	}
}

void Save()
{
	int i;
	for(i = 0; i < eventNumber; ++i)
	{
		char filename[] = "./event";
		char fileEnding[10];
		sprintf(fileEnding, "%d", i + 1);
		strcat(fileEnding, ".txt\0");
		strcat(filename, fileEnding);

		FILE* file= fopen(filename, "wb");
		if (file != NULL) 
		{
			fwrite(&events[i], sizeof(events[i]), 1, file);
			fclose(file);
		}
	}
}

int main()
{
	Load();
	bool exit = false;
	do
	{
		printf("1 - Jelentkezes\n");
		printf("2 - Jelentkezettek listazasa\n");
		printf("3 - Vendeg adatainak modositasa\n");
		printf("4 - Vendeg adatainak torlese\n");
		printf("5 - Uj rendezveny inditasa\n");
		printf("6 - Rendezveny osszes adatanak torlese\n");
		printf("7 - Kilepes es fajlba mentes\n");
		
		printf("\nAdja meg a parancsot: ");

		int inputNumber = 0;
		scanf("%d", &inputNumber);
		switch(inputNumber)
		{
			case 1:
				RegisterNewGuest();
				break;
			case 2:
				ListRegistered();
				break;
			case 3:
				ModifyGuestData();
				break;
			case 4:
				DeleteGuestData();
				break;
			case 5:
				NewEvent();
				break;
			case 6:
				DeleteEvent();
				break;
			case 7:
				exit = true;
				break;
			default:
				printf("Hibas bemenet!");
				break;
		}
	}while(exit == false);
	
	Save();
	
}