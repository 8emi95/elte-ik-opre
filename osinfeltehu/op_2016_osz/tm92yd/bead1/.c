// Operacios rendszerek - 1. beadando
// Babics Balint, B4XD3V

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CLEAR_SCREEN 40

void clearScreen() {

	int i;

	for (i = 0; i < CLEAR_SCREEN; i++) {

		putchar('\n');
	}
}

int menu() {

	int selection = 0;
	
	clearScreen();
	
	printf("Udvozli a Best of Uber!\n\n");
	printf("Kerem valasszon az alabbi lehetosegek kozul:\n\n");
	printf("\t1. Uj celvaros megadasa\n");
	printf("\t2. Celvaros torlese\n");
	printf("\t3. Celvarosok listazasa\n");
	printf("\t4. Ut foglalasa\n");
	printf("\t5. Foglalas keresese vagy eltavolitasa\n");
	printf("\t6. Foglalas modositasa\n");
	printf("\t7. Foglalasok listazasa\n");
	printf("\t8. Kilepes a programbol\n\n");	
	printf("Kerem adja meg a valasztott funkcio szamat: ");
	scanf("%d", &selection);
	
	return selection;
}

void initDestDatabase() {

	FILE *tmp1;
	tmp1 = fopen("dest_db.txt", "w+");
	
	// Elore beegetett adatok
	fprintf(tmp1, "Budapest\n");
	fprintf(tmp1, "Pecs\n");
	fprintf(tmp1, "Debrecen\n");
	fprintf(tmp1, "Szeged\n");
	
	fclose(tmp1);
}

void initBookDatabase() {

	FILE *tmp1;
	tmp1 = fopen("book_db.txt", "w+");

	// Elore beegetett adatok
	fprintf(tmp1, "Debrecen,Kis,Bela,+36701253845,Fri Apr 01 15:37:12 2016\n");
	fprintf(tmp1, "Pecs,Nagy,Geza,+36301783832,Sat Apr 02 10:24:54 2016\n");
	
	fclose(tmp1);
}

void init() {

	int initFile1 = open("dest_db.txt", O_RDONLY);
	int initFile2 = open("book_db.txt", O_RDONLY);

	close(initFile1);
	close(initFile2);

	if(initFile1 < 0 || initFile2 < 0) {

		if (initFile1 < 0) {

			initDestDatabase();
		}
		
		if (initFile2 < 0) {

			initBookDatabase();
		}
	}
}

void addDest() {

	char newCityName[256] = "";
	int cityAlreadyExists = 0;
	
	printf("Adja meg a celvaros listahoz hozzadni kivant varos nevet!: ");
	getchar();
	fgets(newCityName, sizeof(newCityName), stdin);

	FILE *db_cities = fopen("dest_db.txt", "a+");
	char currentLine[256];
	
	while (fgets(currentLine, sizeof(currentLine), db_cities) && cityAlreadyExists == 0) {

		if (strcmp(newCityName, currentLine) == 0) {

			cityAlreadyExists = 1;
		}
	}
	
	if (cityAlreadyExists == 0) {

		printf("A varos listahoz adasa sikerult.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");

		fprintf(db_cities, newCityName);
		fclose(db_cities);
		getchar();
		return;

	} else {

		fclose(db_cities);
		clearScreen();

		printf("A megadott varos mar szerepel a listaban! \nNem tortent valtozas.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");

		getchar();
		return;
	}
		
}

void removeDest() {
	char removeCityName[256] = "";
	int cityExists = 0;
	int cityRemoved = 0;
	fflush(stdin);
	
	printf("Kerem adja meg a listarol eltavolitando varos nevet: ");
	getchar();
	fgets(removeCityName, sizeof(removeCityName), stdin);
	char currentLine[256];
	
		FILE *db_cities = fopen("dest_db.txt", "r");
		FILE *NEW_db_cities = fopen("dest_db_temp.txt", "a+");
		
		while (fgets(currentLine, sizeof(currentLine), db_cities)) {

			if (strcmp(removeCityName, currentLine) != 0) {

				fprintf(NEW_db_cities, currentLine);

			} else {

				cityRemoved = 1;
			}
		}

		fclose(db_cities);
		fclose(NEW_db_cities);
		
		remove("dest_db.txt");
		rename("dest_db_temp.txt", "dest_db.txt");
		
	if (cityRemoved == 0) {

		printf("Nem talalhato varos ilyen nevvel a listaban! \nNem tortent valtozas.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");
		getchar();

	} else {

		printf("Varos eltavolitva\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");
		getchar();
	}
}

