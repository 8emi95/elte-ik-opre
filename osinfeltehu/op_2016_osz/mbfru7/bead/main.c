#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define maxGuests 200
#define maxEvents 100

struct Guest {
	char* name;
	char* email;
	time_t registered;
};

struct Event {
	int numGuests;
	struct Guest* guests;
};

void printMenu();
void newGuest();
void printGuests();
void editGuest();
void removeGuest();
void newEvent();
void removeEvent();
void loadDB(int*, struct Event*);
void saveDB(int*, struct Event*);

int main () {
	int eventnum = 0;
	struct Event* events = malloc(maxEvents * sizeof(struct Event));
	loadDB(&eventnum, events);

	//struct Guest g = {.name = "haha", .email = "hehe", .regTime = time(NULL)};

	//segmentation fault
	//events[0].guests[0] = g;

	char choice;
	do {
		printMenu();

		choice = getchar();
		while (getchar() != '\n');

		switch (choice) {
			case '0': saveDB(&eventnum, events); printf("\n Viszont látásra!\n\n"); break;
			case '1': newGuest(); break;
			case '2': printGuests(); break;
			case '3': editGuest(); break;
			case '4': removeGuest(); break;
			case '5': newEvent(); break;
			case '6': removeEvent(); break;
			default: printf("\n Érvénytelen menüpont!\n"); break;
		}
	} while (choice != '0');

	return 0;
}

void printMenu() {
	printf("\n");
	printf("-----------------------------\n");
	printf(" Látogatói nyilvántartás\n");
	printf("-----------------------------\n");
	printf(" 1. Új vendég.\n");
	printf(" 2. Vendégek listája.\n");
	printf(" 3. Vendégek módosítása.\n");
	printf(" 4. Vendég törlése.\n");
	printf(" 5. Új rendezvény.\n");
	printf(" 6. Rendezvény törlése.\n");
	printf(" 0. Kilépés.\n");
	printf("-----------------------------\n");
	printf(" Választott menüpont: ");
}

void newGuest() {

}

void printGuests() {

}

void editGuest() {

}

void removeGuest() {

}

void newEvent() {

}

void removeEvent() {

}

void loadDB(int* n, struct Event* e) {
	FILE *file;
	file = fopen("database.bin", "rb");
	if (file == NULL) {
		printf("Hiba történt az adatbázis megnyitása közben!\n");
		exit(1);
	}

	fread(n, sizeof(int), 1, file);

	int i;
	for (i = 0; i < *n; i++) {
		fread(&e[i].numGuests, sizeof(int), 1, file);
		e[i].guests = malloc (maxGuests * sizeof(struct Guest));
		fread(e[i].guests, sizeof(struct Guest), e[i].numGuests, file);
	}

	fclose(file);
}

void saveDB(int* n, struct Event* e) {
	FILE *file;
	file = fopen("database.bin", "wb");
	if (file == NULL) {
		printf("Hiba történt az adatbázis mentése közben!\n");
		exit(1);
	}

	fwrite(n, sizeof(int), 1, file);

	int i;
	for (i = 0; i < *n; i++) {
		fwrite(&e[i].numGuests, sizeof(int), 1, file);
		fwrite(e[i].guests, sizeof(struct Guest), e[i].numGuests, file);
	}

	fclose(file);
}
