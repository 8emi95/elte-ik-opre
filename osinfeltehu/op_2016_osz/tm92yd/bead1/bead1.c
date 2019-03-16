// Operacios rendszerek - 1. beadando
//King of Stands
// Varga Máté István - TM92YD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int menu() {
    int selection = 0;

    printf("King of Stands!\n\n");
    printf("Kerem valasszon az alabbi lehetosegek kozul:\n\n");
    printf("\t1. Rendezveny megadasa\n");
    printf("\t2. Rendezveny torlese\n");
    printf("\t3. Rendezvenyek listazasa\n");
    printf("\t4. Latogato regisztralasa\n");
    printf("\t5. Latogato torlese\n");
	printf("\t6. Latogato modositasa\n");
	printf("\t7. Jelentkezettek listazasa\n");
    printf("\t8. Kilepes a programbol\n\n");    
    printf("Kerem adja meg a valasztott funkcio szamat: ");
    scanf("%d", &selection);
     
    return selection;
}

void initRegistration(){
    FILE *tmp1;
	tmp1 = fopen("registrations.txt", "w+");
	
	// Elore beegetett adatok
	fprintf(tmp1, "2,Kiss,Béla,kissbela@email.com,Fri Apr 01 15:37:12 2016\n");
	fprintf(tmp1, "1,Nagy,Géza,nagygeza@email.com,Sat Apr 02 10:24:54 2016\n");
    fprintf(tmp1, "2,Kovacs,Ferenc,kovacsferenc@email.com,Fri Apr 01 15:40:12 2016\n");
    fprintf(tmp1, "2,Kiss,István,kissistvan@email.com,Sat Nov 05 17:10:13 2016\n");
	
	fclose(tmp1);
}

void initEvent(){
    FILE *tmp1;
	tmp1 = fopen("event.txt", "w+");
	
	// Elore beegetett adatok
	fprintf(tmp1, "1,EskuvoKiallitas\n");
	fprintf(tmp1, "2,FilmHet\n");
	
	fclose(tmp1);
}

void init() {
    int initFile1 = open("registrations.txt", O_RDONLY);
	int initFile2 = open("event.txt", O_RDONLY);

	close(initFile1);
	close(initFile2);

	if(initFile1 < 0 || initFile2 < 0) {
		if (initFile1 < 0) {
			initRegistration();
		}
		
		if (initFile2 < 0) {
			initEvent();
		}
	}
}

void next() {
	printf("\n\nA folytatashoz kerem nyomjon meg egy gombot...");
	getchar();
}

void addEvent() {
	char newEvent[256] = "";
	int eventAlredyExists = 0;
	
	printf("Adja meg a rendezveny nevet!: ");
	getchar();
	fgets(newEvent, sizeof(newEvent), stdin);

	FILE *events = fopen("event.txt", "a+");
	char currentLine[256];
	int c = 0;
	
	while (fgets(currentLine, sizeof(currentLine), events) && eventAlredyExists == 0) {
		if (strcmp(newEvent, currentLine) == 0) {
			eventAlredyExists = 1;
		}
		++c;
	}
	
	if (eventAlredyExists == 0) {
		printf("A rendezveny listahoz adasa sikerult.\n\n");

		fprintf(events, newEvent);
		fclose(events);
		return;

	} else {
		fclose(events);
		printf("A megadott rendezveny mar szerepel a listaban! \nNem tortent valtozas.\n\n");
		return;
	}
}

void removeEvent() {
    char removeEvent[256] = "";
	int eventRemoved = 0;
	//fflush(stdin);
	
	printf("Kerem adja meg a listarol eltavolitando rendezveny nevet: ");
	getchar();
	fgets(removeEvent, sizeof(removeEvent), stdin);
	char currentLine[256] = "";

	printf(removeEvent);
	
	FILE *events = fopen("event.txt", "r");
	FILE *new_events = fopen("event_temp.txt", "a+");
		
	while (fgets(currentLine, sizeof(currentLine), events)) {
		if (strcmp(removeEvent, currentLine) != 0) {
			fprintf(new_events, currentLine);
		} else {
			eventRemoved = 1;
		}
	}

	fclose(events);
	fclose(new_events);
		
	remove("event.txt");
	rename("event_temp.txt", "event.txt");
		
	if (eventRemoved == 0) {
		printf("Nem talalhato rendezveny ilyen nevvel a listaban! \nNem tortent valtozas.\n\n");
	} else {
		printf("Rendezveny eltavolitva\n\n");
	}
}

