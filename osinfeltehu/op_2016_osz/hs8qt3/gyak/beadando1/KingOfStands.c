#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <fcntl.h> //open,creat
#include <string.h>
#include <time.h>

#define EVENTSFILE "events"
#define VISITORSFILE "visitors"

// vendég módosítás, esemény törlésnél vendég törlés is

// Strukturak
struct Event{
	unsigned int id;
	char name[30];
};

struct Visitor{
	unsigned int id;
	unsigned int eventId;
	char name[30];
	char email[30];
	time_t time;
};

// Altalanos fuggvenyek
int HowManyBytesInFie(char* fileName){	// Hany byte van a fileban
	struct stat s;
	stat(fileName, &s);
	int fileBytes = s.st_size;
	
	return fileBytes;
}

int GetIndex(char* fileName, int structSize){	// Megkeressuk a fajlban a legnagyobb indexet
	int fileBytes = HowManyBytesInFie(fileName);
	if(fileBytes==0) return 0;
	
	int file=open(fileName,O_RDONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	if(fileName==EVENTSFILE){
		int size = fileBytes/structSize;
		struct Event events[size];
		int i;
		for(i=0; i<size; i++){
			read(file,&events[i],sizeof(events[i]));
		}
		close(file);
		// Maxker
		int max = events[0].id;
		for(i=0; i<size; i++){
			if(max<events[i].id){
				max = events[i].id;
			}
		}
		return max;
	} else if(fileName==VISITORSFILE){
		int size = fileBytes/structSize;
		struct Visitor visitors[size];
		int i;
		for(i=0; i<size; i++){
			read(file,&visitors[i],sizeof(visitors[i]));
		}
		close(file);
		// Maxker
		int max = visitors[0].id;
		for(i=0; i<size; i++){
			if(max<visitors[i].id){
				max = visitors[i].id;
			}
		}
		return max;
	}
}

// Menu fuggvenyek
int ChooseOption(){
	int option=-1;
	printf("Valasszon a lehetosegek kozul:\n");
	scanf("%d", &option);
	return option;
}

// Rendezveny fuggvenyek
void NewEvent(struct Event* event){
	int file=open(EVENTSFILE,O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	printf("%d",(*event).id);
	write(file, &(*event), sizeof(*event));
	
	close(file);
}

void ListEvents(){
	int file=open(EVENTSFILE,O_RDONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}

	printf("\nRendezvenyek:\n");
	struct Event e;
	while(read(file,&e,sizeof(e))){	// Egy rendezvennyi beolvasasa
		printf("Rendezveny nev:\t%s\n\tAzonosito: %d\n",e.name,e.id);
	}
	
	close(file);
}

void DeleteEvent(int id, int structSize){
	int file=open(EVENTSFILE,O_RDWR|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	int fileBytes = HowManyBytesInFie(EVENTSFILE);
	if(fileBytes==0) return;
	
	// Beolvasas
	int size = fileBytes/structSize;
	struct Event events[size];
	struct Event temp[size-1];
	int i;
	int j=0;
	for(i=0; i<size; i++){	// Egy iteracioval megcsinalunk mindent
		read(file,&events[i],sizeof(events[i]));
		// Kereses es "torles"
		if(id!=events[i].id && j<size-1){
			temp[j]=events[i];
			j++;
		}
	}
	close(file);
	
	// Fajlba iras
	file=open(EVENTSFILE,O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	for(i=0; i<size-1; i++){
		write(file, &temp[i], sizeof(temp[i]));	
	}
	
	close(file);
}

// Vendeg fuggvenyek
void NewVisitor(struct Visitor* visitor){
	int file=open(VISITORSFILE,O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	printf("%d",(*visitor).id);
	write(file, &(*visitor), sizeof(*visitor));
	
	close(file);
}

void ListVisitors(){
	int file=open(VISITORSFILE,O_RDONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}

	printf("\nVendegek:\n");
	struct Visitor v;
	while(read(file,&v,sizeof(v))){	// Egy rendezvennyi beolvasasa
		printf("Vendeg nev:\t%s\n\tEmail cím: %s\n\tRegisztracio ideje: %s\tRendezveny: %d\n\tAzonosito: %d\n",v.name,v.email,ctime(&v.time),v.eventId,v.id);
	}
	
	close(file);
}

void DeleteVisitor(int id, int structSize){
	int file=open(VISITORSFILE,O_RDWR|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	int fileBytes = HowManyBytesInFie(VISITORSFILE);
	if(fileBytes==0) return;
	
	// Beolvasas
	int size = fileBytes/structSize;
	struct Visitor visitors[size];
	struct Visitor temp[size-1];
	int i;
	int j=0;
	for(i=0; i<size; i++){	// Egy iteracioval megcsinalunk mindent
		read(file,&visitors[i],sizeof(visitors[i]));
		// Kereses es "torles"
		if(id!=visitors[i].id && j<size-1){
			temp[j]=visitors[i];
			j++;
		}
	}
	close(file);
	
	// Fajlba iras
	file=open(VISITORSFILE,O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	for(i=0; i<size-1; i++){
		write(file, &temp[i], sizeof(temp[i]));	
	}
	
	close(file);
}

void ModifyVisitor(int id, int structSize){
	int file=open(VISITORSFILE,O_RDWR|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	int fileBytes = HowManyBytesInFie(VISITORSFILE);
	if(fileBytes==0) return;
	
	// Beolvasas
	int size = fileBytes/structSize;
	struct Visitor visitors[size];
	int searchedIndex = -1;
	int i;
	for(i=0; i<size; i++){	// Egy iteracioval megcsinalunk mindent
		read(file,&visitors[i],sizeof(visitors[i]));
		if(id==visitors[i].id){
			searchedIndex = i;
		}
	}
	close(file);
	if(searchedIndex==-1) return;	// Ha nem talaltuk meg a keresett indexet
	
	// Modositas
	printf("Jelenlegi adatok:\n");
	printf("Vendeg nev:\t%s\n\tEmail cím: %s\n\tRendezveny: %d\n",visitors[searchedIndex].name,visitors[searchedIndex].email,visitors[searchedIndex].eventId);
	printf("Adja meg a vendeg nevet (szokoz nelkul):\n");
	scanf("%s", &visitors[searchedIndex].name);
	printf("Adja meg a vendeg email cimet:\n");
	scanf("%s", &visitors[searchedIndex].email);
	printf("Adja meg a rendezveny azonositojat, amelyikre a vendeg jelentkezik:\n");
	scanf("%d", &visitors[searchedIndex].eventId);
	
	// Fajlba iras
	file=open(VISITORSFILE,O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR); 
	if(file<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
	
	for(i=0; i<size; i++){
		write(file, &visitors[i], sizeof(visitors[i]));	
	}
	
	close(file);
}

// Main
int main(){
	// Hany byteosak a strukturak a jelenlegi gepen
	int eventBytes = sizeof(struct Event);
	int visitorBytes = sizeof(struct Visitor);
	
	
	// Indexek az adatbazishoz
	int eventIndex=0;
	int visitorIndex=0;
	eventIndex = GetIndex(EVENTSFILE, eventBytes);
	//printf("%d\n", eventIndex);
	visitorIndex = GetIndex(VISITORSFILE, visitorBytes);
	//printf("%d\n", visitorIndex);
	
	struct Event events[30];
	struct Visitor visitors[30];
	
	int menu=0;
	while(menu!=3){
		menu=0;
		printf("\n1. Rendezvenyek\n");
		printf("2. Vendegek\n");
		printf("3. Kilepes\n");

		while(menu!=1 && menu!=2 && menu!=3){
			menu = ChooseOption();
		}
		switch(menu){
			case 1:
				menu = 0;
				printf("\n1. Uj rendezveny\n");
				printf("2. Rendezvenyek listazasa\n");
				printf("3. Rendezveny adatainak torlese\n");
				printf("4. Vissza a fomenube\n");
				while(menu!=1 && menu!=2 && menu!=3 && menu!=4){
					menu = ChooseOption();
				}
				switch(menu){
					case 1:
						// Letrehozok egy ideiglenes esemenyt, csak hogy fileba menthessem
						eventIndex++;
						struct Event temp;
						printf("Adja meg a rendezveny nevet (szokoz nelkul):\n");
						scanf("%s", &temp.name);
						temp.id = eventIndex;
						NewEvent(&temp);
						break;
					case 2:
						ListEvents();
						break;
					case 3:
						printf("Adja meg a torlendo rendezveny azonositojat:\n");
						int id;		// Label utan statement kell, azert ez a sorrend
						scanf("%d", &id);
						DeleteEvent(id, eventBytes);
						menu = 0;
						break;
					case 4:
					default:
						break;
				}
				break;
			case 2:
				menu = 0;
				printf("\n1. Uj vendeg\n");
				printf("2. Vendegek listazasa\n");
				printf("3. Vendeg adatainak torlese\n");
				printf("4. Vendeg adatainak modositasa\n");
				printf("5. Vissza a fomenube\n");
				while(menu!=1 && menu!=2 && menu!=3 && menu!=4 && menu!=5){
					menu = ChooseOption();
				}
				switch(menu){
					case 1:
						visitorIndex++;
						struct Visitor temp;
						printf("Adja meg a vendeg nevet (szokoz nelkul):\n");
						scanf("%s", &temp.name);
						printf("Adja meg a vendeg email cimet:\n");
						scanf("%s", &temp.email);
						printf("Adja meg a rendezveny azonositojat, amelyikre a vendeg jelentkezik:\n");
						scanf("%d", &temp.eventId);
						temp.id = visitorIndex;
						time(&temp.time);
						NewVisitor(&temp);
						break;
					case 2:
						ListVisitors();
						break;
					case 3:
						printf("Adja meg a torlendo vendeg azonositojat:\n");
						int id;		// Label utan statement kell, azert ez a sorrend
						scanf("%d", &id);
						DeleteVisitor(id, visitorBytes);
						menu = 0;
						break;
					case 4:
						printf("Adja meg a modositando vendeg azonositojat:\n");
						int id2;		// Label utan statement kell, azert ez a sorrend
						scanf("%d", &id2);
						ModifyVisitor(id2, visitorBytes);
						break;
					case 5:
					default:
						break;
				}
				break;
			case 3:
			default:
				return 0;
				break;
		}
	}
	
	return 0;
}