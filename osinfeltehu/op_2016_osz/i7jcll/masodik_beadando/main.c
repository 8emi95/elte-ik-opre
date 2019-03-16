/*
A "King of Stands" rendezvény szervezõ társaság, 
amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) 
az esemény lebonyolítására. A gyermekfolyamat indulása után 
csõvezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra! 
A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, 
hogy a megadott helyszinen van, várja a résztvevõket,
kész a rendezvény lebonyolítására. Ezután a rendezvényszervezõ 
csõvezetéken elküldi a helyszínen tartózkodó partnernek, 
hogy kik a résztvevõk. Ezt a partner képernyõre írja, 
majd várja a vendégeket és levezényli a rendezvényt! 
A rendezvény végén jelentést küld vissza, amiben tájékoztatja 
a fõszervezõt, hogy mennyire sikeres volt a rendezvény (véletlenszám), 
illetve a jelentkezettek közül végül is ki nem érkezett meg! 
(Minden résztvevõ 10% valószínûséggel nem érkezik meg!) 
Miután egy esemény lebonyolításra került, "King of Stands" 
újra megvizsgálja, melyik rendezvény a következõ!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

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
void startEvent();

struct Application {
	char name[NAME_SIZE];
	char mail[MAIL_SIZE];
	char eventID[EVENT_ID_SIZE];
};

struct Event {
	char name[NAME_SIZE];
	char eventID[EVENT_ID_SIZE];
};

struct Message {
	long mtype;
	char mtext[1024];
};

key_t kulcs; 

int main (int argc, char** argv) {
	printf("Welcome to \"King of Stands\"!\n");
	kulcs = ftok(argv[0],1);
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
	printf("8: Rendezveny inditasa\n");
	printf("Egyeb: kilepes\n");
	printf("Valasztas: ");
	char i;
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
	else if(i == 8) {
		startEvent();
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
	char tmpLine[APPLICATION_SIZE];
	while(fgets(event, sizeof(event), appFile)) {
		strcpy(tmpLine, event);
		int comp = strcmp(eventID, strtok(tmpLine, ","));
		if(comp == 0) {
			//in this case we delete it (so do nothing here)
		}
		else {
			fprintf(newAppFile, "%s", &event);
		}
	}
	fclose(appFile);
	fclose(newAppFile);
	remove("applications.txt");
	rename("applications2.txt", "applications.txt");
}

void sig_usr(int signo){
	if(signo == SIGINT) {
		printf("Partner megerkezett a rendezveny helyszinere!\n");
	}
	return;
}

void startEvent() {
	char eventID[EVENT_ID_SIZE];
	//list to select from
	listEvents();
	printf("\nInditando rendezveny szama: ");
	int i;
	scanf("%d", &i);
	FILE *f = fopen("events.txt", "r");
	char tmp[APPLICATION_SIZE];
	int line = 0;
	while(fgets(tmp, sizeof(tmp), f)) {
		line++;
		if(line == i) {
			//copy the code of the event
			strcpy(eventID, strtok(tmp, ","));
		}
	}
	fclose(f);                
	pid_t ppid;
	ppid = getpid();
	struct sigaction sig;               
    sigemptyset(&sig.sa_mask);          
    sig.sa_flags = 0;                   
    sig.sa_handler = sig_usr;
	int msgq, status; 
	msgq = msgget(kulcs, 0600 | IPC_CREAT); 
	if (msgq < 0) { 
		perror("Uzenet fogadasi hiba!"); 
		exit(EXIT_FAILURE);
	}
	int pipefd[2];
	pid_t pid;
	if (pipe(pipefd) == -1) {
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) { //child process
		char childEventID[EVENT_ID_SIZE];
		close(pipefd[1]);
		read(pipefd[0], childEventID, sizeof(childEventID)); //get starting event ID from parent
		printf("\nPartner megkapta az inditando rendezveny kodjat: %s\nElindul...\n", childEventID);
		sleep(3); //wait a bit before notifying parent
		kill(ppid, SIGINT); //arrived to event
		char participants[1024];
		read(pipefd[0], participants, sizeof(participants)); //get participant list
		printf("\nAtvett erkezo vendeg lista: %s\n", participants);
		sleep(2); //wait for the event to complete
		char * pch;
		char eventRateAndPeopleNotArrivesMessage[1024] = "";
		//add rating of event
		char rate[20];
		int r=rand()%10;
		sprintf(rate, "%s rendezveny ertekelese: %d\n", childEventID, r);
		strcat(eventRateAndPeopleNotArrivesMessage, rate);
		//add list of people who did not attend at the event
		strcat(eventRateAndPeopleNotArrivesMessage, "Meg nem jelent vendegek:\n");
		pch = strtok(participants, ",");
		while (pch != NULL) {
			if(5 == rand()%10) {
				char notAttend[NAME_SIZE];
				sprintf(notAttend, "%s", pch);
				strcat(notAttend, "\n");
				strcat(eventRateAndPeopleNotArrivesMessage, notAttend);
			}
			pch = strtok(NULL, ",");
		}
		close(pipefd[0]);
		send(msgq, eventRateAndPeopleNotArrivesMessage); //now send participant list to child
	} 
	else {    //parent process 
		close(pipefd[0]);
		write(pipefd[1], eventID, sizeof(eventID)); //send ID of starting event to child process
		//wait for signal
		if(sigaction(SIGINT,&sig,NULL) == 0)
			pause();
		// get list of participants
		FILE *f = fopen("applications.txt", "r");
		char line[APPLICATION_SIZE];
		char participants[1024] = "";
		char *token;
		int first = 0;
		while(fgets(line, sizeof(line), f)) {
			char event[EVENT_ID_SIZE];
			char name[NAME_SIZE];
			token = strtok(line, ",");
			sprintf(event, "%s", token);
			token = strtok(NULL, ",");
			sprintf(name, "%s", token);
			int comp = strcmp(eventID, event);
			if(comp == 0) {
				//this means that this participant will attend at the event specified	
				if(first != 0) {
					strcat(participants, ",");
				}
				strcat(participants, name);
				first = 1;
			}
		}
		fclose(f);
		//end of list preparation
		write(pipefd[1], participants, sizeof(participants)); //send participant list
		close(pipefd[1]);
		sleep(3);
		receive(msgq); //get the list of participants
		kill(pid, SIGTERM);
		mainMenu();
	}
}

int send(int msgq, char text[1024]) {
	const struct Message msg = {1, ""};
	strcpy(msg.mtext, text);
	int status;
	status = msgsnd(msgq, &msg, strlen(msg.mtext)+1, 0); 
	if ( status < 0 ) {
		perror("Uzenet kuldesi hiba!"); 
	}
}

int receive(int msgq) { 
    struct Message msg; 
    int status;
    status = msgrcv(msgq, &msg, 1024, 1, 0); 
    if ( status < 0 ) {
        perror("Uzenet kuldesi hiba!"); 
	}
	else {
		printf("%s", msg.mtext);
	}
} 