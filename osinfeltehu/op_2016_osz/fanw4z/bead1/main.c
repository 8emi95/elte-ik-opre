#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

struct Guest
{
	char name[20];
	char email[20];
	char date[20];
};

struct Event
{
	int id;
	char name[60];
	int guestCount;
	struct Guest guestArray[20];
};

void proceedWithEnter()
{
	getchar(); //elozo enter
	char ch;
	printf("Nyomjon entert a folytatáshoz! ");
	while ((ch = getchar()) != '\n'){}		
}

void deleteGuest(int* guestCount, struct Guest* guests)
{
	char toDelete[20];
	printf("Adja meg a törölni kívánt vendég nevét!\n");
	fscanf(stdin, "%s", toDelete);
	
	bool found = false;
	int i = 0;
	while(!found && i<*guestCount) 
	{
		if(strcmp(toDelete, guests->name) == 0) found = true;
		if(!found)
		{
			++guests;
			++i;
		}
	}
	
	if(found)
	{
		--(*guestCount);
		while(i<*guestCount)
		{
			*guests = *(guests+1);
			++guests;
			++i;
		}
		printf("Sikeres törlés\n\n");
	}
	else
	{
		printf("Nincs ilyen vendég\n\n");
	}
	
}

void editGuest(int* guestCount, struct Guest* guests)
{
	char toEdit[20];
	printf("Adja meg a szerkeszteni kívánt vendég nevét!\n");
	fscanf(stdin, "%s", toEdit);
	
	bool found = false;
	int i = 0;
	while(!found && i<*guestCount) 
	{
		if(strcmp(toEdit, guests->name) == 0) found = true;
		if(!found)
		{
			++guests;
			++i;
		}
	}
	
	if(found)
	{
		printf("Adja meg az új nevet!\n");
		fscanf(stdin, "%s", guests->name);
		getchar(); //enter 
		
		printf("Adja meg az email címét!\n");
		fscanf(stdin, "%s", guests->email);
		getchar(); //enter 
		
		time_t now = time(NULL);
		strftime(guests->date, 20, "%Y-%m-%d", gmtime(&now));
		
		printf("Sikeres módosítás\n");
	}
	else
	{
		printf("Nincs ilyen vendég\n\n");
	}
}

void readFile(int* eventCount, struct Event* events)
{
	
	char buff[255];
	FILE * f = fopen("input.txt","r");
	if (f==NULL){perror("Adathiba"); exit(1);}
	if(!f) perror("fopen");
	
	fgets(buff, 255, f);
	
	*eventCount = atoi(buff);

	for(int i=0; i<*eventCount; ++i)
	{	
		fgets(buff, 255, f);
		events->id = atoi(buff);
		fgets(events->name, 70, f);
		strtok(events->name, "\n");
		
		fgets(buff, 255, f);
		events->guestCount = atoi(buff);
		for(int i=0; i<events->guestCount; ++i)
		{
			fscanf(f, "%s", events->guestArray[i].name);
			fscanf(f, "%s", events->guestArray[i].email);
			fscanf(f, "%s", events->guestArray[i].date);
			fgets(buff, 255, f); //új sor karakter
		}
		++events;
	}
}

void writeFile(int* eventCount, struct Event* events)
{
	FILE * f;
	char buff[255];
	f=fopen("input.txt","w");
	if (f==NULL){perror("Hiba az adatfájl mentésekor, a módosítások elvesztek"); exit(1);}
	fgets(buff, 255, f);
	
	fprintf(f, "%d", *eventCount); 
	fputc('\n', f);
	for(int i=0; i<*eventCount; ++i)
	{
		fprintf(f, "%d", events->id); 
		fputc('\n', f);
		fprintf(f, "%s", events->name); 
		fputc('\n', f);
		fprintf(f, "%d", events->guestCount); 
		fputc('\n', f);
		for(int i=0; i<events->guestCount; ++i)
		{
			fputs(events->guestArray[i].name, f);
			fputc(' ', f);
			fputs(events->guestArray[i].email, f);
			fputc(' ', f);
			fputs(events->guestArray[i].date, f);
			fputc('\n', f);
		}
		++events;
	}
	
}


void writeMainMenu()
{
	printf("\n");
	printf("Kérem adjon meg egy parancsot!\n");
	printf("1 - Események listázása\n");
	printf("2 - Jelentkezés egy eseményre\n");
	printf("3 - Új esemény indítása\n");
	printf("4 - Esemény törlése\n");
	printf("5 - Esemény adatainak listázása\n");
	printf("q - Kilépés\n");
}

void listEvents(int* eventCount, struct Event* events)
{
	
	if(*eventCount == 0) printf("Még nincs esemény létrehozva\n");
	
	for(int i=0; i<*eventCount; ++i)
	{
		printf("ID: %d, Név: %s\n", events->id, events->name);
		++events;
	}
	
	proceedWithEnter();
}

