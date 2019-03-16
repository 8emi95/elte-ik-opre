#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

typedef struct{
	char name[100];
	char mail[100];
	char id[100];
} visitor;

typedef struct{
	char id[30];
	int visitorCount;
} event;

char getChar();
int LogIn();
int Write();
int isIn(char *id);
int ListVisitors();
int AddEvent();
int ListEvents();
int GetEventCount();
int DelVisitor();
int ModVisitor();
int DelEvent();
int 

event events[256];
int eventCount = 0;
char *fname = "visitors.txt";
	
int main(int argc,char ** argv) {
	
	GetEventCount();
	char c = 0;
	size_t buffsize = 30;
	char* buffer;
	do {
		Write();
		printf("Parancs sorszam:  ");
		c = getChar();
		system("clear");
		switch (c)
		{
		case '1':
			LogIn();
			break;
		case '2':
			ListVisitors();
			break;
		case '3':
			ModVisitor();
			break;
		case '4':
			DelVisitor();
			break;
		case '5':
			AddEvent();
			break;
		case '6':
			DelEvent();
			break;
		case '7':
			ListEvents();
			break;
		}
	} while (c != '0');
	return 0;
}

int ModVisitor(){
	visitor v;
	char date[100];
	char name[100];
	char mail[100];
	char id[100];
	int i = 0;
	
	printf("Modositando latogato neve: ");
	fgets(name, sizeof(name), stdin);
	name[strlen(name)-1] = '\0';
	
	printf("Email: ");
	fgets(mail, sizeof(mail), stdin);
	mail[strlen(mail)-1] = '\0';
	
	printf("Id: ");
	fgets(id, sizeof(id), stdin);
	id[strlen(id)-1] = '\0';
	
	
	FILE * f;
	f=fopen(fname,"r");
	
	FILE * tmp;
	tmp = fopen("tmp.exe", "w");
	
	while (!feof(f)){
		fscanf(f, "%[^:]: %[^,], %[^,], %[^\n]\n", v.name, v.mail, v.id, date);
		if (strcmp(v.name, name) != 0){
			fprintf(tmp, "%s:", v.name);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.mail);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.id);
			fprintf(tmp, " ");
			fprintf(tmp, "%s", date);
			fprintf(tmp, "\n");
		}
		else{
			fprintf(tmp, "%s:", name);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", mail);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", id);
			fprintf(tmp, " ");
			fprintf(tmp, "%s", date);
			fprintf(tmp, "\n");
		}
	}
	fclose(f);
	fclose(tmp);
	
	remove(fname);
	rename("tmp.exe", fname);
	
	return 0;
}

int DelVisitor(){
	visitor v;
	char date[100];
	char name[100];
	int i = 0;
	
	printf("Torlendo latogato neve: ");
	fgets(name, sizeof(name), stdin);
	name[strlen(name)-1] = '\0';
	
	FILE * f;
	f=fopen(fname,"r");
	
	FILE * tmp;
	tmp = fopen("tmp.exe", "w");
	
	while (!feof(f)){
		fscanf(f, "%[^:]: %[^,], %[^,], %[^\n]\n", v.name, v.mail, v.id, date);
		if (strcmp(v.name, name) != 0){
			fprintf(tmp, "%s:", v.name);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.mail);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.id);
			fprintf(tmp, " ");
			fprintf(tmp, "%s", date);
			fprintf(tmp, "\n");
		}
	}
	fclose(f);
	fclose(tmp);
	
	remove(fname);
	rename("tmp.exe", fname);
	
	return 0;
}

int GetEventCount(){
	visitor v;
	char date[100];
	
	FILE * f;
	f=fopen(fname,"a");
	fclose(f);

	f=fopen(fname,"r");
	
	if (f==NULL){
		perror("File opening error\n"); exit(1);
	}
	
	while (!feof(f)){
		fscanf(f, "%[^:]: %[^,], %[^,], %[^\n]\n", v.name, v.mail, v.id, date);
		if(isIn(v.id) == -1){
			strcpy(events[eventCount].id, v.id);
			events[eventCount].visitorCount = 0;
			eventCount++;
		}
		else{
			events[isIn(v.id)].visitorCount += 1;
		}
	}
	fclose(f);
	
}

