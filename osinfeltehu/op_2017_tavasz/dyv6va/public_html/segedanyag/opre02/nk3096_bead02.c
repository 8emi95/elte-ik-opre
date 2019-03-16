//Pinter Arianna NK3096 2. beadando

#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define P1_READ		0	//parent reads from
#define P2_WRITE	1	//child writes to
#define	P2_READ		2	//child reads from
#define	P1_WRITE	3	//parent writes to

#define NUM_PIPES	2 	//number of pipe pairs

#define STAYHOME	10	//hany szazalek alatt maradjanak otthon a resztvevok (<=)

typedef struct {
	char name[30];
	char email[30];
	char eventID[10];
	char date[30];
} visitorData;

typedef struct {
	char name[30];
	char eventID[10];
	char date[30];
	char place[30];
} eventData;

void handler(int signumber){
	printf("[GY] Megerkeztem, keszen allok a rendezveny lebonyolitasara!\n");
}

int getRandomNumber(int div){
	return rand() % div;
}

int handleEvent(int visitorSize, visitorData* vdata, int eventSize, eventData* edata){
	printf("-- Rendezveny lebonyolitasa --\n");
	srand(time(NULL));
	int fd[2 * NUM_PIPES];
	int val = 0, len, i;
	pid_t pid;
	signal(SIGUSR1,handler);

	for(i = 0; i < NUM_PIPES; i++){
		if ( pipe(fd + (i * 2) ) < 0){
			perror("Failed to allocate pipes");
			exit(EXIT_FAILURE);
		}
	}

	//megkeressuk a bekert varost
    printf("Keresett varos: ");
	char wantedPlace[10]; scanf("%s", wantedPlace);
	int j; int found = -1;
	for(j = 0; j < eventSize; j++){
		if( strcmp(edata[j].place, wantedPlace) == 0 ){
			found = j; break;
		}
	}

	if (found > -1) {
		pid = fork();
		if (pid < 0) { perror("Hiba tortent a fork kozben!"); return EXIT_FAILURE;}

	//gyerek
		if( pid == 0 ) { //gyerek

		//lezarjuk a nem hasznalt vegeket
			close(fd[P1_READ]);
		    close(fd[P1_WRITE]);

		//gyerek kiolvassa a varost (string)
	    	char city[30] = "";
	    	len = read(fd[P2_READ], &city, sizeof(city));
	    	if(len < 0){ perror("[GY] Hiba tortent az adat olvasasa kozben!"); exit(EXIT_FAILURE);} 
	    	else if (len == 0) { fprintf(stderr, "Child: Read EOF from pipe - city"); } 

	    	printf("[GY] Elindulok a kovetkezo helyszinre: %s ...\n", city);
		    sleep(2);
		
		//gyerek signalt kuld, hogy johetnek az adatok
		    kill(getppid(), SIGUSR1);

		//gyerek kiolvassa a resztvevok szamat (int)
	    	int tmp = 0;
	    	len = read(fd[P2_READ], &tmp, sizeof(int));
	    	if(len < 0) { perror("[GY] Hiba tortent az adat olvasasa kozben! - resztvevok szama\n"); exit(EXIT_FAILURE); }
	    	else if (len == 0) fprintf(stderr, "Child: Read EOF from pipe - eventVisitorsSum\n");

	    	int eventVisitorsSum = tmp;

	    //ha vannak resztvevok, megyunk tovabb
	    	if(eventVisitorsSum > 0 ) {

	    	//gyerek kiolvassa a resztvevok listajat (array)
	    		char visitors[eventVisitorsSum][100];
		    	int k;
		    	for(k = 0; k < eventVisitorsSum; k++) {
		    		read(fd[P2_READ], &visitors[k], sizeof(visitors[k]) );
		    	} 

		    //gyerek konzolra irja a kapott listat
		    	printf("[GY] A kovetkezo listat kaptam (%d fo): \n", eventVisitorsSum);
		    	for(k = 0; k < eventVisitorsSum; k++) {
		    		printf("- %s\n", visitors[k]);
		    	}
		    	sleep(1);
		    	printf("[GY] A rendezvenyt megnyitjuk ... \n");
		    	sleep(3);

		    //gyerek elkuldi az ertekelest
				int percent = getRandomNumber(101);
				if (write(fd[P2_WRITE], &percent, sizeof(percent)) < 0) {
	                perror("[GY] Hiba tortent az adat irasa kozben! - ertekeles\n");
	                exit(EXIT_FAILURE);
	            }
	            printf("[GY] A rendezvenynek vege, elkuldtem az erteklest!\n");

	        //vendegek es ertekelesek kiszamitasa
		    	int didntComeSum = 0;
		    	char didntCome[eventVisitorsSum][100];
		    	for(j = 0; j < eventVisitorsSum; j++) {
		    		int r = getRandomNumber(101);
		    		if(r <= STAYHOME){
		    			strcpy(didntCome[didntComeSum], visitors[j]);
		    			didntComeSum++;
		    		}
		    	}
		    	sleep(2);

		    //gyerek elkudi, hanyan nem mentek el
	            printf("[GY] Kuldom a nem megjelent vendegek listajat ... \n");
	            if (write(fd[P2_WRITE], &didntComeSum, sizeof(int)) < 0) {
	                perror("[GY] Hiba tortent az adat irasa kozben! - didntComeSum\n");
	                exit(EXIT_FAILURE);
	            }
	            sleep(2);
	            
	        //gyerek elkuldi a meg nem jelent vendegeket (array)
	            for(j = 0; j < didntComeSum; j++) {
	            	if ( write(fd[P2_WRITE], &didntCome[j], sizeof(didntCome[j]) ) < 0) {
	                	perror("[GY] Hiba tortent az adat irasa kozben! - didntCome\n");
	                	exit(EXIT_FAILURE);
	            	}
	            }
	            sleep(3);
	    	}
	    	
	        close(fd[P2_READ]);
	    	close(fd[P2_WRITE]);
	    	return EXIT_SUCCESS;

	//szulo
		} else {

		//lezarjuk a nem hasznalt vegeket
			close(fd[P2_READ]);
		    close(fd[P2_WRITE]);

			printf("Talalat: %s - %s\n", edata[found].name, edata[found].date );
				
		//szulo elkuldi a helysztint
			printf("[SZ] Kuldom a helyszint: %s\n", edata[found].place);
			if (write(fd[P1_WRITE], &edata[found].place, sizeof(edata[found].place)) != sizeof(edata[found].place)){
			    perror("[SZ] Hiba tortent az adat irasa kozben! - helyszin\n");
			    exit(EXIT_FAILURE);
			}

		//szulo megvarja a gyerek jelet
		    pause();

		//szulo osszegyujti a vendegeket
		    printf("[SZ] Kuldom a vendegek listajat...\n");
		    char participants[visitorSize][100];
		    int j;
		    int partSize = 0;
		    for(j = 0; j < visitorSize; j++) {
		    	if(strcmp(vdata[j].eventID, edata[found].eventID) == 0) {
		    		strcpy(participants[partSize], vdata[j].name);
		    		partSize++;
		    	}
		    }    

		//szulo elkuldi a resztvevok szamat
		   	if ( write(fd[P1_WRITE], &partSize, sizeof(int)) != sizeof(int)){
		        perror("[SZ] Hiba tortent az adat irasa kozben! - participants size ");
		        exit(EXIT_FAILURE);
		    }
		   	sleep(1);

		//ha voltak resztvevok, megyunk tovabb
		   	if(partSize > 0) {

		   	//szulo elkuldi a resztvevok listajat
		   		for(j = 0; j < partSize; j++) {
			    	if (write(fd[P1_WRITE], participants[j], (strlen(participants[j])+1)) != (strlen(participants[j])+1) ){
				        perror("[SZ] Hiba tortent az adat irasa kozben! - participants list");
			    	    exit(EXIT_FAILURE);
			   		}
		    		sleep(1);
		    	}

			//szulo kiolvassa azt ertekelest (int)
			    int tmp = 0;
			    len = read(fd[P1_READ], &tmp, sizeof(int));
			    int percent = tmp;
			    if (len < 0) {
			        perror("[SZ] Hiba tortent az adat olvasasa kozben! - ertekeles\n");
			        exit(EXIT_FAILURE);
			    }
			    else if (len == 0){ printf("Parent: read EOF from pipe - ertekeles\n"); }

			    printf("[SZ] Kapott ertekeles: %d%%\n", percent);

		    //szulo kiolvassa a meg nem jelent vendegek szamat (int)
		        int didntComeSum = 0;
		        len = read(fd[P1_READ], &didntComeSum, sizeof(int));
		        if(len < 0) {
		        	perror("[SZ] Hiba tortent az adat olvasasa kozben! - didntComeSum\n");
		        	exit(EXIT_FAILURE);
		        } else if(len == 0) { fprintf(stderr, "Parent: Read EOF from pipe - didntComeSum\n");}
		        sleep(2);

		    //szulo kiolvassa a meg nem jelent vendegeket (array)
		        if(didntComeSum == 0) printf("[SZ] Minden vendeg eljott!\n");
		        else {

		        	printf("[SZ] A kovetkezo vendegek nem jottek el (%d fo): \n", didntComeSum);
		        	char gotDidntCome[didntComeSum][100];
			    	int k;
			    	for(k = 0; k < didntComeSum; k++) {
				    	len = read(fd[P1_READ], &gotDidntCome[k], sizeof(gotDidntCome[k]) );
				    	if(len < 0) {
				        	perror("[SZ] Hiba tortent az adat olvasasa kozben! - didntCome\n");
				        	exit(EXIT_FAILURE);
				        } else if(len == 0) { fprintf(stderr, "Parent: Read EOF from pipe - didntCome\n"); }
				    	printf("- %s\n", gotDidntCome[k]);
				    }

		        }   

		    //kovetkezo rendezveny sorsolasa
		    	char nextEventPlace[20];
		    	strcpy(nextEventPlace, wantedPlace);
		    	int r;
		    	while( strcmp(nextEventPlace, wantedPlace) == 0 ) {
		    		r = getRandomNumber(eventSize);
		    		strcpy(nextEventPlace, edata[r].place);
		    	};
		    	printf("[SZ] A kovetkezo rendezveny: %s (%s)\n", edata[r].name, edata[r].place);
		    	sleep(3);

		   	} else {
		   		printf("[SZ] Senki sem jelentkezett a keresett rendezvenyre!\n");
		   	}

	   	// lezarjuk a nem hasznalt vegeket
		    close(fd[P1_READ]);
		    close(fd[P1_WRITE]);

		// megvárjuk, hogy a gyerek befejezodjon
		    waitpid(pid, NULL, 0);

		    return EXIT_SUCCESS;
		}

	} else {
		printf("Nincs rendezveny az adott varosban!\n");
		exit(EXIT_FAILURE);
	}

}

