#ifndef BEAD1_H
#define BEAD1_H
#define FIELDSIZE 80
#define BUFFERSIZE 200
#define ARRAYSIZE 1000
#include <time.h>

void manageUser();
void manageEvent();
void listUsers();
void listEvents();
void addUser();
void addEvent();
void delUser();
void delEvent();
void modUser();
void delUsersByEventId();

void dU();
void dE();

struct user{
	char name[FIELDSIZE];
	char mail[FIELDSIZE];
	char eventname[FIELDSIZE];
	char time[FIELDSIZE];
};
struct event{
	char name[FIELDSIZE];
};

struct user users[ARRAYSIZE];
int usercount = 0;
struct event events[ARRAYSIZE];
int eventcount = 0;

char input[FIELDSIZE];

void clearChars(){
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
};

void clearInput(){
	input[0]='\0';
}

void readline(){
	int counter = 0;
	char c;
	while ((c = getchar())!='\n' && counter<FIELDSIZE-1){
		input[counter] = c;
		counter++;
	}
	input[counter] = '\0';
}	
void manageUser(){
	do{
		system("clear");
		printf("FELHASZNÁLÓKEZELÉS\n");
		printf("---------------------\n");
		printf("list: felhasználók listája\n");
		printf("add: felhasználó hozzáadás\n");
		printf("del: felhasználó törlése\n");
		printf("mod: felhasználó módosítása\n");
		printf("x: vissza\n");
		printf("---------------------\n");
		readline();
		if (strcmp(input, "list") == 0){
			listUsers();
		} else if (strcmp(input, "add") == 0){
			addUser();
			clearInput();
		} else if (strcmp(input, "del") == 0){
			delUser();
			clearInput();
		} else if (strcmp(input, "mod") == 0){
			modUser();
			clearInput();
		}
		
	} while (strcmp(input, "x") != 0);
}

void manageEvent(){
	do{
		system("clear");
		printf("ESEMÉNYKEZELÉS\n");
		printf("---------------------\n");
		printf("list: események listája\n");
		printf("add: esemény hozzáadása\n");
		printf("del: esemény törlése\n");
		printf("x: vissza\n");
		printf("---------------------\n");
		readline();
		
		if (strcmp(input, "list") == 0){
			listEvents();
		} else if (strcmp(input, "add") == 0) {
			addEvent();
		} else if (strcmp(input, "del") == 0){
			delEvent();
			clearInput();
		}
		
	} while (strcmp(input, "x") != 0);
}

void delUser(){
	clearInput();
	while(strcmp(input, "x") != 0){
		system("clear");
		printf("Felhasználó törlése\n");
		printf("---------------------\n");
		printf("x: visszalépés az előző menübe\n\n");
		printf("Sorszám: ");
		readline();
		int t = atoi(input);
		if (t > 0 && t <= usercount){
			dU(t-1);
			//users[t-1].name[0] = '\0';
			printf("---------------------\n");
			printf("Sikeres törlés\n");
			printf("---------------------\n");
			getchar();
			input[0] = 'x';
			input[1] = '\0';
		}
	}
}

void delEvent(){
	clearInput();
	while(strcmp(input, "x") != 0){
		system("clear");
		printf("Esemény törlése\n");
		printf("---------------------\n");
		printf("x: visszalépés az előző menübe\n\n");
		printf("Sorszám: ");
		readline();
		int t = atoi(input);
		if (t > 0 && t <= eventcount){
			delUsersByEventId(t-1);
			dE(t-1);
			//events[t-1].name[0] = '\0';
			printf("---------------------\n");
			printf("Sikeres törlés\n");
			printf("Minden felhasználót töröltünk, aki az eseményhez kapcsolódott\n");
			printf("---------------------\n");
			getchar();
			input[0] = 'x';
			input[1] = '\0';
		}
	}
}

int calcUserByEvent(int id){
	int i = 0;
	int c = 0;
	while (i<usercount){
		if (strcmp(users[i].eventname, events[id].name) == 0) c++;
		i++;
	}
	return c;
}

void modUser(){
	clearInput();
	while(strcmp(input, "x") != 0){
		system("clear");
		printf("Felhasználó módosítása\n");
		printf("---------------------\n");
		printf("x: visszalépés az előző menübe\n\n");
		printf("Sorszám: ");
		readline();
		int t = atoi(input);
		if (t > 0 && t <= usercount){
			t--;
			printf("Név: ");
			readline();
			strcpy(users[t].name, input);
			printf("E-mail: ");
			readline();
			strcpy(users[t].mail, input);
			
			printf("---------------------\n");
			printf("Felhasználó módosítva\n");
			printf("---------------------\n");
			input[0] = 'x';
			input[1] = '\0';
			getchar();
		}
	}
}

