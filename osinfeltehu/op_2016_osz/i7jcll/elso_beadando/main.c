/*
Egy kiállításokat, rendezvényeket szervezõ társaság a "King of Stands", 
elektronikus látogatói nyilvántartást készített. 
A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, 
a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, 
hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetõséget
vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez.
Legyen lehetõségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! 
A jelentkezõknél a korábbi adatok mellett rögzítsük automatikusan a 
jelentkezési idõt(rendszeridõ) is(ezt nem kell bekérni)!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#define APPLICATION_SIZE 100
#define NAME_SIZE 20
#define MAIL_SIZE 30
#define EVENT_SIZE 30
#define EVENT_ID_SIZE 7 //so the code can be 6 long maximum
#define TIME_SIZE 20

void mainMenu();
struct Application createApplication();
void addApplicant();
void listApplications();
int countLines(char*);
void modifyApplication();
struct Application modify(struct Application);
void deleteApplication();
void listEvents();
void newEvent();
void deleteEvent();

struct Application {
	char name[NAME_SIZE];
	char mail[MAIL_SIZE];
	char eventID[EVENT_ID_SIZE];
};

struct Event {
	char name[NAME_SIZE];
	char eventID[EVENT_ID_SIZE];
};

int main (int argc, char** argv) {
	printf("Welcome to \"King of Stands\"!\n");
	mainMenu();
	return 0;
}

void mainMenu() {
	//provide user menu
	printf("\nFOMENU\n");
	printf("1: Jelentkezes\n");
	printf("2: Jelentkezesek listazasa\n");
	printf("3: Jelentkezes modositasa\n");
	printf("4: Jelentkezes torlese\n");
	printf("5: Uj rendezveny\n");
	printf("6: Rendezveny torlese\n");
	printf("7: Rendezvenyek listazasa\n");
	printf("Egyeb: kilepes\n");
	printf("Valasztas: ");
	int i;
	scanf("%d", &i);
	if(i == 1) {
		addApplicant();
		mainMenu();
	}
	else if(i == 2) {
		listApplications();
		mainMenu();
	}
	else if(i == 3) {
		modifyApplication();
		mainMenu();
	}
	else if(i == 4) {
		deleteApplication();
		mainMenu();
	}
	else if(i == 5) {
		newEvent();
		mainMenu();
	}
	else if(i == 6) {
		deleteEvent();
		mainMenu();
	}
	else if(i == 7) {
		listEvents();
		mainMenu();
	}
	else {
		printf("\n GOOD BYE!\n");
		return;
	}
}

struct Application createApplication() {
	struct Application application;
	//get name
	printf("\nJelentkezo neve: ");
	//discard input stuff
	char inputDiscard;
	while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
	scanf ("%[^\n]%*c",  &application.name);
	//get email
	printf("Jelentkezo email cime: ");
	scanf("%s", &application.mail);
	//get eventID
	FILE *f = fopen("events.txt", "r");
	char tmp[EVENT_SIZE];
	printf("\nRendezvenyek listaja:\n");
	int c = 1;
	while(fgets(tmp, sizeof(tmp), f)) {
		printf("%d. %s", c, tmp);
		c++;
	}
	fclose(f);
	printf("\nValasszon rendezvenyt: ");
	int i;
	scanf("%d", &i);
	FILE *g = fopen("events.txt", "r");
	char getEvent[EVENT_SIZE];
	int line = 0; 
	while(fgets(tmp, sizeof(tmp), f)) {
		line++;
		if(line == i) {
			strcpy(application.eventID, strtok(tmp, ","));
			break;
		}
	}
	fclose(g);
	return application;
}

void addApplicant() {
	//get applicant info
	struct Application application = createApplication();
	//save application
	FILE *applicationsFile = fopen("applications.txt", "a");
	time_t mytime;
    mytime = time(NULL);
	fprintf(applicationsFile, "%s,%s,%s,%s", application.eventID, application.name, application.mail, ctime(&mytime));
	printf("\nRogzitve: %s, %s, %s, %s", application.eventID, application.name, application.mail, ctime(&mytime));
	fclose(applicationsFile);
	//notice user of apllication number
	int counter = countLines("applications.txt");
	printf("Jelentkezes sorszama: %d\n", counter);
}

int countLines(char* fileName) {
	char c;
	int counter = 0;
	int getFile = open(fileName, O_RDONLY);
	while (read(getFile, &c, sizeof(c))) {
		if(c == '\n') {
			counter++;
		}
	}
	return counter;
}

void listApplications() {
	FILE *f = fopen("applications.txt", "r");
	char tmp[APPLICATION_SIZE];
	printf("\nJelentkezesek listaja:\n");
	int line = 1;
	while(fgets(tmp, sizeof(tmp), f)) {
		printf("%d. %s", line, tmp);
		line++;
	}
	fclose(f);
}

void listEvents() {
	FILE *f = fopen("events.txt", "r");
	char tmp[APPLICATION_SIZE];
	printf("\nRendezvenyek listaja:\n");
	int line = 1;
	while(fgets(tmp, sizeof(tmp), f)) {
		printf("%d. %s", line, tmp);
		line++;
	}
	fclose(f);
}

void modifyApplication() {
	//list to select from
	listApplications();
	printf("\nModositando jelentkezes szama: ");
	int i;
	scanf("%d", &i);
	//search for the selected line
	FILE *f = fopen("applications.txt", "r");
	FILE *g = fopen("applications2.txt", "w");
	char tmp[APPLICATION_SIZE];
	int line = 0;
	struct Application application;
	while(fgets(tmp, sizeof(tmp), f)) {
		line++;
		if(line == i) {
			char str[APPLICATION_SIZE];
			strcpy(str, tmp);
			char delim[] = ",\n";
			char* token;
			int place = 0;
			char* time;
			struct Application application;
			for (token = strtok(str, delim); token; token = strtok(NULL, delim)) {
				place++;
				if(place == 1) {
					strcpy(application.eventID, token);
				}
				else if(place == 2) {
					strcpy(application.name, token);
				}
				else if(place == 3) {
					strcpy(application.mail, token);
				}
				else if(place == 4) {
					time = token;
					printf("\nido: %s\n", time);
				}
			}
			application = modify(application);
			fprintf(g, "%s,%s,%s,%s\n", application.eventID, application.name, application.mail, time);
		}
		else {
			fprintf(g, "%s", &tmp);
		}
	}
	fclose(f);
	fclose(g);
	remove("applications.txt");
	rename("applications2.txt", "applications.txt");
}

struct Application modify(struct Application application) {
	char inputDiscard;
	while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
	printf("\nNev modositasa(i/n)? ");
	char answer;
	scanf("%c", &answer);
	if(answer == 'i') {
		printf("\nJelentkezo neve: ");
		while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
		scanf ("%[^\n]%*c", &application.name);
	}
	while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
	printf("\nE-mail modositasa(i/n)? ");
	scanf("%c", &answer);
	if(answer == 'i') {
		printf("Jelentkezo email cime: ");
		scanf("%s", &application.mail);
	}
	while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
	printf("\nRendezveny modositasa(i/n)? ");
	scanf("%c", &answer);
	if(answer == 'i') {
		printf("Rendezveny azonosito: ");
		scanf("%s", &application.eventID);
	}
	return application;
}

void deleteApplication() {
	//list to select from
	listApplications();
	printf("\nTorlendo jelentkezes szama: ");
	int i;
	scanf("%d", &i);
	//search for the selected line
	FILE *f = fopen("applications.txt", "r");
	FILE *g = fopen("applications2.txt", "w");
	char tmp[APPLICATION_SIZE];
	int line = 0;
	while(fgets(tmp, sizeof(tmp), f)) {
		line++;
		if(line == i) {
			//in this case we simply skip the insertion
			printf("\nJelentkezes sikeresen torolve!\n");
		}
		else {
			fprintf(g, "%s", &tmp);
		}
	}
	fclose(f);
	fclose(g);
	remove("applications.txt");
	rename("applications2.txt", "applications.txt");
}

void newEvent() {
	struct Event event;
	//get name
	printf("\nRendezveny neve: ");
	//discard input stuff
	char inputDiscard;
	while((inputDiscard = getchar()) != '\n' && inputDiscard != EOF){};
	scanf ("%[^\n]%*c",  &event.name);
	//get code
	printf("Rendezveny kodja: ");
	scanf("%s", &event.eventID);
	FILE *f = fopen("events.txt", "a");
	fprintf(f, "%s,%s\n", event.eventID, event.name);
	printf("\nRogzitve: %s, %s", event.eventID, event.name);
	fclose(f);
}

void deleteEvent() {
	char eventID[EVENT_ID_SIZE];
	//list to select from
	listEvents();
	printf("\nTorlendo rendezveny szama: ");
	int i;
	scanf("%d", &i);
	//search for the selected line
	FILE *f = fopen("events.txt", "r");
	FILE *g = fopen("events2.txt", "w");
	char tmp[APPLICATION_SIZE];
	int line = 0;
	while(fgets(tmp, sizeof(tmp), f)) {
		line++;
		if(line == i) {
			//copy the code of the event
			strcpy(eventID, strtok(tmp, ","));
			//in this case we simply skip the insertion
			printf("\nRendezveny sikeresen torolve!\n");
		}
		else {
			fprintf(g, "%s", &tmp);
		}
	}
	fclose(f);
	fclose(g);
	remove("events.txt");
	rename("events2.txt", "events.txt");
	char event[APPLICATION_SIZE];
	//remove applications related to event
	FILE *appFile = fopen("applications.txt", "r");
	FILE *newAppFile = fopen("applications2.txt", "w");
	printf("\nTORLENDO: %s\n", eventID);
	char tmpLine[APPLICATION_SIZE];
	while(fgets(event, sizeof(event), appFile)) {
		printf("\nSTART\n");
		strcpy(tmpLine, event);
		printf("\nAKTUALIS SOR: %s\n", tmpLine);
		int comp = strcmp(eventID, strtok(tmpLine, ","));
		if(comp == 0) {
			printf("\nSTIMMEL, TORLES: %s\n", tmpLine);
			//in this case we delete it (so do nothing here)
		}
		else {
			printf("\nDELETE: %s\n", event);
			fprintf(newAppFile, "%s", &event);
		}
	}
	fclose(appFile);
	fclose(newAppFile);
	remove("applications.txt");
	rename("applications2.txt", "applications.txt");
}