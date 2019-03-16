#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "filemanager.c"
#include <time.h>

typedef struct{
  char name[40];
  char email[40];
  int id;
  time_t timestamp;
} guest;

typedef struct{
  int id;
} event;

void start(guest* guests, event* events);
int selection();
void run(int selected, guest* guests, event* events);
void login(guest* guests, event* events);
void list(guest* guests, event* events);
void modGuest(guest* guests, event* events);
void deleteGuest(guest* guests, event* events);
void createEvent(guest* guests, event* events);
void deleteEvent(guest* guests, event* events);

int main()
{
  
  guest* guests;
  event* events;
  start(guests, events);
  return 0;
}

void start(guest* guests, event* events)
{
  int selected;
  
  system("clear");
  printf("King of Stands\n\n");
  
  printf("1 - Jelentkezes rendezvenyre\n");
  printf("2 - Jelentkezettek listazasa\n");
  printf("3 - Vendeg adatainak modositasa\n");
  printf("4 - Vendeg adatainak torlese\n");
  printf("5 - Rendezveny inditasa\n");
  printf("6 - Rendezveny adatainak torlese\n");
  printf("7 - Kilepes\n");
  
  selected = selection();
  run(selected, guests, events);
  
}

int selection()
{
  int selected;
  
  printf("\nValasztott menupont: ");
  scanf("%d", &selected);
  
  return selected;
}

void run(int selected, guest* guests, event* events)
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  switch(selected)
  {
    case 1 :
	  login(guests, events);
      break;	
	case 2 :
	  list(guests, events);
      break;
	  
	case 3 :
	  modGuest(guests, events);
      break;
	 
	case 4 :
	  deleteGuest(guests, events);
      break;
	  
	case 5 :
	  createEvent(guests, events);
      break;
	  
	case 6 :
	  deleteEvent(guests, events);
      break;
	  
	case 7 :
	  exit(0);
      break;
  }
}

void login(guest* guests, event* events)
{
  guest newGuest;
  
  char *buffer;
  size_t bufsize = 40;
  size_t characters;
  int db = 0;
  int sorszam = 0;
  int i = 0;
  
  buffer = (char *)malloc(bufsize * sizeof(char));
  if( buffer == NULL)
  {
    perror("Unable to allocate buffer");
    exit(1);
  }
  
  system("clear");
  printf("\nJelentkezes rendezvenyre\n");
  
  printf("\nNev: ");
  characters = getline(&buffer,&bufsize,stdin);
  memcpy(newGuest.name, buffer, characters-1);
  newGuest.name[characters-1] = '\0';
  
  printf("\nE-mail: ");
  characters = getline(&buffer,&bufsize,stdin);
  memcpy(newGuest.email, buffer, characters-1);
  newGuest.email[characters-1] = '\0';
  
  printf("\nRend. azon.: ");
  characters = getline(&buffer,&bufsize,stdin);
  newGuest.id = atoi(buffer);
  
  newGuest.timestamp = time(NULL);
  
  append_file(&newGuest, sizeof(guest), 1, "guests.bin");
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  for(i = 0;i< db;i++)
  {
	if(guests[i].id == newGuest.id)
	{
		sorszam++;
	}
  }
  printf("\nOn a %d. jelentkezo az esemenyre!", sorszam);
  
  getchar();
  free(buffer);
  start(guests, events);
}

void list(guest* guests, event* events)
{
  system("clear");
  int db;
  int i = 0;
  int id;
  int volte = 0;
  
  printf("\nRendezvény azonosito: ");
  scanf("%d", &id);
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  for(i = 0;i< db;i++)
  {
	if(guests[i].id == id)
	{
		printf("%s, %s, %d, %s\n", guests[i].name, guests[i].email, guests[i].id, ctime(&guests[i].timestamp));
		volte++;
	}
  }
  if(volte < 1)
	  printf("Nincs jelentkezo a rendezvenyre, vagy nincs ilyen rendezveny");
  getchar();
  getchar();
  start(guests, events);
}

void modGuest(guest* guests, event* events)
{
  char* name;
  int i = 0;
  int db = 0;
  int sorszam = 0;
  system("clear");
  printf("Vendeg adatainak modositasa, torlese\n");
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  for(i = 0;i< db;i++)
	printf("%d. %s, %s, %d, %s\n", i+1, guests[i].name, guests[i].email, guests[i].id, ctime(&guests[i].timestamp));
  
  printf("Modositando vendeg sorszama: ");
  scanf("%d", &sorszam);
  
  reset_file("guests.bin");
  for(i = 0; i < db; i++)
  {
	if( sorszam != i+1)
		append_file(&guests[i], sizeof(guest), 1, "guests.bin");
	else
	{
		guest newGuest;

		char *buffer;
		size_t bufsize = 40;
		size_t characters;

		buffer = (char *)malloc(bufsize * sizeof(char));
		if( buffer == NULL)
		{
		perror("Unable to allocate buffer");
		exit(1);
		}
		
		getchar();
		
		printf("Uj nev: ");
		characters = getline(&buffer,&bufsize,stdin);
		memcpy(newGuest.name, buffer, characters-1);
		newGuest.name[characters-1] = '\0';

		printf("Uj e-mail: ");
		characters = getline(&buffer,&bufsize,stdin);
		memcpy(newGuest.email, buffer, characters-1);
		newGuest.email[characters-1] = '\0';
		
		newGuest.id = guests[i].id;
		
		newGuest.timestamp = time(NULL);
		
		append_file(&newGuest, sizeof(guest), 1, "guests.bin");
		free(buffer);
	}
  }
  
  getchar();
  start(guests, events);
}

void deleteGuest(guest* guests, event* events)
{
  char* name;
  int i = 0;
  int db = 0;
  int sorszam = 0;
  system("clear");
  printf("Vendeg adatainak torlese\n");
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  for(i = 0;i< db;i++)
	printf("%d. %s, %s, %d, %s\n", i+1, guests[i].name, guests[i].email, guests[i].id, ctime(&guests[i].timestamp));
  
  printf("Torlendo vendeg sorszama: ");
  scanf("%d", &sorszam);
  
  reset_file("guests.bin");
  for(i = 0; i < db; i++)
  {
	if( sorszam != i+1)
		append_file(&guests[i], sizeof(guest), 1, "guests.bin");
  }
  
  getchar();
  start(guests, events);
}

void createEvent(guest* guests, event* events)
{
  event newEvent;
  
  system("clear");
  printf("Rendezveny inditasa\n");
  
  printf("Rendezveny azonosito: ");
  scanf("%d", &newEvent.id);
  
  append_file(&newEvent, sizeof(event), 1, "events.bin");
  
  getchar();
  start(guests, events);
}

void deleteEvent(guest* guests, event* events)
{
  int id;
  int db;
  system("clear");
  printf("\nRendezveny azonosito: ");
  scanf("%d", &id);
  int i = 0;
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  
  
  reset_file("guests.bin");
  for(i = 0; i < db; i++)
  {
	if(guests[i].id != id)
		append_file(&guests[i], sizeof(guest), 1, "guests.bin");
  }
  
  getchar();
  start(guests, events);
}
