#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

typedef struct{
	char name[100];
	char mail[100];
	char id[100];
} visitor;

typedef struct{
	char id[30];
	int visitorCount;
} event;

void Reg();
int isIn(char *id);
void Attendees();
void NewEvent();
void ListEvents();
int GetEventCount();
void RegistrationDelete();
void RegChange();
void EventDelete();

event events[256];
int eventCount = 0;
char *fname = "visitors.txt";


int main()
{
printf("Udvozlom a 'King of Stands' elektronikus latogatoi nyilvantartasaban! \n");

int option;




do
{
printf("1) Uj esemeny hozzaadasa. \n");
printf("2) Esemeny torlese. \n");
printf("3) Kilepes a rendszerbol. \n");
printf("4) Uj Regisztracio. \n");
printf("5) Resztvevok listajanak lekerese. \n");
printf("6) Regisztracio torlese. \n");
printf("7) Regisztracio modositasa. \n");
printf("8) Rendezveny torlese. \n");


scanf("%d", &option);

if(option == 1)
{
	NewEvent();

}
else if(option == 2)
{
	EventDelete();
	
}
else if(option == 4)
{
	Reg();
}
else if(option == 5)
{
	printf("A resztvevok:\n");
	Attendees();

}
else if(option == 6)
{
	printf("Regisztracio torlese. \n");
	RegistrationDelete();

}

else if(option == 7)
{
	printf("Regisztracio modositasa. Egyesevel lehetseges.\n");
	RegChange();

}
else if(option == 8)
{
	printf("Esemeny torlese.\n");
	EventDelete();

}


}
while (option != 3);

printf("Koszonjuk, hogy a mi szolgaltatasunkat valasztotta, viszontlatasra!\n");

 return 0;
}


void RegChange(){
	visitor v;
	char date[100];
	char name[100];
	char mail[100];
	char id[100];
	int i = 0;
	
	printf("Modositando latogato neve: ");
	fgets(name, sizeof(name), stdin);

	
	printf("Email: ");
	fgets(mail, sizeof(mail), stdin);

	
	printf("Id: ");
	fgets(id, sizeof(id), stdin);

	
	
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
}

void RegistrationDelete(){
	visitor v;
	char date[100];
	char name[100];
	int i = 0;
	
	printf("Torlendo latogato neve: ");
	scanf("%s",name);
	
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

void EventDelete(){
	visitor v;
	char date[100];
	char id[100];
	int i = 0;
	int tmpForId; 
	
	printf("Torlendo esemeny azonositoja: ");
	scanf("%s",id);

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
	
	if(tmpForId != -1)	{
		for (i = tmpForId; i < eventCount -1; i++){
			strcpy(events[i].id, events[i+1].id);
			events[i].visitorCount = events[i+1].visitorCount;
		}
		eventCount -= 1;
	}
}

void ListEvents(){
	int i;
	for (i = 0; i < eventCount; i++){
		printf("event %d: %s\n", i, events[i].id);
	}
}

void NewEvent(){
	char newId[100];
	printf("Uj rendezveny azonositoja:\n");
	scanf("%s",newId);
	
	if (isIn(newId) == -1){
		strcpy(events[eventCount].id, newId);
		events[eventCount].visitorCount = 0;
		eventCount++;
	}
	
}

void Attendees(){
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
}


void Reg(){
	visitor v;
	FILE * f;
	f=fopen(fname,"a");
	
	if (f==NULL){
		perror("File opening error\n"); exit(1);
	}
		
printf("Kerem adja meg a nevet! \n");
	scanf("%s", v.name);

		fprintf(f, "%s:", v.name);
		    fprintf(f, " ");
	
printf("Kerem adja meg az email cimet! \n");
	scanf("%s",v.mail);

	    fprintf(f, "%s,", v.mail);
			fprintf(f, " ");
			
printf("Kerem adja meg az esemeny azonositojat! \n");
	scanf("%s", v.id);

		fprintf(f, "%s,", v.id);
			fprintf(f, " ");
			
	time_t mytime;
    time(&mytime);
    fprintf(f, "%s", ctime(&mytime));			

	int visitorCountOfThisEvent = events[isIn(v.id)].visitorCount;
	
	if (isIn(v.id) == -1){
		strcpy(events[eventCount].id,v.id);
		events[eventCount].visitorCount = 1;
		visitorCountOfThisEvent = 1;
		
		eventCount++;
	}
	else{
		events[isIn(v.id)].visitorCount++;
	}
	
    
    printf("Sorszamod: %d\n", visitorCountOfThisEvent);
    fclose(f);
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