int DelEvent(){
	visitor v;
	char date[100];
	char id[100];
	int i = 0;
	int tmpForId; 
	
	printf("Torlendo esemeny azonositoja: ");
	fgets(id, sizeof(id), stdin);
	id[strlen(id)-1] = '\0';
	tmpForId = isIn(id);
	
	FILE * f;
	f=fopen(fname,"r");
	
	FILE * tmp;
	tmp = fopen("tmp.exe", "w");
	
	while (!feof(f)){
		fscanf(f, "%[^:]: %[^,], %[^,], %[^\n]\n", v.name, v.mail, v.id, date);
		if (strcmp(v.id, id) != 0){
			fprintf(tmp, "%s:", v.name);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.mail);
			fprintf(tmp, " ");
			fprintf(tmp, "%s,", v.id);
			fprintf(tmp, " ");
			fprintf(tmp, "%s", date);
			fprintf(tmp, "\n");
		}
	}
	fclose(f);
	fclose(tmp);
	
	remove(fname);
	rename("tmp.exe", fname);
	
	printf("%d\n", tmpForId);
	if(tmpForId != -1)	{
		for (i = tmpForId; i < eventCount -1; i++){
			strcpy(events[i].id, events[i+1].id);
			events[i].visitorCount = events[i+1].visitorCount;
		}
		eventCount -= 1;
	}
    return 0;
}

int ListEvents(){
	int i;
	for (i = 0; i < eventCount; i++){
		printf("event %d: %s\n", i, events[i].id);
	}
	return 0;
}

int AddEvent(){
	char newId[100];
	printf("Uj rendezveny azonositoja: ");
	fgets(newId, sizeof(newId), stdin);
	
	if (isIn(newId) == -1){
		strcpy(events[eventCount].id, newId);
		events[eventCount].visitorCount = 0;
		eventCount++;
	}
	
	return 0;
	
}

int ListVisitors(){
	char str[400];
	int i = 0;
	FILE * f;
	f=fopen(fname,"r");
	
	if (f==NULL){
		perror("File opening error\n"); exit(1);
	}
	
	while (fgets(str, sizeof(str), f)!= NULL){
		printf("%s",str);
	}
    fclose(f);
    return 0;
}

int Write(){
	printf("0 - Kilepes\n");
	printf("1 - Bejelentkezes vendegek szamara\n");
	printf("2 - Listazas\n");
	printf("3 - Vendeg adatainak modositasa\n");
	printf("4 - Vendeg adatainak torlese\n");
	printf("5 - Uj rendezveny inditasa\n");
	printf("6 - Rendezveny torlese\n");
	printf("7 - Rendezvenyek listazasa\n");
	printf("8 - Rendezvény lebonyolítása\n")
	
	return 0;
}

int LogIn(){
	visitor v;
	
	printf("Nev: ");
	fgets(v.name, sizeof(v.name), stdin);
	v.name[strlen(v.name)-1] = '\0';
	
	printf("E-mail: ");
	fgets(v.mail, sizeof(v.mail), stdin);
	v.mail[strlen(v.mail)-1] = '\0';
	
	printf("Rendezvenyazonosito: ");
	fgets(v.id, sizeof(v.id), stdin);
	v.id[strlen(v.id)-1] = '\0';
	
	FILE * f;
	f=fopen(fname,"a");
	
	
	if (f==NULL){
		perror("File opening error\n"); exit(1);
	}
	
	int visitorCountOfThisEvent = events[isIn(v.id)].visitorCount;
	
	if (isIn(v.id) == -1){
		strcpy(events[eventCount].id, v.id);
		events[eventCount].visitorCount = 1;
		visitorCountOfThisEvent = 1;
		
		eventCount++;
	}
	else{
		events[isIn(v.id)].visitorCount++;
	}
	
	fprintf(f, "%s:", v.name);
	
    fprintf(f, " ");
    fprintf(f, "%s,", v.mail);
	fprintf(f, " ");
	fprintf(f, "%s,", v.id);
	fprintf(f, " ");

	time_t mytime;
    time(&mytime);
    fprintf(f, "%s", ctime(&mytime));
    
    printf("Sorszamod: %d\n", visitorCountOfThisEvent);
    fclose(f);
	return 0;
}	

char getChar(){
	char ch;
	ch = getchar();
	
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
	
	return ch;
}

int isIn(char *id){
	int i;
	for (i = 0; i < eventCount; i++){
		if(strcmp(id, events[i].id) == 0){
			return i;
		}
	}
	
	return -1;
}
