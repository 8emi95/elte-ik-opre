#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct Guest
{
	char name[20];
	char eMailAddress[40];
	char programID[5];
	char registrationTime[30];
};

struct Program
{
	char id[5];
	char name[30];
};

int numberOfPrograms = 0;
int numberOfGuests = 0;
struct Program programs[1000];
struct Guest guests[1000];

int isFileEmpty(FILE * file)
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

void writeToFile()
{
	//guests.txt
	FILE * fileGuest = fopen("guests.txt", "w");
	if(fileGuest == NULL)
	{
		printf("Fájl megnyitási hiba, guests.txt, írás!\n");
		exit(1);
	}
	int i;
	for(i = 0; i < numberOfGuests; i++)
	{
		if(strcmp(guests[i].name, "###") != 0)
		{
			fprintf(fileGuest, "%s\t\t%s\t\t%s\t\t%s\n", guests[i].name, guests[i].eMailAddress, guests[i].programID, guests[i].registrationTime);
		}
	}
	fclose(fileGuest);
	
	//programs.txt
	FILE * fileProgram = fopen("programs.txt", "w");
	if(fileProgram == NULL)
	{
		printf("Fájl megnyitási hiba, programs.txt, írás!\n");
		exit(1);
	}
	for(i = 0; i < numberOfPrograms; i++)
	{
		if(strcmp(programs[i].id, "###") != 0)
		{
			fprintf(fileProgram, "%s\t\t%s\n", programs[i].id, programs[i].name);
		}
	}
	fclose(fileProgram);
}

void readInputFiles()
{
	//guests.txt
	FILE * fileGuest = fopen("guests.txt", "r");
	if(fileGuest == NULL)
	{
		printf("Fájl megnyitási hiba, guests.txt, olvasás!\n");
		exit(1);
	}
	if(isFileEmpty(fileGuest) == 0)
	{
		while(!feof(fileGuest))
		{
			fscanf(fileGuest, "%s\t\t%s\t\t%s\t\t%s\n", &guests[numberOfGuests].name, &guests[numberOfGuests].eMailAddress, &guests[numberOfGuests].programID, &guests[numberOfGuests].registrationTime);
			numberOfGuests++;
		}
	}
	fclose(fileGuest);
	
	//programs.txt
	FILE * fileProgram = fopen("programs.txt", "r");
	if(fileProgram == NULL)
	{
		printf("Fájl megnyitási hiba, programs.txt, olvasás!\n");
		exit(1);
	}
	if(isFileEmpty(fileProgram) == 0)
	{
		while(!feof(fileProgram))
		{
			fscanf(fileProgram, "%s\t\t%s\n", &programs[numberOfPrograms].id, &programs[numberOfPrograms].name);
			numberOfPrograms++;
		}
	}
	fclose(fileProgram);
}

void printMenu()
{
	printf("--King of Stands--\n");
	printf("Elektronikus látogatói nyilvántartás\n\n");
	printf("1 - Jelentkezés rendezvényre\n");
	printf("2 - Jelentkezettek listázása\n");
	printf("3 - Vendég adatainak módosítása\n");
	printf("4 - Vendég adatainak törlése\n");
	printf("5 - Új rendezvény felvétele\n");
	printf("6 - Rendezvény törlése\n");
	printf("0 - Kilépés\n\n");
}

