#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <fcntl.h> //open,creat
#include <string.h>
#include <time.h>
#include <unistd.h> // for pipe()
#include <signal.h>

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
void handler(int signumber){
  //printf("A %i. szamu signal megerkezett\n",signumber);
}

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

void StartEvents(int eventStructSize, int visitorStructSize){
	srand ( time(NULL) );
	int pipepw[2];
	int pipecw[2];
	pid_t pid;
	signal(SIGUSR1,handler);
	
	if (pipe(pipepw) == -1){
		perror("Hiba a pipe megnyitasakor!");
		exit(1);
	}
	if (pipe(pipecw) == -1){
		perror("Hiba a pipe megnyitasakor!");
		exit(1);
	}
	
	pid = fork();
	if (pid == -1) {
		perror("Fork hiba");
		exit(1);
	}
	if (pid > 0){	// Szulo
		close(pipepw[0]);
		close(pipecw[1]);
		printf("King of Stands nekilat a rendezvenyek lebonyolitasanak.\n");
		////////////////////////////////////// Osszes rendezveny es vendeg beolvasasa
		int eventFile=open(EVENTSFILE,O_RDWR|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
		if(eventFile<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
		int fileBytes = HowManyBytesInFie(EVENTSFILE);
		if(fileBytes==0) return;
		
		int eSize = fileBytes/eventStructSize;
		struct Event events[eSize];
		int i;
		for(i=0; i<eSize; i++){
			read(eventFile,&events[i],sizeof(events[i]));
		}
		
		int visitorFile=open(VISITORSFILE,O_RDWR|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR); 
		if(visitorFile<0) { perror("Hiba a file megnyitasakor!\n");exit(1);}
		fileBytes = HowManyBytesInFie(VISITORSFILE);
		if(fileBytes==0) return;
		
		int vSize = fileBytes/visitorStructSize;
		struct Visitor visitors[vSize];
		for(i=0; i<vSize; i++){
			read(visitorFile,&visitors[i],sizeof(visitors[i]));
		}
		
		close(eventFile);
		close(visitorFile);
		////////////////////////////////////// Beolvasasok vege
		write(pipepw[1], &eSize, 3);
		pause();
		for(i=0; i<eSize; i++){
			// Esemenyek kuldese
			printf("\nKing: Kedves partner! Utazz a %d. szamu rendezvenyre, melynek neve %s.\n", events[i].id, events[i].name);
			write(pipepw[1], &events[i], eventStructSize);
			pause();
			// Vendegek szama
			int j;
			int numOfVisitors = 0;
			for(j=0; j<vSize; j++){
				if(visitors[j].eventId==events[i].id){
					numOfVisitors++;
				}
			}
			//printf("King: Vendegek szama: %d\n", numOfVisitors);
			write(pipepw[1], &numOfVisitors, sizeof(numOfVisitors));
			//pause();
			for(j=0; j<vSize; j++){
				// Vendegek kuldese
				int succesLevel = 0;
				if(visitors[j].eventId==events[i].id){
					write(pipepw[1], &visitors[j], visitorStructSize);
					//pause();
					int isArrived = 0;
					read(pipecw[0],&isArrived,sizeof(isArrived));
					//kill(pid,SIGUSR1);
					if(isArrived==1){
						printf("King: Ugy latom, hogy %s nem erkezett meg.\n", visitors[j].name);
					}
					//printf("King: Ugy latom, hogy %s megerkezett.\n", visitors[j].name);
					read(pipecw[0],&succesLevel,sizeof(succesLevel));
					//kill(pid,SIGUSR1);
					numOfVisitors--;
					if(numOfVisitors==0){
						printf("King: A partnerunk szerint %d%-ban volt sikeres a rendezveny.\n", succesLevel);
					}
				}
			}
		}
		close(pipepw[1]);
		close(pipecw[0]);
	} 
	else {	// Gyerek
		close(pipepw[1]);
		close(pipecw[0]);
		int numOfEvents = 0;
		read(pipepw[0],&numOfEvents,3);		// Addig var amig nincs mit olvasni
		kill(getppid(),SIGUSR1);
		int k;
		for(k=0; k<numOfEvents; k++){
			// Rendezveny megkapasa
			struct Event event;
			//printf("Partner: Olvasom a rendezveny nevet.\n");
			read(pipepw[0],&event,eventStructSize);
			kill(getppid(),SIGUSR1);
			printf("Partner: A %d. szamu rendezvenyre ertem, johetnek a vendegek!\n", event.id);
			// Vendegszam megkapasa
			int numOfVisitors = 0;
			read(pipepw[0],&numOfVisitors,sizeof(numOfEvents));
			//kill(getppid(),SIGUSR1);
			//printf("Partner: Vendegek szama: %d\n", numOfVisitors);
			int l;
			for(l=0; l<numOfVisitors; l++){
				struct Visitor visitor;
				read(pipepw[0],&visitor,visitorStructSize);
				//kill(getppid(),SIGUSR1);
				int randnum = rand()%10;
				//printf("Partner: %d rand.\n", randnum);
				if(randnum==1){
					printf("Partner: %s nevu vendeg nem erkezett meg.\n", visitor.name);
				} else {
					printf("Partner: %s nevu vendeg megerkezett.\n", visitor.name);
				}
				write(pipecw[1],&randnum,sizeof(randnum));
				//pause();
				randnum = rand()%100;
				//printf("Partner: %d succes.\n", randnum);
				write(pipecw[1],&randnum,sizeof(randnum));
				//pause();
			}
		}
		close(pipepw[0]);
		close(pipecw[1]);
	}
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
		printf("3. Rendezvenyek inditasa\n");
		printf("4. Kilepes\n");

		while(!(menu>=1 && menu<=4)){
			menu = ChooseOption();
		}
		switch(menu){
			case 1:
				menu = 0;
				printf("\n1. Uj rendezveny\n");
				printf("2. Rendezvenyek listazasa\n");
				printf("3. Rendezveny adatainak torlese\n");
				printf("4. Vissza a fomenube\n");
				while(!(menu>=1 && menu<=4)){
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
				while(!(menu>=1 && menu<=5)){
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
				StartEvents(eventBytes, visitorBytes);
				break;
			case 4:
			default:
				return 0;
				break;
		}
	}
	
	return 0;
}