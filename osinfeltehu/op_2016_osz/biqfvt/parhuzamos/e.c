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

#define SIZE 10
#define NUM_EVENTS 2
#define NUM_GUESTS 4

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
pid_t child;

void utazik(char place[SIZE]) {
	sleep(2);
	kill(getppid(), SIGUSR1);
	printf("LEBONYOLITO: %s varosban vagyok. Keszen allok a rendezveny lebonyolitasara.\n", place);
}

void kezelo() {
	char name[SIZE];
	int i;
	
	close(fd2[0]);
	strcpy(name, "Alad");
	write(fd2[1], &name, strlen(name) + 1);
	
	/* for(i = 0; i < 4; i++) {
		if(guests[i].eventId == nextEvent.eventId) {
			strcpy(name, guests[i].name);
			write(fd[1], &name, 5);
		}
	} */
	close(fd2[1]);
	
}

void vendegeket_fogad() {
	char name[SIZE];
	close(fd2[1]);
	printf("LEBONYOLITO: vart vendegek: \n");
	
	read(fd2[0], &name, strlen(name) + 1);
	printf(name);
	
	/* while( read(fd[0], &name, 5) ) {
        printf(name);
		printf(" ");
    } */
	printf("\n");
	close(fd2[0]);
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
	
	strcpy(guests[0].name, "Alad");
	strcpy(guests[1].name, "Bela");
	strcpy(guests[2].name, "Ceci");
	strcpy(guests[3].name, "Dene");
	
	nextEvent = events[0];
	int status;
	printf("King of Stands vagyok. Megbizok valakit az esemeny lebonyolitasaval.\n");
	
	if (pipe(fd1)<0 || pipe(fd2)<0) {
		printf("Cso letrehozasa nem sikerult.");
		exit(EXIT_FAILURE);
	}
	
	child = fork();
	
	if(child > 0) {

		close(fd1[0]);
		write(fd1[1], &nextEvent.place, SIZE);
		close(fd1[1]);
		
		signal(SIGUSR1, kezelo);
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