void listDest() {

	getchar();
	FILE *db_cities = fopen("dest_db.txt", "r");
	char currentLine[256];
	int i = 1;
	printf("Elerheto varosok:\n\n");

	while (fgets(currentLine, sizeof(currentLine), db_cities)) {

		printf("%d. %s", i, currentLine);
		i++;
	}

	fclose(db_cities);
	printf("\n\nA folytatashoz kerem nyomjon meg egy gombot...");
	getchar();
	
}

void bookDest() {

	FILE *db_cities = fopen("dest_db.txt", "r");
	FILE *db_bookings = fopen("book_db.txt", "a+");
	
	char cityToBook[256] = "";
	char firstName[64] = "";
	char lastName[64] = "";
	char phoneNumber[32] = "";
	
	char currentLine[256];
	int cityExists = 0;

	fflush(stdin);
	printf("Kerem adja meg a varos nevet, ahova szeretne foglalni: ");
	scanf("%s", &cityToBook);
	
	while (fgets(currentLine, sizeof(currentLine), db_cities) && cityExists == 0) {

		int i = strlen(currentLine) - 1;
		currentLine[i] = '\0';

		if (strcmp(cityToBook, currentLine) == 0) {

			cityExists = 1;
		}
	}
	
	if (cityExists == 1) {

		printf("\nFoglalas: %s\n\n", cityToBook);
		printf("Kerem adja meg a vezeteknevet: ");
		scanf("%s", &firstName);
		printf("Kerem adja meg a keresztnevet: ");
		scanf("%s", &lastName);
		printf("Kerem adja meg a telefonszamat: ");
		scanf("%s", &phoneNumber);
		
		time_t currentTime;
		char* c_time_string;
	
		currentTime = time(NULL);
	
		fprintf(db_bookings, "%s,%s,%s,%s,%s", cityToBook, firstName, lastName, phoneNumber, ctime(&currentTime));
		getchar();

	} else {

		fclose(db_cities);
		fclose(db_bookings);
		getchar();

		printf("\nA megadott varos nem szerepel az uticelok listaban! A foglalas sikertelen.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");

		getchar();
		return;
	}

	fclose(db_cities);
	fclose(db_bookings);

	printf("\nSikeres foglalas!\n\n");
	printf("A folytatashoz kerem nyomjon meg egy gombot...");

	getchar();
}

void searchDatabase() {

	char search[256] = "";
	int lineCounter = 0;
	char *ptr;
	int i;
	int newLineCharReplace;
	fflush(stdin);
	
	printf("Kerem adja meg az uticel nevet, a foglalasi nevet vagy a foglalasi telefonszamot: ");

	fgets(search, sizeof(search), stdin);
	newLineCharReplace = strlen(search) - 1;
	search[newLineCharReplace] = '\0';
	char currentLine[256];
	
	FILE *db_bookings = fopen("book_db.txt", "r");
	
	while (fgets(currentLine, sizeof(currentLine), db_bookings)) {

		lineCounter++;

		if (strstr(currentLine, search) != 0) {

			ptr = strtok(currentLine, ",");

			for (i = 1; i < 6; i++) {

				if (i == 1) {

					printf("\nFoglalasi azonosito: %d\n\n\tUticel: %s", lineCounter, ptr);

				} else if (i == 2) {

					printf("\n\tNev: %s", ptr);

				} else if (i == 3) {

					printf(" %s", ptr);

				} else if (i == 4) {

					printf("\n\tTelefonszam: %s", ptr);

				} else {

					printf("\n\tFoglalas ideje: %s", ptr, "\n");
				}

				ptr = strtok(NULL, ",");
			}
		}
	}

	fclose(db_bookings);
}