void listEvent() {
    getchar();
	FILE *events = fopen("event.txt", "r");
	char currentLine[256];
	int i = 1;
	printf("Rendezvenyek:\n\n");

	while (fgets(currentLine, sizeof(currentLine), events)) {

		printf("%d. %s", i, currentLine);
		i++;
	}

	fclose(events);
}

void eventRegistration() {
	listEvent();
	FILE *events = fopen("event.txt", "r");
	FILE *registrations = fopen("registrations.txt", "a+");
	
	char eventName[256] = "";
	char firstName[64] = "";
	char lastName[64] = "";
	char email[32] = "";
	
	char currentLine[256];
	int eventExists = 0;

	fflush(stdin);
	printf("Kerem adja meg a rendezveny azonositojat: ");
	scanf("%s", &eventName);
	
	int c = 0;
	while (fgets(currentLine, sizeof(currentLine), events) && eventExists == 0) {
		int i = strlen(currentLine) - 1;
		currentLine[i] = '\0';

		if (strcmp(eventName, currentLine) == 0) {
			eventExists = 1;
		}
		++c;
	}
	
	if (eventExists == 1) {
		printf("\nFoglalas: %s\n\n", eventName);
		printf("Kerem adja meg a vezeteknevet: ");
		scanf("%s", &firstName);
		printf("Kerem adja meg a keresztnevet: ");
		scanf("%s", &lastName);
		printf("Kerem adja meg a email cimet: ");
		scanf("%s", &email);
		
		time_t currentTime;
		char* c_time_string;
	
		currentTime = time(NULL);
	
		fprintf(registrations, "%s,%s,%s,%s,%s", eventName, firstName, lastName, email, ctime(&currentTime));
		getchar();

	} else {
		fclose(events);
		fclose(registrations);
		getchar();

		printf("\nA megadott rendezveny nem szerepel az uticelok listaban! A foglalas sikertelen.\n\n");
		return;
	}

	fclose(events);
	fclose(registrations);
	printf("\nSikeres foglalas %i. -ként!\n\n");
}

void searchDatabase() {
	char search[256] = "";
	int lineCounter = 0;
	char *ptr;
	int i;
	int newLineCharReplace;

	fflush(stdin);
	printf("Kerem adjon meg egy rendezvenyt, nevet, emailcimet a szukiteshez: ");

	fgets(search, sizeof(search), stdin);
	newLineCharReplace = strlen(search) - 1;
	search[newLineCharReplace] = '\0';

	char currentLine[256];
	FILE *registrations = fopen("registrations.txt", "r");	

	while (fgets(currentLine, sizeof(currentLine), registrations)) {
		lineCounter++;
		if (strstr(currentLine, search) != 0) {
			ptr = strtok(currentLine, ",");
			for (i = 1; i < 6; i++) {
				if (i == 1) {
					printf("\nFoglalasi azonosito: %d\n\n\tRendezveny: %s", lineCounter, ptr);
				} else if (i == 2) {
					printf("\n\tNev: %s", ptr);
				} else if (i == 3) {
					printf(" %s", ptr);
				} else if (i == 4) {
					printf("\n\tEmail: %s", ptr);
				} else {
					printf("\n\tFoglalas ideje: %s", ptr, "\n");
				}
				ptr = strtok(NULL, ",");
			}
		}
	}
	fclose(registrations);
}