//Adatok beolvasasa
void readData(int* visitorSize, visitorData** vdata, int* eventSzie, eventData** edata);
//Rendezvenyek kiiratasa
void listEvents(int eventSize, eventData* edata);
//Latogatok kiiratasa
void listVisitors(int visitorSize, visitorData* vdata);

int main(int argc, char** argv) {

	visitorData* v = (visitorData*)malloc(10*sizeof(visitorData));
	eventData* 	 e = (eventData*)malloc(10*sizeof(eventData));

	int eventSize   = 0;
	int visitorSize = 0;

	visitorData *vdata;
	eventData   *edata;

	int *pEventSize   = &eventSize;
	int *pVisitorSize = &visitorSize;
	visitorData **pvdata = &vdata;
	eventData   **pedata = &edata;

	readData(pVisitorSize, pvdata, pEventSize, pedata);

	int choice;

	do {
		sleep(3);
		printf("-- MENU --\n");
		printf("1 - Rendezvenyek listazasa\n");
		printf("2 - Latogatok listazasa\n");
		printf("3 - Rendezveny lebonyolitasa\n");
		printf("0 - Kilepes\n");
		printf("Valasztott menupont: "); scanf("%d", &choice);
		printf("\n");

		switch(choice) {
			case 0: //Kilepes
				break;
			case 1: //Rendezvenyek listazasa
				listEvents(eventSize, edata);
				break;
			case 2: //Latogatok listazasa
				listVisitors(visitorSize, vdata);
				break;
			case 3: //Rendezveny lebonyolitasa
				handleEvent(visitorSize, vdata, eventSize, edata);
				break;
			default:
				printf("Nincs ilyen menupont!\n");
				break;
		}
	} while(choice != 0);

	free(v); free(e);

	return 0;
}