void removeBooking() {

	getchar();
	int lineToRemove;
	int bookingRemoved = 0;
	char currentLine[256];
	
	searchDatabase();
		
	fflush(stdin);
		
	printf("\nKerem adja meg a torolni kivant foglalas foglalasi azonositojat (Ha meggondolta magat, kerem nyomjon '0'-t): ");
	scanf("%d", &lineToRemove);
		
	if (lineToRemove == 0) {

		return;
	}
	
	int lineCounter = 0;
	
	FILE *NEW_db_bookings = fopen("book_db_temp.txt", "a+");
	FILE *OLD_db_bookings = fopen("book_db.txt", "r");
	
	while (fgets(currentLine, sizeof(currentLine), OLD_db_bookings)) {

		lineCounter++;

		if (lineToRemove != lineCounter) {

			fprintf(NEW_db_bookings, currentLine);

		} else {

			bookingRemoved = 1;
		}
	}
	fclose(OLD_db_bookings);
	fclose(NEW_db_bookings);
		
	remove("book_db.txt");
	rename("book_db_temp.txt", "book_db.txt");
	
	if (bookingRemoved == 1) {

		getchar();
		printf("A foglalas sikeresen torolve.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");

	} else {

		getchar();
		printf("Foglalas nem talalhato.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");
	}
	
	getchar();
}

void modifyBooking() {

	int lineToModify;
	searchDatabase();
	
	fflush(stdin);
	
	printf("Kerem adja meg a szerkeszteni kivant foglalas goflalasi azonositojat (Ha meggondolta magat, kerem nyomjon '0'-t): ");
	scanf("%d", &lineToModify);
	
	if (lineToModify == 0) {

		return;
	}
	
	int lineCounter = 0;
	int bookingModified = 0;
	char currentLine[256];
	
	FILE *NEW_db_bookings = fopen("book_db_temp.txt", "a+");
	FILE *OLD_db_bookings = fopen("book_db.txt", "r");
	
	while (fgets(currentLine, sizeof(currentLine), OLD_db_bookings)) {

		lineCounter++;

		if (lineToModify != lineCounter) {

			fprintf(NEW_db_bookings, currentLine);

		} else {

			char newDestination[256] = "";
			char newFirstName[256] = "";
			char newLastName[256] = "";
			char newPhoneNumber[256] = "";
			
			printf("Kerem adja meg az uj uticelt: ");
			scanf("%s", &newDestination);
			printf("Kerem adja meg a vezeteknevet: ");
			scanf("%s", &newFirstName);
			printf("Kerem adja meg a keresztnevet: ");
			scanf("%s", &newLastName);
			printf("Kerem adja meg a telefonszamat: ");
			scanf("%s", &newPhoneNumber);
			
			time_t currentTime;
			char* c_time_string;
	
			currentTime = time(NULL);
			fprintf(NEW_db_bookings, "%s,%s,%s,%s, Modositva: %s\n", newDestination, newFirstName, newLastName, newPhoneNumber, ctime(&currentTime));
			bookingModified = 1;
		}
	}

	fclose(OLD_db_bookings);
	fclose(NEW_db_bookings);
		
	remove("book_db.txt");
	rename("book_db_temp.txt", "book_db.txt");
	
	if (bookingModified == 0) {

		getchar();
		printf("Nem tortent modositas a foglalason.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");
		getchar();

	} else {

		getchar();
		printf("A foglalas modositasa sikeres.\n\n");
		printf("A folytatashoz kerem nyomjon meg egy gombot...");
		getchar();
	}
}

void listBooking() {

	getchar();
	FILE *db_bookings = fopen("book_db.txt", "r");
	char currentLine[256];
	int i = 1;
	printf("Foglalasok: \n\n");

	while(fgets(currentLine, sizeof(currentLine), db_bookings)) {

		printf("%d. %s", i, currentLine);
		i++;
	}

	fclose(db_bookings);
	printf("\n\nA folytatashoz kerem nyomjon meg egy gombot...");
	getchar();
}

int main(int argc,char ** argv) {

	init();
	
	int selection = menu();
	
	while (selection != 8) {

		clearScreen();

		switch(selection) {

			case 1:
				addDest();
				break;
			case 2:
				removeDest();
				break;
			case 3:
				listDest();
				break;
			case 4:
				bookDest();
				break;
			case 5:
				removeBooking();
				break;
			case 6:
				modifyBooking();
				break;
			case 7:
				listBooking();
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