void removeRegistration() {
    getchar();
	int lineToRemove;
	int guestRemoved = 0;
	char currentLine[256];
	searchDatabase();
	fflush(stdin);
		
	printf("\nKerem adja meg a torolni kivant latogato azonositojat (Ha meggondolta magat, kerem nyomjon '0'-t): ");
	scanf("%d", &lineToRemove);
		
	if (lineToRemove == 0) {
		return;
	}
	
	int lineCounter = 0;
	
	FILE *NEW_registrations = fopen("registrations_temp.txt", "a+");
	FILE *OLD_registrations = fopen("registrations.txt", "r");
	
	while (fgets(currentLine, sizeof(currentLine), OLD_registrations)) {
		lineCounter++;
		if (lineToRemove != lineCounter) {
			fprintf(NEW_registrations, currentLine);
		} else {
			guestRemoved = 1;
		}
	}
	fclose(OLD_registrations);
	fclose(NEW_registrations);
		
	remove("registrations.txt");
	rename("registrations_temp.txt", "registrations.txt");
	
	if (guestRemoved == 1) {
		printf("A foglalas sikeresen torolve.\n\n");
	} else {
		printf("Foglalas nem talalhato.\n\n");
	}
	getchar();
}

void modifyRegistration() {
	getchar();
	int lineToModify;
	searchDatabase();
	fflush(stdin);
	
	printf("Kerem adja meg a szerkeszteni kivant latogato azonositojat (Ha meggondolta magat, kerem nyomjon '0'-t): ");
	scanf("%d", &lineToModify);
	
	if (lineToModify == 0) {
		return;
	}
	
	int lineCounter = 0;
	int guestModified = 0;
	char currentLine[256];
	
	FILE *NEW_registrations = fopen("registrations_temp.txt", "a+");
	FILE *OLD_registrations = fopen("registrations.txt", "r");
	
	while (fgets(currentLine, sizeof(currentLine), OLD_registrations)) {
		lineCounter++;
		if (lineToModify != lineCounter) {
			fprintf(NEW_registrations, currentLine);
		} else {
			char newEvent[256] = "";
			char newFirstName[256] = "";
			char newLastName[256] = "";
			char newEmal[256] = "";
			
			printf("Kerem adja meg az uj rendezvenyt: ");
			scanf("%s", &newEvent);
			printf("Kerem adja meg a vezeteknevet: ");
			scanf("%s", &newFirstName);
			printf("Kerem adja meg a keresztnevet: ");
			scanf("%s", &newLastName);
			printf("Kerem adja meg a emailcimet: ");
			scanf("%s", &newEmal);
			
			time_t currentTime;
			char* c_time_string;
	
			currentTime = time(NULL);
			fprintf(NEW_registrations, "%s,%s,%s,%s, Modositva: %s\n", newEvent, newFirstName, newLastName, newEmal, ctime(&currentTime));
			guestModified = 1;
		}
	}

	fclose(OLD_registrations);
	fclose(NEW_registrations);
		
	remove("registrations.txt");
	rename("registrations_temp.txt", "registrations.txt");
	
	if (guestModified == 0) {
		getchar();
		printf("Nem tortent modositas a latogaton.\n\n");
		next();
	} else {
		getchar();
		printf("A latogato modositasa sikeres.\n\n");
		next();
	}
}

void listRegistration() {
	getchar();
	FILE *registrations = fopen("registrations.txt", "r");
	char currentLine[256];
	int i = 1;
	printf("Registralasok: \n\n");

	while(fgets(currentLine, sizeof(currentLine), registrations)) {
		printf("%d. %s", i, currentLine);
		i++;
	}
	fclose(registrations);
}

int main(int argc,char ** argv) {
    init();

    int selection = menu();
	
	while (selection != 6) {
		switch(selection) {
			case 1:
				addEvent();
				next();
				break;
			case 2:
				removeEvent();
				next();
				break;
			case 3:
				listEvent();
				next();
				break;
			case 4:
				eventRegistration();
				next();
				break;
			case 5:
				removeRegistration();
				next();
				break;
			case 6:
				modifyRegistration();
				next();
				break;
			case 7:
				listRegistration();
				next();
				break;
			case 8:
				return 0;
			default:
				printf("\n\nA megadott szam ervenytelen, kerem probalja ujra!\n\n");
				break;
		}
		selection = menu();
	}
}