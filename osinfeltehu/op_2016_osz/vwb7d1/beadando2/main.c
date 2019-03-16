/**
 * @author Nagy Tibor (vwb7d1)
 */

/*
	2. beadandó feladat. Beadási határidő: 2016-12-11.

	A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét)
	az esemény lebonyolítására. A gyermekfolyamat indulása után csővezetéken megkapja, hogy hol kerül a rendezvény
	lebonyolításra! A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott
	helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. Ezután a rendezvényszervező csővezetéken
	elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevők. Ezt a partner képernyőre írja, majd várja a
	vendégeket és levezényli a rendezvényt! A rendezvény végén jelentést küld vissza, amiben tájékoztatja a
	főszervezőt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki
	nem érkezett meg! (Minden résztvevő 10% valószínűséggel nem érkezik meg!) Miután egy esemény lebonyolításra
	került, "King of Stands" újra megvizsgálja, melyik rendezvény a következő!
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#define MNU_PROGRAM_CREATE 1
#define MNU_PROGRAM_GET_VISITORS 2
#define MNU_PROGRAM_START 3

#define MNU_VISITOR_CREATE 4
#define MNU_VISITOR_UPDATE 5
#define MNU_VISITOR_DELETE 6

#define MNU_PROGRAM_CLEAR_ALL_DATA 7

#define MNU_EXIT 8

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

pid_t child;
int fd[2];
char place[128];
char placeList[8][128];

void readVisitorsFromPipe() {
	if (child == 0) {
		int visitorCame = 0;
		int visitorNotCame = 0;

		char programVisitorName[STRING_LENGTH];

		printf("A gyerek beolvassa a resztvevoket a csobol\n");

		close(fd[1]);

		while (read(fd[0], &programVisitorName, sizeof(programVisitorName))) {
			printf("Vendeg neve: %s\n", programVisitorName);
			printf("Eljott: ");
			if (rand() % 100 < 10) {
				visitorNotCame++;
				printf("Nem\n\n");
			} else {
				visitorCame++;
				printf("Igen\n\n");
			}

			fflush(stdout);
		}

		close(fd[0]);

		printf("A gyerek beolvasta a resztvevoket a csobol\n");

		printf("A rendezvenyre %i vendeg jott el, %i vendeg maradt tavol.", visitorCame, visitorNotCame);

		printf("A gyerek tavozik a helyszinrol\n");

		exit(0);
	}
}

void writeVisitorsToPipe() {
	if (child > 0) {
		int i;

		close(fd[0]);

		for (i = 0; i < programs[programId].counter; i++) {
			write(fd[1], &programs[programId].visitors[i].name, sizeof(programs[programId].visitors[i].name));
		}

		close(fd[1]);

		printf("A szulo beirta a resztvevoket a csobe\n");

		wait();
	}
}

void childSignal(int sn) {
	printf("A gyerek a helyszinen van!\n\n");

	printf("A szulo elkuldi a resztvevok listajat\n\n");

	writeVisitorsToPipe();
}

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

void programStart() {
	printf("--- Rendezveny inditasa ---\n");

	showSelectablePrograms();

	if (pipe(fd) < 0){
		perror("Pipe error\n");
		exit(EXIT_FAILURE);
	}

	child = fork();
	if (child == -1) {
		perror("Fork hiba!\n");
		exit(EXIT_FAILURE);
	}

	if (child == 0) {
		// Gyerek folyamat
		close(fd[1]);

		printf("A gyerek olvas a csobol\n");
		read(fd[0], &place, sizeof(place));
		printf("A beolvasott hely: %s\n", place);
		close(fd[0]);

		kill(getppid(), SIGUSR1);

		sleep(1);

		readVisitorsFromPipe();
	} else {
		// Szulo folyamat
		int placeId = rand() % 8;

		close(fd[0]);

		write(fd[1], &placeList[placeId], sizeof(placeList[placeId]));
		printf("A szulo beirta a helyszint a csobe\n");

		close(fd[1]);

		wait();
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

	printf(" %d - Rendezveny inditasa\n", MNU_PROGRAM_START);

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
		case MNU_PROGRAM_START:
			programStart();
			sleep(1);
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
	srand(time(NULL));

	strcpy(placeList[0], "Elso helyszin");
	strcpy(placeList[1], "Masodik helyszin");
	strcpy(placeList[2], "Harmadik helyszin");
	strcpy(placeList[3], "Negyedik helyszin");
	strcpy(placeList[4], "Otodik helyszin");
	strcpy(placeList[5], "Hatodik helyszin");
	strcpy(placeList[6], "Hetedik helyszin");
	strcpy(placeList[7], "Nyolcadik helyszin");

	signal(SIGUSR1, childSignal);

	loadPrograms();
	handleMenu();

	return 0;
}