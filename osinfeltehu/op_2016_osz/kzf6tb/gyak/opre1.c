#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //open,create
#include <time.h>
//#include <sys/types.h>

#define EVENTLIMIT 100
#define GUESTLIMIT 50

struct Guest {
    char name[100];
    char email[100];
    time_t time;
};

struct Event {
    int guestnum;
    struct Guest* guests;
};

int eventnum = 0;
struct Event events[EVENTLIMIT];

int isValidEventId ( int id ) {
    return (eventnum > 0 && id >= 0 && id < eventnum);
}

int askForEventId () {
    char strEventid[1];

    printf("Add meg az esemeny azonoitojat: \n");
    scanf("%s", strEventid);

    int eventid = atoi(strEventid);
    if ((eventid || *strEventid == '0') && isValidEventId(eventid))
        return eventid;
    else
        return -1;
}

void addGuest() {
    int eventid = askForEventId ();
    if (eventid <= -1) {
        printf("Hibas azonosito!");
        return;
    }

    if (events[eventid].guestnum >= GUESTLIMIT) {
        printf("Minden hely megtelt...");
        return;
    }

    char name[100];
    char email[100];

    printf("Nev: \n");
    scanf("%s", name);

    printf("Email: \n");
    scanf("%s", email);

    int guestnum = events[eventid].guestnum++;

    strcpy(events[eventid].guests[guestnum].name, name);
    strcpy(events[eventid].guests[guestnum].email, email);
    time(&events[eventid].guests[guestnum].time);

}

void listGuests () {
    int eventid = askForEventId ();
    if (eventid <= -1) {
        printf("Hibas azonosito!");
        return;
    }

    printf("Jelentkezettek listaja:\n");
    int ii;
    for ( ii = 0; ii < events[eventid].guestnum; ++ii ) {
        char* time = ctime(&events[eventid].guests[ii].time);
        time[ strlen(time)-1 ] = 0;
        printf("  %i. jelentkezo: %s (email: %s, ido: %s)\n", ii+1, events[eventid].guests[ii].name, events[eventid].guests[ii].email, time);
    }
}

void editGuest() {
    int eventid = askForEventId ();
    if (eventid <= -1) {
        printf("Hibas azonosito!");
        return;
    }

    char strGuestid[1];

    printf("Add meg a vendeg sorszamat: \n");
    scanf("%s", strGuestid);

    int guestid = atoi(strGuestid);
    if ((guestid || *strGuestid == '0') && guestid > 0 && guestid <= events[eventid].guestnum ) {
        guestid--;
        printf("Vendeg jelenlegi adatai: %s (%s)\n\n", events[eventid].guests[guestid].name, events[eventid].guests[guestid].email);
        printf("Uj adatok:\n");

        char name[100];
        char email[100];

        printf("Nev: \n");
        fgets(name,100,stdin);

        printf("Email: \n");
        scanf("%s", email);

        strcpy(events[eventid].guests[guestid].name, name);
        strcpy(events[eventid].guests[guestid].email, email);
    } else {
         printf("Hibas vendeg azonosito!\n");
    }
}

void delGuest() {
    int eventid = askForEventId ();
    if (eventid <= -1) {
        printf("Hibas azonosito!");
        return;
    }

    char strGuestid[1];

    printf("Add meg a vendeg sorszamat: \n");
    scanf("%s", strGuestid);

    int guestid = atoi(strGuestid);
    if ((guestid || *strGuestid == '0') && guestid > 0 && guestid <= events[eventid].guestnum) {
        guestid--;
        if (events[eventid].guestnum > 1)
            events[eventid].guests[guestid] = events[eventid].guests[ events[eventid].guestnum -1 ];

        events[eventid].guestnum--;
    }
}

void addEvent() {
    if (eventnum >= EVENTLIMIT){
        printf("Tobb esemeny nem adhato hozza!\n");
        return;
    }

    int iNewEventId = eventnum++;
    events[iNewEventId].guests = (struct Guest*)malloc(GUESTLIMIT*sizeof(struct Guest));
    events[iNewEventId].guestnum = 0;

    printf("Esemeny hozzaadva, azonositoja: %i\n", iNewEventId);
}

void delEvent () {
    int eventid = askForEventId ();

    if (eventid > -1) {
        if (isValidEventId(eventid)) {
            events[eventid].guestnum = 0;
            events[eventid].guests = realloc(events[eventid].guests,(GUESTLIMIT*sizeof(struct Guest)));
            printf("A torles sikeres!\n");

            return;
        }
    }

    printf("A torles sikertelen!\n");
}

void saveData () {
    FILE* f;
    f = fopen("out.txt", "w");

    if (f == NULL){
        printf("##Hiba a mentésnél!!!");
        return;
    }

    fwrite( &eventnum, sizeof(int), 1, f);

    int i;
    for (i = 0; i < eventnum; i++) {
        fwrite( &events[i].guestnum, sizeof(int), 1, f);
        fwrite( events[i].guests, sizeof(*events[i].guests), events[i].guestnum, f);
    }

    fclose(f);
}

void loadData () {
    FILE* f;
    f = fopen("out.txt", "r");

    if (f == NULL){
        printf("##Betöltés sikertelen");
        return;
    }

    fread(&eventnum, sizeof(int),1, f);
    int i;
    for (i = 0; i < eventnum; i++) {
        fread( &events[i].guestnum, sizeof(int), 1, f);
        events[i].guests = (struct Guest*)malloc(20*sizeof(struct Guest));
        fread( events[i].guests, sizeof(*events[i].guests), events[i].guestnum, f);
    }
}

int main () {
    loadData();

	char choice[1];
	do {
        printf("\n\n");
		printf("1.  Jelentkezes\n\n");

		printf("2. Jelentkezettek listazasa\n\n");

		printf("3. Vendeg modositasa\n\n");
		printf("4. Vendeg torlese\n\n");

		printf("5. Rendezveny hozzadasa\n");
		printf("6. Rendezveny torlese\n\n");

		printf("\n7. Kilepes\n");

		scanf("%s", choice);
		switch (*choice) {
			case('1'):
                addGuest ();
				break;
			case('2'):
                listGuests();
				break;

			case('3'):
                editGuest();
				break;

			case('4'):
                delGuest();
				break;

			case('5'):
                addEvent ();
				break;

			case('6'):
                delEvent();
				break;
		}

		saveData();
	} while (*choice != '7' );

	return 1;
}
