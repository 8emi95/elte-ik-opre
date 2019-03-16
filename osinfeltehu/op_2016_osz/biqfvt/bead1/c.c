/* Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define SIZE 30

//void loadDataFromFile();
//void writeDataToFile();
//bool eventExists(char eventId[SIZE]);
//void addGuest();
//void listGuests();
//void modifyGuest();
//void deleteGuest();
//void newEvent();
//void deleteEventData();

struct guestData {
	char eventId[SIZE];
	char name[SIZE];
	char email[SIZE];
	char regTime[SIZE];
	struct guestData *next;
};

struct event {
	char eventId[SIZE];
	char place[SIZE];
	char startTime[SIZE];
	struct event *next;
};

typedef int bool;
#define true 1
#define false 0

struct guestData *guestList = 0;
struct event *eventList = 0;

void loadDataFromFile() {
	int i;
	char eventId[SIZE];
	char name[SIZE];
	char email[SIZE];
	char regTime[SIZE];
	char place[SIZE];
	char startTime[SIZE];
	struct guestData *gData;
	struct event *eData;
	
	if( access("guests", 0 ) != -1 ) {
		FILE *guestFile;
		guestFile = fopen("guests", "r");
		if (guestFile != NULL) {
			while (fread(gData, sizeof(*gData), 1, guestFile)) {
				gData = malloc( sizeof(struct guestData) );
				gData->next = guestList;
				/* strcpy(gData->eventId, eventId);
				strcpy(gData->name, name);
				strcpy(gData->email, email);
				strcpy(gData->regTime, regTime); */
				for (i = 0; i < SIZE; i++) gData->eventId[i] = eventId[i];
				for (i = 0; i < SIZE; i++) gData->name[i] = name[i];
				for (i = 0; i < SIZE; i++) gData->email[i] = email[i];
				for (i = 0; i < SIZE; i++) gData->regTime[i] = regTime[i];
				guestList = gData;
			}
		}
		fclose(guestFile);
	}
	
	if( access("events", 0 ) != -1 ) {
		FILE *eventFile;
		eventFile = fopen("events", "r");
		if (eventFile != NULL) {
			while (fread(eData, sizeof(*eData), 1, eventFile)) {
				eData = malloc( sizeof(struct event) );
				eData->next = eventList;
				/* strcpy(eData->eventId, eventId);
				strcpy(eData->place, place);
				strcpy(eData->startTime, startTime); */
				for (i = 0; i < SIZE; i++) eData->eventId[i] = eventId[i];
				for (i = 0; i < SIZE; i++) eData->place[i] = place[i];
				for (i = 0; i < SIZE; i++) eData->startTime[i] = startTime[i];
				eventList = eData;
			}
		}
		fclose(eventFile);
	}
}

void writeDataToFile() {
	FILE *guestFile;
	guestFile = fopen("guests", "w");
	struct guestData *gData = guestList;
	while (gData != 0) {
		fwrite(gData, sizeof(*gData), 1, guestFile);
		gData = gData->next;
	}
	fclose(guestFile);
	
	FILE *eventFile;
	eventFile = fopen("events", "w");
	struct event *eData = eventList;
	while (eData != 0) {
		fwrite(eData, sizeof(*eData), 1, eventFile);
		eData = eData->next;
	}
	fclose(eventFile);
}

bool eventExists(char eventId[SIZE]) {
	struct event *events = eventList;
	int i = 1;
	while (events != 0 && events->eventId != eventId) {
		events = events->next;
		i++;
	}
	if(events == 0) return false;
	return true;
}

void addGuest() {
	char eventId[SIZE];
	char name[SIZE];
	char email[SIZE];
	char regTime[SIZE];
	char place[SIZE];
	char startTime[SIZE];
	
	printf("\n---Uj vendeg megadasa---\n");
	printf("\nRendezveny azonositoja: ");
	scanf("%s", &eventId);
	if( eventExists(eventId) ) {
		getchar();
		printf("Vendeg neve: ");
		scanf("%s", &name);
		getchar();
		printf("Vendeg email cime: ");
		scanf("%s", &email);
		getchar();
		
		time_t currentTime;
		currentTime = time(NULL);
		strcpy(regTime, ctime(&currentTime) );
		
		int i;	
		struct guestData *guest;
		guest = malloc( sizeof(struct guestData) );
		guest->next = guestList;
		/* strcpy(guest->eventId, eventId);
		strcpy(guest->name, name);
		strcpy(guest->email, email);
		strcpy(guest->regTime, regTime); */
		for (i = 0; i < SIZE; i++) guest->eventId[i] = eventId[i];
		for (i = 0; i < SIZE; i++) guest->name[i] = name[i];
		for (i = 0; i < SIZE; i++) guest->email[i] = email[i];
		for (i = 0; i < SIZE; i++) guest->regTime[i] = regTime[i];
		guestList = guest;
		printf("\nA vendeg regisztralasa sikerult.\n");
	} else {
		printf("\nNincs ilyen rendezveny.\n");
	}
	
}

void listGuests() {
	char eventId[SIZE];
	printf("\nVendegek listazasa\n");
	printf("\nAdja meg a rendezveny azonositojat:\n");
	scanf("%s", &eventId);
	getchar();
	if( eventExists(eventId) ) {
		printf("\nA rendezvenyre jelentkezett vendegek:\n\n");
		struct guestData *data = guestList;
		if (data != 0) {
			int i = 1;
			do {
				if(data->eventId == eventId) {
					printf("\n%d. ", i);
					printf("Rendezveny: %s", data->eventId);
					printf("Nev: %s", data->name);
					printf("E-mail: %s", data->email);
					printf("Idopont: %s", data->regTime);
				}
				i++;
				data = data->next;
			} while (data != 0);
		}
		else printf("Ures lista.\n");
	} else {
		printf("\nNincs ilyen rendezveny.\n");
	}
}