int isProgramIdPresent(char id[])
{
	int i;
	for(i = 0; i < numberOfPrograms; i++)
	{
		if(strcmp(id, programs[i].id) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void menuRegisterToProgram()
{
	system("clear");
	printf("Új vendég felvétele\n\n");
	printf("1 - Felvesz egy új vendéget\n");
	printf("0 - Kilépés\n\n");
	int input;
	printf("Adjon meg egy számot: ");
	scanf("%d", &input);
	if(input == 1)
	{
		printf("Aktív programok:\n");
		int i;
		for(i = 0; i < numberOfPrograms; i++)
		{
			if(strcmp(programs[i].id, "###") != 0)
			{
				printf("%d - Id: %s, Név: %s\n", i+1, programs[i].id, programs[i].name);
			}
		}
		struct Guest guest;
		int response = 1;
		do
		{
			printf("\nAdja meg a vendég nevét: ");
			scanf("%s", guest.name);
			printf("Adja meg a vendég e-mail címét: ");
			scanf("%s", guest.eMailAddress);
			printf("Adja meg a program azonosítóját: ");
			scanf("%s", guest.programID);
			time_t mytime = time(NULL);
			char time[40];
			strcpy(time, ctime(&mytime));
			time[strlen(time) - 1] = '\0';
			for(i = 0; i < strlen(time); i++)
			{
				if(time[i] == ' ')
				{
					time[i] = '_';
				}
			}
			strcpy(guest.registrationTime, time);
			response = isProgramIdPresent(guest.programID);
		}while(response == 0);
		
		guests[numberOfGuests] = guest;
		numberOfGuests++;
		
		int place = 0;
		for(i = 0; i < numberOfGuests; i++)
		{
			if(strcmp(guests[i].programID, guests[numberOfGuests - 1].programID) == 0 && strcmp(guests[i].name, "###") != 0)
			{
				place++;
			}
		}
		printf("Ön a(z) %d. jelentkező a programra.\n", place);
		int input;
		printf("\nAdjon meg egy számot a folytatáshoz... ");
		scanf("%d", &input);
	}
}

void menuListGuests()
{
	system("clear");
	printf("Vendégek kilistázása\n\n");
	int i;
	for(i = 0; i < numberOfGuests; i++)
	{
		if(strcmp(guests[i].name, "###") != 0)
		{
			printf("Név: %s, E-mail: %s, Program: %s, Reg. Idő: %s\n", guests[i].name, guests[i].eMailAddress, guests[i].programID, guests[i].registrationTime);
		}
	}
	int input;
	printf("\nAdjon meg egy számot a folytatáshoz... ");
	scanf("%d", &input);
}

void menuModifyGuestData()
{
	system("clear");
	printf("Vendég adatainak módosítása\n\n");
	int i;
	for(i = 0; i < numberOfGuests; i++)
	{
		if(strcmp(guests[i].name, "###") != 0)
		{
			printf("%d - Név: %s, E-mail: %s, Program: %s, Reg. Idő: %s\n", i+1, guests[i].name, guests[i].eMailAddress, guests[i].programID, guests[i].registrationTime);
		}
	}
	printf("0 - Kilépés\n\n");
	int input;
	printf("Adjon meg egy számot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfGuests+1 && strcmp(guests[input - 1].name, "###") != 0)
	{
		printf("Adja meg a vendég új nevét: ");
		scanf("%s", guests[input - 1].name);
		printf("Adja meg a vendég új e-mail címét: ");
		scanf("%s", guests[input - 1].eMailAddress);
	}
}

void menuRemoveGuest()
{
	system("clear");
	printf("Vendég törlése\n\n");
	int i;
	for(i = 0; i < numberOfGuests; i++)
	{
		if(strcmp(guests[i].name, "###") != 0)
		{
			printf("%d - Név: %s, E-mail: %s, Program: %s, Reg. Idő: %s\n", i+1, guests[i].name, guests[i].eMailAddress, guests[i].programID, guests[i].registrationTime);
		}
	}
	printf("0 - Kilépés\n\n");
	int input;
	printf("Adjon meg egy számot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfGuests+1 && strcmp(guests[input - 1].name, "###") != 0)
	{
		strcpy(guests[input - 1].name, "###");
	}
}

void menuNewProgram()
{
	system("clear");
	printf("Új program felvétele\n\n");
	printf("1 - Felvesz egy új programot\n");
	printf("0 - Kilépés\n\n");
	int input;
	printf("Adjon meg egy számot: ");
	scanf("%d", &input);
	if(input == 1)
	{
		printf("Aktív programok:\n");
		int i;
		for(i = 0; i < numberOfPrograms; i++)
		{
			if(strcmp(programs[i].id, "###") != 0)
			{
				printf("Id: %s, Név: %s\n", programs[i].id, programs[i].name);
			}
		}
		struct Program pr;
		int response = 0;
		do
		{
			printf("\nAdja meg az esemény azonosítóját: ");
			scanf("%s", pr.id);
			printf("Adja meg az esemény nevét: ");
			scanf("%s", pr.name);
			response = isProgramIdPresent(pr.id);
		} while(response == 1);
		programs[numberOfPrograms] = pr;
		numberOfPrograms++;
	}
}

void menuRemoveProgram()
{
	system("clear");
	printf("Program törlése\n\n");
	int i;
	for(i = 0; i < numberOfPrograms; i++)
	{
		if(strcmp(programs[i].id, "###") != 0)
		{
			printf("%d - Id: %s, Név: %s\n", i+1, programs[i].id, programs[i].name);
		}
	}
	printf("0 - Kilépés\n\n");
	int input;
	printf("Adjon meg egy számot: ");
	scanf("%d", &input);
	if(input > 0 && input < numberOfPrograms+1 && strcmp(programs[input - 1].id, "###") != 0)
	{
		int j;
		for(j = 0; j < numberOfGuests; j++)
		{
			if(strcmp(guests[j].programID, programs[input-1].id) == 0)
			{
				strcpy(guests[j].name, "###");
			}
		}
		strcpy(programs[input-1].id, "###");
	}
}

int exitProgram()
{
	printf("Kilépés...\n");
	return 0;
}

void runProgram()
{
	int isRunning = 1;
	while(isRunning == 1)
	{
		system("clear");
		printMenu();
		int input;
		printf("Adjon meg egy számot: ");
		scanf("%d", &input);
		switch(input)
		{
			case 0:
				isRunning = exitProgram();
				break;
			case 1:
				menuRegisterToProgram();
				break;
			case 2:
				menuListGuests();
				break;
			case 3:
				menuModifyGuestData();
				break;
			case 4:
				menuRemoveGuest();
				break;
			case 5:
				menuNewProgram();
				break;
			case 6:
				menuRemoveProgram();
				break;
			default:
				isRunning = exitProgram();
				break;
		}
	}
}

int main(int argc,char** argv)
{
	readInputFiles();
	runProgram();
	writeToFile();
	return 0;
}