void addUser(){
	system("clear");
	printf("Felhasználó hozzáadása\n");
	printf("---------------------\n");
	printf("Név: ");
	readline();
	strcpy(users[usercount].name, input);
	printf("E-mail: ");
	readline();
	strcpy(users[usercount].mail, input);
	printf("Eseményazonosító: ");
	readline();
	strcpy(users[usercount].eventname, input);
	
	//timestamp
	char text[100];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(text, sizeof(text)-1, "%Y-%m-%d %H:%M", t);
	strcpy(users[usercount].time, text);
	
	//check events
	int i = 0;
	int eventid = -1;
	while (i<eventcount && eventid<0){
		if (strcmp(events[i].name, users[usercount].eventname) == 0) eventid = i;
		i++;
	}
	
	if (eventid >= 0){
		usercount++;
		printf("---------------------\n");
		printf("%i. felhasználó hozzáadva\n", calcUserByEvent(eventid));
		printf("---------------------\n");
	} else {
		printf("---------------------\n");
		printf("Nincs ilyen esemény, a felhasználó nem lett hozzáadva\n");
		printf("---------------------\n");
	}
	getchar();
}

void delUsersByEventId(int id){
	int i = 0;
	while (i<usercount){
		if (strcmp(users[i].eventname, events[id].name) == 0){
			dU(i);
			i--;
		}
		i++;
	}
}

void dU(int x){
	strcpy(users[x].name, users[usercount-1].name);
	strcpy(users[x].mail, users[usercount-1].mail);
	strcpy(users[x].eventname, users[usercount-1].eventname);
	strcpy(users[x].time, users[usercount-1].time);
	usercount--;
}

void dE(int x){
	strcpy(events[x].name, events[eventcount-1].name);
	eventcount--;
}

void addEvent(){
	system("clear");
	printf("Esemény hozzáadása\n");
	printf("---------------------\n");
	printf("Azonosító: ");
	readline();
	strcpy(events[eventcount].name, input);
	printf("---------------------\n");
	printf("Esemény hozzáadva\n");
	printf("---------------------\n");
	eventcount++;
	getchar();
}

void loadUsers(){
	FILE * handle;
	handle = fopen("user.txt", "r");
	char buffer[BUFFERSIZE];
	usercount = 0;
	int step;
	int reader;
	int inner;
	while(fgets(buffer, sizeof(buffer), handle) != 0){
		reader = 0;
		step = 0;
		inner = 0;
		while(buffer[reader] != '\n' && buffer[reader] != '\r'){
			//printf("\nchar: '%c'", buffer[reader]);
			if (buffer[reader] == '|'){
				reader++;
				step++;
				inner = 0;
			}
			if (step == 0) users[usercount].name[inner] = buffer[reader];
			if (step == 1) users[usercount].mail[inner] = buffer[reader];
			if (step == 2) users[usercount].eventname[inner] = buffer[reader];
			if (step == 3) users[usercount].time[inner] = buffer[reader];
			
			inner++;
			reader++;
		}
		//printf("%s", buffer);
		usercount++;
	}
	fclose(handle);
}
void loadEvents(){
	FILE * handle;
	handle = fopen("event.txt", "r");
	char buffer[BUFFERSIZE];
	eventcount = 0;
	int reader;
	while(fgets(buffer, sizeof(buffer), handle) != 0){
		reader = 0;
		while(buffer[reader] != '\n' && buffer[reader] != '\r'){
			events[eventcount].name[reader] = buffer[reader];
			reader++;
		}
		eventcount++;
	}
	fclose(handle);
}
void listUsers(){
	system("clear");
	printf("Felhasználók listázása\n");
	printf("---------------------\n");
	int i = 0;
	while(i<usercount){
		printf("[%i] %s, %s, %s, %s\n", i+1, users[i].name, users[i].mail, users[i].eventname, users[i].time);
		i++;
	}
	printf("---------------------\n");
	getchar();
}
void listEvents(){
	system("clear");
	printf("Események listázása\n");
	printf("---------------------\n");
	int i = 0;
	//printf("listamerete:%i\n", eventcount);
	while(i<eventcount){
		printf("[%i] %s\n", i+1, events[i].name);
		i++;
	}
	printf("---------------------\n");
	getchar();
}

void saveUsers(){
	FILE *fp;
	fp = fopen("user.txt", "w+");
	int i = 0;
	while (i<usercount){
		fputs(users[i].name, fp);
		fputs("|", fp);
		fputs(users[i].mail, fp);
		fputs("|", fp);
		fputs(users[i].eventname, fp);
		fputs("|", fp);
		fputs(users[i].time, fp);
		fputs("\n", fp);
		i++;
	}
	fclose(fp);
}

void saveEvents(){
	FILE *fp;
	fp = fopen("event.txt", "w+");
	int i = 0;
	while (i<eventcount){
		fputs(events[i].name, fp);
		fputs("\n", fp);
		i++;
	}
	fclose(fp);
}

#endif