void modifyGuest() {
	char eventId[SIZE];
	char name[SIZE];
	char email[SIZE];
	char regTime[SIZE];
	char place[SIZE];
	char startTime[SIZE];
	int which;
	
	printf("\nVendeg adatainak modositasa\n");
	printf("\nAdja meg a rendezveny azonositojat: ");
	scanf("%s", &eventId);
	getchar();
	if( eventExists(eventId) ) {
		printf("\nAdja meg, hanyadik vendeget szeretne modositani: ");
		scanf("%d", &which);
		getchar();
		printf("\nAdja meg az uj adatokat:\n");
		printf("Vendeg neve: ");
		scanf("%s", &name);
		getchar();
		printf("Vendeg e-mail cime: ");
		scanf("%s", &email);
		getchar();
		
		time_t currentTime;
		currentTime = time(NULL);
		strcpy(regTime, ctime(&currentTime) );
		
		struct guestData *data = guestList;
		int i = 1;
		while (data != 0 && data->eventId != eventId && i != which) {
			data = data->next;
			i++;
		}
		if (data == 0) printf("Nincs ilyen sorszamu vendeg!\n");
		else {
			/* strcpy(data->eventId, eventId);
			strcpy(data->name, name);
			strcpy(data->email, email);
			strcpy(data->regTime, regTime); */
			for (i = 0; i < SIZE; i++) data->eventId[i] = eventId[i];
			for (i = 0; i < SIZE; i++) data->name[i] = name[i];
			for (i = 0; i < SIZE; i++) data->email[i] = email[i];
			for (i = 0; i < SIZE; i++) data->regTime[i] = regTime[i];
			printf("\nA bejegyzes modositva.\n");
		}
	} else {
		printf("\nNincs ilyen rendezveny.\n");
	}
}

void deleteGuest() {
	char eventId[SIZE];
	printf("\nVendeg torlese\n");
	printf("\nAdja meg a rendezveny azonositojat:\n");
	scanf("%s", &eventId);
	getchar();
	if( eventExists(eventId) ) {
		int which;
		printf("\nAdja meg, hanyadik vendeget szeretne torolni: ");
		scanf("%d", &which);
		getchar();
		
		struct guestData *data = guestList;
		struct guestData *elozo = 0;
		int i = 1;
		while (data != 0 && data->eventId != eventId && i != which) {
			elozo = data;
			data = data->next;
			i++;
		}
		if (data == 0) printf("Nincs ilyen sorszamu vendeg.\n");
		else {
			if (elozo == 0) guestList = data->next;
			else elozo->next = data->next;
			free(data);
			printf("\nA vendeg torolve.\n");
		}
	} else {
		printf("\nNincs ilyen rendezveny.\n");
	}
}

void newEvent() {
	char eventId[SIZE];
	char place[SIZE];
	char startTime[SIZE];
	printf("\nUj rendezveny letrehozasa\n");
	printf("\nAdja meg a rendezveny azonositojat: ");
	scanf("%s", &eventId);
	int i;	
	struct event *events;
	events = malloc( sizeof(struct event) );
	events->next = eventList;
	/* strcpy(events->eventId, eventId);
	strcpy(events->place, place);
	strcpy(events->startTime, startTime); */
	for (i = 0; i < SIZE; i++) events->eventId[i] = eventId[i];
	for (i = 0; i < SIZE; i++) events->place[i] = place[i];
	for (i = 0; i < SIZE; i++) events->startTime[i] = startTime[i];
	eventList = events;
	printf("\nA rendezveny hozzaadasa sikerult.\n");
}

void deleteEventData() {
	char eventId[SIZE];
	printf("\nRendezveny adatainak torlese\n");
	printf("\nAdja meg a rendezveny azonositojat: ");
	scanf("%s", &eventId);
	getchar();
	if( eventExists(eventId) ) {
		struct event *events = eventList;
		struct event *elozo = 0;
		int i = 1;
		while (events != 0 && events->eventId != eventId) {
			elozo = events;
			events = events->next;
			i++;
		}
		printf("\nA rendezveny torolve.\n");
	} else {
		printf("\nNincs ilyen rendezveny.\n");
	}
	
}

int main()
{
	bool exit = false;
	printf("King of Stands\n\n");
	printf("Ezzel a programmal rendezvenyekre lehet regisztralni es az adatokat kezelni.\n\n");
	loadDataFromFile();
	do {
		printf("Valasszon muveletet:\n\n");
		printf("1: Jelentkezes rendezvenyre\n");
		printf("2: Jelentkezettek listazasa\n");
		printf("3: Vendeg adatainak modositasa\n");
		printf("4: Vendeg adatainak torlese\n");
		printf("5: Uj rendezveny inditasa\n");
		printf("6: Rendezveny adatainak torlese\n");
		printf("7: Kilepes\n");
		printf("Valasztas: ");
		int choice = 0;
		scanf("%d", &choice);
		
		if(choice == 1) {
			addGuest();
		} else if(choice == 2) {
			listGuests();
		} else if(choice == 3) {
			modifyGuest();
		} else if(choice == 4) {
			deleteGuest();
		} else if(choice == 5) {
			newEvent();
		} else if(choice == 6) {
			deleteEventData();
		} else if(choice == 7) {
			exit = true;
		}
	} while(exit == false);
	writeDataToFile();
	return 0;
}