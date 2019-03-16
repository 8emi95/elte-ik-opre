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
  char helyszin[100];
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

void esemeny(guest* guests, event* events);
event* chooseEvent(guest* guests, event* events);

event selectEvent(guest* guests, event* events);
void addEvent(event* events, int eventId, int* eventCount);

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
  printf("8 - Esemeny");
  
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
	
	case 8 :
	  esemeny(guests, events);
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
  
  printf("Rendezveny helyszine: ");
  scanf("%s", newEvent.helyszin);
  
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>


void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

#include <sys/shm.h>

void esemeny(guest* guests, event* events) {
	
	int* s;
	key_t key;
    int shared_mem_id;
	
	key = ftok("fghj", 1);

    shared_mem_id = shmget(key, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	s = shmat(shared_mem_id, NULL, 0);
	
	signal(SIGRTMIN, handler);
	
	unlink("fifo");
	int fid = mkfifo("fifo", S_IRUSR | S_IWUSR );
	if (fid == -1) {
		printf("Error number: %i", errno);
		exit(EXIT_FAILURE);
    }
	
	pid_t partner = fork();
	
	if (partner < 0) {
		//gond van
		perror("Hiba van! (fork)");
		exit(1);
	}
	
	if (partner > 0) {
		int i, k, guestCount;
		
		
		
		// parent
		
		// ----------- NamedPipe --------------
		event* esemeny = chooseEvent(guests, events);
		int fd = open("fifo", O_WRONLY);
		write(fd, esemeny->helyszin, sizeof(esemeny->helyszin));
		close(fd);
//		printf("State 1\n");
		// ----------- Signal --------------
		pause();
		
		// ----------- NamedPipe --------------
		guest* resztvevok = (guest*)malloc( sizeof(guest) * guestCount );
		guests = (guest*)read_file(sizeof(guest), &guestCount, "guests.bin");
		k = 0;
		for(i = 0; i < guestCount; i++) {
			if(guests[i].id == esemeny->id) {
				memcpy( &(resztvevok[k]), &(guests[i]), sizeof(guest) );
				k++;
			}
		}
		fd = open("fifo", O_WRONLY);
		write(fd, &k, sizeof(int));
		write(fd, resztvevok, sizeof(guest) * k);
		close(fd);
		
		// ----------- sharedmem --------------
		int ertekeles;
		pause();
		memcpy(&ertekeles, s, sizeof(int));
		
		printf("Ertekeles: %d/100\n", ertekeles);
		
		// ------------------ MQUEUE -----------------
		
		
		// Megvárjuk a gyereket -> partner
		wait(partner);
	} else {
		// child
		// ----------- NamedPipe --------------
		char helyszin[100];
		int fd = open("fifo", O_RDONLY);
		read(fd, helyszin, sizeof(helyszin));
		close(fd);
//		printf("Juhu!!\n");
		printf("%s\n", helyszin);
		
		// ----------- Signal --------------
		kill(getppid(), SIGRTMIN);
		
		// ----------- NamedPipe --------------
		
		int k, i;
		fd = open("fifo", O_RDONLY);
		read(fd, &k, sizeof(k));
		if (k < 1) {
			perror("Nincs.\n");
			exit(23);
		}
			
		guest* resztvevok = (guest*)malloc(sizeof(guest) * k);
		read(fd, resztvevok, sizeof(guest) * k);
		for (i = 0; i < k; i++) {
			printf("%s\n", resztvevok[i].name);
		}
		
		close(fd);
		unlink("fifo");
		
		events = read_file(sizeof(event), &k, "events.bin");
		reset_file("events.bin");
		for (i = 0; i < k; i++) {
			if (events[i].id != resztvevok[0].id) {
				append_file(&events[i], sizeof(event), 1, "events.bin");
			}
		}
		
		guests = read_file(sizeof(guest), &k, "guests.bin");
		reset_file("guests.bin");
		for (i = 0; i < k; i++) {
			if (guests[i].id != resztvevok[0].id) {
				append_file(&guests[i], sizeof(guest), 1, "guests.bin");
			}
		}
		// ------------------ SharedMem -----------------
		
		int value = rand() % 100;
		memcpy(s, &value, sizeof(int));
		kill(getppid(), SIGRTMIN);
		
		// ------------------ MQUEUE -----------------
		
	}
}

event* chooseEvent(guest* guests, event* events) {
	int eventCount, i, selected;
	
	events = (event*)read_file(sizeof(event), &eventCount, "events.bin");
	if (eventCount < 1) {
		printf("Nincs esemeny :(");
		exit(12);
	}
	printf("Valassz egy esemenyt! : ");
	for (i = 0; i < eventCount; i++) {
		printf("%d %s\n", events[i].id, events[i].helyszin);
	}
	scanf("%d", &selected);
	
	for (i = 0; i < eventCount; i++) {
		if (events[i].id == selected) {
			break;
		}
	}
	
	return &(events[i]);
}

event selectEvent(guest* guests, event* events) {
  int db;
  int i = 0;
  int nth;
  int eventCount = 0;
  int selected;
  
  guests = (guest*)read_file(sizeof(guest), &db, "guests.bin");
  for(i = 0; i < db; i++) {
	addEvent(events, guests[i].id, &eventCount);
  }
  if(eventCount < 1) {
	printf("Nincs rendezveny.");
	exit(12);
  }
  
  // Esemeny kivalaszt
  printf("Esemenyek:\n");	
  for(i = 0; i < eventCount; i++) {
	  printf("%d ", events[i].id);
  }
  printf("\nValassz esemenyt: ");
  scanf("%d", &selected);
  for (i = 0; i < eventCount; i++) {
	if (events[i].id == selected) {
		nth = events[selected].id;
		break;
	}
  }
  
  printf("\nEsemeny helyszine: ");
  scanf("%s", events[nth].helyszin);
  return events[nth];
}

void addEvent(event* events, int eventId, int* eventCount) {
	int i;
	int vane = 0;
	for(i = 0; i < *eventCount; i++) {
		if (events[i].id == eventId) {
			vane = 1;
			break;
		}
	}
	if (!vane) {
		eventCount++;
		event* temp = (event*)malloc( sizeof(event) * (*eventCount) );
		memcpy( temp, events, sizeof(event) * (*eventCount-1) );
		temp[*eventCount-1].id = eventId;
		free(events);
		events = temp;
	}
}
