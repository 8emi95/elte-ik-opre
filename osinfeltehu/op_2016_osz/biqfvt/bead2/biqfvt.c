/* A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására.

A gyermekfolyamat indulása után csővezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra!

A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására.

Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevők.

Ezt a partner képernyőre írja, majd várja a vendégeket és levezényli a rendezvényt!

A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! (Minden résztvevő 10% valószínűséggel nem érkezik meg!)

Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következő! */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define SIZE 10
#define NUM_EVENTS 2
#define NUM_GUESTS 5

struct guestData {
	char eventId;
	char name[SIZE];
};

struct event {
	char eventId;
	char place[SIZE];
	char startDate[SIZE];
};

struct event events[NUM_EVENTS];
struct guestData guests[NUM_GUESTS];

struct event nextEvent;

int fd1[2];
int fd2[2];
int fd3[2];
int fd4[2];
pid_t child;

void utazik(char place[SIZE]) {
	kill(getppid(), SIGUSR1);
	printf("LEBONYOLITO: %s varosban vagyok. Keszen allok a rendezveny lebonyolitasara.\n", place);
}

void kezelo() {
	char name[SIZE];
	int i, j;
	
	close(fd2[0]);
	
	for(i = 0; i < NUM_GUESTS; i++) {
		if(guests[i].eventId == nextEvent.eventId) {
			for (j = 0; j < SIZE; j++) name[j] = guests[i].name[j];
			//strcpy(name, guests[i].name);
			write(fd2[1], &name, SIZE);
		}
	}
	close(fd2[1]);
	
}

void vendegeket_fogad() {
	char name[SIZE];
	
	close(fd2[1]);
	close(fd3[0]);
	close(fd4[0]);
	
	srand(time(NULL));
	int siker = rand() % 101;
	char siker_str[SIZE];
	sprintf(siker_str, "%d", siker);
    write(fd3[1], &siker_str, SIZE);
	
	int megerkezett;
	printf("LEBONYOLITO: vart vendegek: ");
	srand(time(NULL));
	while( read(fd2[0], &name, SIZE) ) {
		
        printf(name);
		printf("  ");
		
		megerkezett = rand() % 101;
		if(megerkezett < 20) {
			write(fd4[1], &name, SIZE);
		}
    }

	printf("\n");
	close(fd2[0]);
	close(fd3[1]);
	close(fd4[1]);
	
	kill(getppid(), SIGUSR2);

}

void rendezveny_vege() {
	close(fd4[1]);
	char name[SIZE];
	char siker_str[SIZE];
	read( fd3[0], &siker_str, SIZE );
	printf("SZERVEZO: A rendezveny sikeressege: "); printf(siker_str); printf("%\n");
	printf("SZERVEZO: Nem jottek el: ");
	
	while( read(fd4[0], &name, SIZE) ) {
		printf(name);
		printf(" ");
	}
	
	close(fd4[0]);
	printf("\n\n");
}

int main()
{
    
	events[0].eventId = '1';
	events[1].eventId = '2';
	
	strcpy(events[0].place, "Miskolc");
	strcpy(events[1].place, "Eger");
	
	strcpy(events[0].startDate, "16.12.30");
	strcpy(events[1].startDate, "17.01.30");
	
	guests[0].eventId = '1';
	guests[1].eventId = '1';
	guests[2].eventId = '2';
	guests[3].eventId = '2';
	guests[4].eventId = '1';
	
	strcpy(guests[0].name, "Aladar");
	strcpy(guests[1].name, "Bela");
	strcpy(guests[2].name, "Cecil");
	strcpy(guests[3].name, "Denes");
	strcpy(guests[4].name, "Elemer");
	
	nextEvent = events[0];
	int status;
	printf("SZERVEZO: Megbizok valakit az esemeny lebonyolitasaval.\n");
	
	if (pipe(fd1)<0 || pipe(fd2)<0 || pipe(fd3)<0|| pipe(fd4)<0) {
		printf("Cso letrehozasa nem sikerult.");
		exit(EXIT_FAILURE);
	}
	
	child = fork();
	
	if(child > 0) {

		close(fd1[0]);
		write(fd1[1], &nextEvent.place, SIZE);
		close(fd1[1]);
		
		signal(SIGUSR1, kezelo);
		signal(SIGUSR2, rendezveny_vege);
		
		waitpid(child,&status,0);
		
	} else {
		
		char place[SIZE];
		close(fd1[1]);
		read(fd1[0], &place, SIZE);
		close(fd1[0]);
		
		utazik(place);
		vendegeket_fogad();
	}

    return 0;
}