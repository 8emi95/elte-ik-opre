#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct visitor {
	char name[80], emailaddress[80];
	int eventid, isremoved;
	time_t time;
} visitorbuffer;

struct event {
	char name[80];
	int id, isremoved;
} eventbuffer;

void printmenu() {
	puts("\nKérem, válasszon az alábbi lehetőségek közül!\n");
       	puts("1 - látogató jelentkezése a nyilvántartásba");
	puts("2 - vendégek listázása");
	puts("3 - vendég adatainak módosítása");
	puts("4 - vendég adatainak törlése");
	puts("5 - új rendezvény indítása");
	puts("6 - rendezvény összes adatának törlése");
	puts("0 - kilépés\n");
	return;
}

int invalid(int c) {
	return c<'0' || c>'6';
}

int empty(char *s) {
	return *s == '\n' || *s == '\0'; /* *s==s[0] */
}

int notemail(char *s) {
	if (empty(s)) return 1;
	else {
		int i;
		for (i=0; !empty(i+s); ++i) {
			if (i[s] == '@') return 0;
		}
		return 1;
	}
}

void removenewline(char *s) {
	for (; !empty(s); ++s) ; /* egyesével léptetjük előre, hogy hanyadik legyen a kezdő karakter */
	*s='\0'; /* a '\n'-t kivesszük */
}

void clearkeyboard() {
	char dummy;
	do {
		dummy = getchar();
	} while (dummy != '\n' && dummy != EOF);
}

void application() {
	printf("Legyen szíves, adja meg a látogató nevét (ne legyen üres): ");
	while(empty(fgets(visitorbuffer.name, sizeof(visitorbuffer.name), stdin))) ; /*amíg a beolvasott 0. karakter '\0' vagy '\n'*/
	removenewline(visitorbuffer.name);
	printf("Kérem, adja meg a vendég e-mail címét (ne legyen üres, és legyen benne @): ");
	while (notemail(fgets(visitorbuffer.emailaddress, sizeof(visitorbuffer.emailaddress), stdin))) ;
	removenewline(visitorbuffer.emailaddress);
	printf("Kérem, adja meg a rendezvény azonosító számát: ");
	while (scanf("%d", &visitorbuffer.eventid)!=1) clearkeyboard();
	clearkeyboard();
	visitorbuffer.isremoved=0;
	visitorbuffer.time=time(NULL);
	if (((time_t)-1)==visitorbuffer.time) {
		(void) fprintf(stderr, "Almásrétes! Nem sikerült elérni a rendszeridőt!");
	}
}

void list() {
}

void modify() {
}

void deleteguest() {
}

void newevent() {
}

void eraseevent() {
}

int main() {
	int choice;
	do {
		printmenu();
		while (invalid(choice=getchar())) ;
		clearkeyboard();
		puts("");
		switch (choice) {
			case '1':
				application();
				break;
			case '2':
				list();
				break;
			case '3':
				modify();
				break;
			case '4': 
				deleteguest();
				break;
			case '5':
				newevent();
				break;
			case '6':
				eraseevent();
				break;
			case '0':
				break;
			default:
				puts("Érvénytelen választás!");
		}
	} while (choice!='0');
	return 0;
}
