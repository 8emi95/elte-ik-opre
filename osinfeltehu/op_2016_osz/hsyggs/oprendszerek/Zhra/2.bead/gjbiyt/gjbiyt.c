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

const char* parentPipe = "parentToChild";
const char* childPipe = "childToParent";
visitor visitors[1024];

int closePipes();
void handler(int signumber);
int fetchVisitors(int c);

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
int StartEvent();

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
		case '8':
			StartEvent();
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
	printf("8 - Rendezvény lebonyolítása\n");
	
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

void handler(int signumber){
  printf("A partner megérkezett a helyszínre.\n");
}

int closePipes(){
	unlink(parentPipe);
	unlink(childPipe);
	
	return 0;
}

//-------------------------------------------------------------------------------

int StartEvent(){
	struct sigaction sigact;
	sigact.sa_handler = handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0; 
	sigaction(SIGUSR1,&sigact,NULL);
	
	srand(time(NULL));

	int pid, pTC, cTP;
	int c = 0;

	int childToParent = mkfifo(childPipe, S_IRUSR | S_IWUSR);
	int parentToChild = mkfifo(parentPipe, S_IRUSR | S_IWUSR);
	
	if (childToParent == -1) {
		printf("Error number(childToParent): %i", errno);
		exit(EXIT_FAILURE);
	}
	
	if (parentToChild == -1) {
		printf("Error number(parentToChild): %i", errno);
		exit(EXIT_FAILURE);
	}
	
	pid = fork();

	if (pid > 0) //parent 
	{
		char rcvdMsg[1024];
		char sendMsg[1024];
		
		while (c != eventCount){
			pTC = open(parentPipe, O_WRONLY);
			write(pTC, events[c].id, strlen(events[c].id) + 1);
			
			pause();
			
			fetchVisitors(c);
			
			printf("fetch után\n");
			
			write(pTC, visitors, sizeof(visitors));
			close(pTC);
			
			int random;
			cTP = open(childPipe, O_RDONLY);
			read(cTP, &random, sizeof(random));
			printf("parent: a rendezveny sikeressege: %d\n", random);
			
			int i;
			for (i = 0; i < events[c].visitorCount; ++i){
				read(cTP, &random, sizeof(random));
				if (random == 1){
					printf("parent: %s nem erkezett meg a rendezvenyre.\n", visitors[i]);
				}
			}
			printf(" \n\n");
			close(cTP);
			++c;
		}
	
		printf("Nincs több esemény!\n");
		closePipes();
	} 
	else // child
	{
		char rcvdMsg[1024];
		char sendMsg[1024];
		
		while (c != eventCount){
			pTC = open(parentPipe, O_RDONLY);
			read(pTC, rcvdMsg, sizeof(rcvdMsg));
			printf("Partner: Utazok a helyszínre (%s)\n", rcvdMsg);

			
			kill(getppid(),SIGUSR1);
			
			visitor childVisitors[1024];
			read(pTC, childVisitors, sizeof(childVisitors));
			
			int i;
			for (i = 0; i < events[c].visitorCount; ++i){
				printf("partner: %s\n", childVisitors[i].name);
			}
			close(pTC);
			
			int random = rand() % 10;
			cTP = open(childPipe, O_WRONLY);
			write(cTP, &random, sizeof(random));
			
			for (i = 0; i < events[c].visitorCount; ++i){
				random = rand() % 10 == 0;
				write(cTP, &random, sizeof(random));
			}
			close(cTP);
			++c;
		}
		
		exit(0);

	}
	return 0;
}

int fetchVisitors(int c){
	visitor v;
	char str[400];
	int i = 0;
	char date[400];
	
	FILE * f;
	f=fopen(fname,"r");
	
	while (!feof(f)){
		fscanf(f, "%[^:]: %[^,], %[^,], %[^\n]\n", v.name, v.mail, v.id, date);
		if (strcmp(v.id, events[c].id) == 0){
			strcpy(visitors[i].name, v.name);
			++i;
		}
	}
	fclose(f);
}