void readData(int* visitorSize, visitorData** vdata, int* eventSize, eventData** edata) {
	FILE* file = fopen("visitors.txt", "r");
	fscanf(file, "%d", visitorSize);
	*vdata = (visitorData*)malloc(*visitorSize*sizeof(visitorData));

	int i;
	for(i = 0; i < *visitorSize; i++){
		//Nev
		fscanf(file, "%s", (*vdata)[i].name);
		char name2[20];
		fscanf(file, "%s", name2);
		strcat((*vdata)[i].name, " ");
		strcat((*vdata)[i].name, name2);

		//Email
		fscanf(file, "%s", (*vdata)[i].email);

		//Event ID
		fscanf(file, "%s", (*vdata)[i].eventID);

		//Datum
		fscanf(file, "%s", (*vdata)[i].date);
		char time[10];
		fscanf(file, "%s", time);
		strcat((*vdata)[i].date, " ");
		strcat((*vdata)[i].date, time);
	}
	fclose(file);

	file = fopen("events.txt", "r");
	fscanf(file, "%d", eventSize);
	*edata = (eventData*)malloc(*eventSize*sizeof(eventData));

	for(i = 0; i < *eventSize; i++) {
		fscanf(file, "%s", (*edata)[i].name);
		fscanf(file, "%s", (*edata)[i].eventID);
		fscanf(file, "%s", (*edata)[i].date);
		fscanf(file, "%s", (*edata)[i].place);
	}
	fclose(file);
}

void listEvents(int eventSize, eventData* edata){
	printf("-- Osszes rendezveny --\n");
	int i;
	for(i = 0; i < eventSize; i++) {
		printf("%s ",   edata[i].name);
		printf("(%s) ", edata[i].eventID);
		printf("%s, ",  edata[i].date);
		printf("%s\n", 	edata[i].place);
	}
	printf("\n");
}

void listVisitors(int visitorSize, visitorData* vdata) {
	printf("-- Osszes latogato --\n");
	int i;
	for(i = 0; i < visitorSize; i++) {
		printf("%d., %s ", 		   i+1, vdata[i].name);
		printf("(%s)\n", 				vdata[i].email);
		printf("rendezveny kod: %s, ", 	vdata[i].eventID);
		printf("jelentkezett: %s\n", 	vdata[i].date);
	}
	printf("\n");
}