void attendEvent(int* eventCount, struct Event* events)
{
	printf("Adja meg az esemény azonosítóját, amelyre jelentkezni kíván!\n");
	
	char buff[255]; int eventToAttend;
	fscanf(stdin, "%s", buff);
	eventToAttend = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToAttend ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}	
	
	if(found)
	{
		int guestNumber = events->guestCount;
		events->guestCount = events->guestCount + 1;
		
		printf("Adja meg a nevét!\n");
		fscanf(stdin, "%s", events->guestArray[guestNumber].name);
		getchar(); //enter 
		
		printf("Adja meg az email címét!\n");
		fscanf(stdin, "%s", events->guestArray[guestNumber].email);
		
		
		time_t now = time(NULL);
		strftime(events->guestArray[guestNumber].date, 20, "%Y-%m-%d", gmtime(&now));
		
		printf("Sikeres regisztráció\n\n");
	}
	else
	{
		printf("Nincs ilyen esemény\n\n");
	}
	
	proceedWithEnter();

}

void newEvent(int* eventCount, struct Event* events)
{
	events += *eventCount;
	getchar();
	printf("Léterhozandó esemény neve: ");
	fgets(events->name, 70, stdin);
	strtok(events->name, "\n");
	events->id = *eventCount;
	events->guestCount = 0;
	++(*eventCount);
	
	printf("Esemény létrehozva\n");
}

void deleteEvent(int* eventCount, struct Event *events)
{
	
	printf("Adja meg az esemény azonosítóját, amelyet törölni kíván!\n");
	
	char buff[255]; int eventToDelte;
	fscanf(stdin, "%s", buff);
	eventToDelte = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToDelte ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}

	if(found)
	{
		--(*eventCount);
		while(i<*eventCount)
		{
			*events = *(events+1);
			++events;
			++i;
		}
		printf("Sikeres törlés\n\n");
	}
	else
	{
		printf("Nincs ilyen esemény\n\n");
	}
	
	proceedWithEnter();
}

void editEvent(int* eventCount, struct Event* events)
{
	printf("Adja meg az esemény azonosítóját, amelyet listázni kíván!\n");
	
	char buff[255]; int eventToEdit;
	fscanf(stdin, "%s", buff);
	eventToEdit = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToEdit ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}

	if(found)
	{
		printf("Az esemény azonosítója: %d\n", events->id); ;
		printf("Az esemény neve: %s\n", events->name); 
		printf("A vendégek listája:\n");
		
		if(events->guestCount == 0) printf("Ezen az eseményen nincsenek még vendégek\n\n");
		else
		{
			for(int i=0; i<events->guestCount; ++i)
			{
				printf("A vendég neve: %s\n", events->guestArray[i].name);
				printf("A vendég email címe: %s\n", events->guestArray[i].email);
				printf("A vendég jelentkezésének dátuma: %s\n\n", events->guestArray[i].date);
			}
			
			printf("Szeretné módosítani a vendéglistát? (y - igen, egyéb - vissza a főmenübe) ");
			getchar(); //elozo enter
			char cmd = getchar();
			if(cmd == 'y')
			{
				getchar();
				printf("\n");
				printf("1 - vendég törlése\n");
				printf("2 - vendég módosítása\n");
				printf("Más - vissza a főmenübeme\n");
				cmd = getchar();
				int* gc = &(events->guestCount);
				
				switch(cmd)
				{
					case '1':
						deleteGuest(gc, events->guestArray);
						proceedWithEnter();
						break;
					case '2':
						editGuest(gc, events->guestArray);
						break;
					default:
						break;
				}
			}
		}
	}
	else
	{
		printf("Nincs ilyen esemény\n\n");
		proceedWithEnter();
	}
	
	
}




int main(int argc, char** argv)
{
	int* eventCount = malloc(sizeof(*eventCount));
	struct Event events[10];
	
	readFile(eventCount, events);
	bool keepDoing = true;
	do
	{
		writeMainMenu();
		char cmd = getchar();
		switch(cmd)
		{
			case '1':
				listEvents(eventCount, events);
				break;
			case '2':
				attendEvent(eventCount, events);
				break;
			case '3':
				newEvent(eventCount, events);
				break;
			case '4':
				deleteEvent(eventCount, events);
				break;
			case '5':
				editEvent(eventCount, events);
				break;
			case 'q':
				printf("Viszont Látásra!\n");
				keepDoing = false;
				break;
			default:
				printf("Nincs ilyen parancs!\n");
				break;
		}
	}while(keepDoing);
	
	writeFile(eventCount, events);
	
}