/**
 * @author Nagy Tibor (vwb7d1)
 */

/*
    1. beadando feladat. Beadasi hatarido: 2016-11-07.
    Keszitsen C programot, amely rendszerhivasok alkalmazasaval megoldja a kovetkezo feladatot:

    Egy kiallitasokat, rendezvenyeket szervezo tarsasag a "King of Stands", elektronikus latogatoi nyilvantartast keszitett.
    A latogatok a nyilvantartasba jelentkezeskor megadjak nevuket, mail cimuket, a rendezveny azonosito szamat!
    Az elektronikus rendszer a jelentkezes nyugtazasakent visszaadja, hogy hanyadik vendegkent tortent meg a regisztracio!

    Az alkalmazas adjon lehetoseget vendeg jelentkezeshez, a jelentkezettek listazasahoz, egy vendeg adatainak modositasahoz, torlesehez.
    A jelentkezoknel a korabbi adatok mellett rogzitsuk automatikusan a jelentkezesi idot (rendszerido) is (ezt nem kell bekerni)!

    Legyen lehetosegunk egy uj rendezveny inditasahoz, egy rendezveny osszes adatanak torlesehez!

    Keszitsunk C programot, amellyel a leirt tevekenyseg elvegezheto. Az adatokat fajlban taroljuk.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

#define MNU_PROGRAM_CREATE 1
#define MNU_PROGRAM_GET_VISITORS 2

#define MNU_VISITOR_CREATE 3
#define MNU_VISITOR_UPDATE 4
#define MNU_VISITOR_DELETE 5

#define MNU_PROGRAM_CLEAR_ALL_DATA 6

#define MNU_EXIT 7

#define PROGRAM_FILE_PREFIX "program"
#define PROGRAM_FILE_EXT "dat"

#define PROGRAM_COUNT 8
#define VISITOR_COUNT 16

#define STRING_LENGTH 64

struct s_visitor {
	char name[STRING_LENGTH];
	char email[STRING_LENGTH];
	char createdAt[STRING_LENGTH];
};

struct s_program {
	unsigned int initialized;
	unsigned int counter;

	struct s_visitor visitors[VISITOR_COUNT];
};

unsigned int menuCode;
unsigned int programCounter = 0;
unsigned int programId;
unsigned int visitorId;

struct s_program programs[PROGRAM_COUNT];
struct s_visitor visitor;

void clearVisitorData() {
	int i;

	for (i = 0; i < STRING_LENGTH; i++) {
		visitor.name[i] = 0;
		visitor.email[i] = 0;
		visitor.createdAt[i] = 0;
	}


}

void showSelectablePrograms() {
	printf("A valaszthato rendezvenyek: %d - %d\n", 1, programCounter);

	do {
		printf("Rendezveny szama: ");

		scanf("%d", &programId);

		if (programId > 0 && programId <= programCounter) {
			break;
		}
		printf("Nincs ilyen azonositoju rendezveny, kerjuk, probalja ujra!\n");
	} while (1);
}

void showSelectableVisitors(int count) {
	if (count == 0) {
		printf("Ehhez a rendezvenyhez nincs regisztralt vendeg.\n");
	} else {
		printf("A valaszthato vendegek: %d - %d\n", 1, count);

		do {
			printf("Vendeg szama: ");

			scanf("%d", &visitorId);

			if (visitorId > 0 && visitorId <= count) {
				break;
			}
			printf("Nincs ilyen azonositoju vendeg, kerjuk, probalja ujra!\n");
		} while (1);
	}
}

int createFile(int idx, int clr) {
	if (clr || programCounter < PROGRAM_COUNT) {
		char fileName[32];
		sprintf(fileName, "%s%d.%s", PROGRAM_FILE_PREFIX, idx, PROGRAM_FILE_EXT);

		int file = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

		if (file < 0) {
			perror("A fajl megnyitasa sikertelen!\n");
		} else {
			write(file, "", 1);
			close(file);
		}

		return 0;
	} else {
		printf("\n\nMinden rendezvenyhely betelt!\n\n");
		return 1;
	}
}

void writeProgramsToFile() {
	int i, j;
	char fileName[32];
	int file;

	for (i = 0; i < PROGRAM_COUNT; i++) {
		if (programs[i].initialized) {
			sprintf(fileName, "%s%d.%s", PROGRAM_FILE_PREFIX, i, PROGRAM_FILE_EXT);
			file = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

			if (file < 0) {
				perror("A fajl megnyitasa sikertelen!\n");
			} else {
				for (j = 0; j < programs[i].counter; j++) {
					clearVisitorData();

					strcpy(visitor.name, programs[i].visitors[j].name);
					strcpy(visitor.email, programs[i].visitors[j].email);
					strcpy(visitor.createdAt, programs[i].visitors[j].createdAt);

					write(file, &visitor, sizeof(visitor));
				}
				close(file);
			}
		}
	}
}

void loadPrograms() {
	do {
		programCounter++;
		char fileName[32];

		sprintf(fileName, "%s%d.%s", PROGRAM_FILE_PREFIX, programCounter, PROGRAM_FILE_EXT);
		int file = open(fileName, O_RDONLY);

		if (file < 0) {
			programCounter--;
			break;
		}

		programs[programCounter].initialized = 1;
		programs[programCounter].counter = 0;

		while (read(file, &visitor, sizeof(visitor))) {
			strcpy(programs[programCounter].visitors[programs[programCounter].counter].name, visitor.name);
			strcpy(programs[programCounter].visitors[programs[programCounter].counter].email, visitor.email);
			strcpy(programs[programCounter].visitors[programs[programCounter].counter].createdAt, visitor.createdAt);

			programs[programCounter].counter++;

			clearVisitorData();
		}

		close(file);
	} while (1);

	printf("Jelenleg %d rendezvenyhely van felhasznalva a %d-bol.\n\n", programCounter, PROGRAM_COUNT);
}

void programCreate() {
	printf("\n--- Rendezveny letrehozasa ---\n");

	programCounter++;
	int result = createFile(programCounter, 0);

	if (result == 0) {
		programs[programCounter].initialized = 1;

		printf("A rendezveny sikeresen letrehozva!\nA rendezveny sorszama: %d\n\n", programCounter);
	} else {
		programCounter--;
	}
}

void programGetVisitors() {
	printf("--- Rendezveny vendegeinek listaja ---\n");

	showSelectablePrograms();

	if (programs[programId].counter == 0) {
		printf("\nNincs vendeg jelentkezve erre a rendezvenyre.\n");
	} else {
		int i;
		for (i = 0; i < programs[programId].counter; i++) {
			printf("--- %d. vendeg ---\nNev: %s\nE-mail cim: %s\nLetrehozva: %s\n\n", i + 1,
				   programs[programId].visitors[i].name, programs[programId].visitors[i].email,
				   programs[programId].visitors[i].createdAt);
		}
	}
}

void programClearAllData() {
	printf("--- Rendezveny adatainak torlese ---\n");

	showSelectablePrograms();

	createFile(programId, 1);

	int i, j;
	for (i = 0; i < programs[programId].counter; i++) {
		for (j = 0; j < STRING_LENGTH; j++) {
			programs[programId].visitors[i].name[j] = 0;
			programs[programId].visitors[i].email[j] = 0;
			programs[programId].visitors[i].createdAt[j] = 0;
		}
	}

	programs[programId].counter = 0;

	printf("A rendezveny adatai sikeresen torlesre kerultek!\n\n");
}

void visitorCreate() {
	clearVisitorData();

	printf("--- Vendeg letrehozasa ---\n");

	showSelectablePrograms();

	printf("Nev: ");
	getchar();
	fgets(visitor.name, 127, stdin);

	printf("E-mail cim: ");
	fgets(visitor.email, 127, stdin);

	time_t currentTime;
	time(&currentTime);
	sprintf(visitor.createdAt, "%s", ctime(&currentTime));

	strtok(visitor.name, "\n");
	strtok(visitor.email, "\n");
	strtok(visitor.createdAt, "\n");

	strcpy(programs[programId].visitors[programs[programId].counter].name, visitor.name);
	strcpy(programs[programId].visitors[programs[programId].counter].email, visitor.email);
	strcpy(programs[programId].visitors[programs[programId].counter].createdAt, visitor.createdAt);
	programs[programId].counter++;

	writeProgramsToFile();

	printf("A vendeg sikeresen hozzaadva!\nA vendeg sorszama: %d\n\n", programs[programId].counter);

	clearVisitorData();
}

void visitorUpdate() {
	printf("--- Vendeg szerkesztese ---\n");

	showSelectablePrograms();
	showSelectableVisitors(programs[programId].counter);

	// Zero index
	visitorId--;

	printf("Nev (%s): ", programs[programId].visitors[visitorId].name);
	getchar();
	fgets(visitor.name, 127, stdin);

	printf("E-mail cim (%s): ", programs[programId].visitors[visitorId].email);
	fgets(visitor.email, 127, stdin);

	strtok(visitor.name, "\n");
	strtok(visitor.email, "\n");

	strcpy(programs[programId].visitors[visitorId].name, visitor.name);
	strcpy(programs[programId].visitors[visitorId].email, visitor.email);

	writeProgramsToFile();

	printf("A vendeg sikeresen szerkesztve!\n\n");

	clearVisitorData();
}

void visitorDelete() {
	printf("--- Vendeg torlese ---\n");

	showSelectablePrograms();
	showSelectableVisitors(programs[programId].counter);

	// Zero index
	visitorId--;

	int i, j;
	for (i = 0, j = 0; i < programs[programId].counter; i++) {
		if (visitorId != i) {
			programs[programId].visitors[j] = programs[programId].visitors[i];
			j++;
		}
	}

	for (j = 0; j < STRING_LENGTH; j++) {
		programs[programId].visitors[programs[programId].counter].name[j] = 0;
		programs[programId].visitors[programs[programId].counter].email[j] = 0;
		programs[programId].visitors[programs[programId].counter].createdAt[j] = 0;
	}

	programs[programId].counter--;

	writeProgramsToFile();
}

void handleMenu() {
	printf("Kerem, valasszon menupontot!\n");
	printf(" %d - Rendezveny letrehozasa\n", MNU_PROGRAM_CREATE);
	printf(" %d - Rendezveny vendegeinek listaja\n", MNU_PROGRAM_GET_VISITORS);

	printf(" %d - Vendeg letrehozasa\n", MNU_VISITOR_CREATE);
	printf(" %d - Vendeg szerkesztese\n", MNU_VISITOR_UPDATE);
	printf(" %d - Vendeg torlese\n", MNU_VISITOR_DELETE);

	printf(" %d - Rendezveny adatainak torlese\n", MNU_PROGRAM_CLEAR_ALL_DATA);

	printf("\n %d - Kilepes\n", MNU_EXIT);

	printf("\nKivalasztott menupont: ");

	scanf("%d", &menuCode);

	switch (menuCode) {
		case MNU_PROGRAM_CREATE:
			programCreate();
			break;
		case MNU_PROGRAM_GET_VISITORS:
			programGetVisitors();
			break;
		case MNU_VISITOR_CREATE:
			visitorCreate();
			break;
		case MNU_VISITOR_UPDATE:
			visitorUpdate();
			break;
		case MNU_VISITOR_DELETE:
			visitorDelete();
			break;
		case MNU_PROGRAM_CLEAR_ALL_DATA:
			programClearAllData();
			break;
		case MNU_EXIT:
			writeProgramsToFile();
			exit(0);
		default:
			printf("Ervenytelen menupont!\nKerjuk, probalja ujra!\n\n");
			break;
	}

	handleMenu();
}

int main(int argc, char **argv) {
	loadPrograms();

	handleMenu();

	return 0;
}