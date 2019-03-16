#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int lastIndex = 0;
int lastIndex2 = 0;

struct visitor{
	char name[50], email[50], timestamp[50];
	int eventId;
}*visitors[100];

struct event{
	char name[100];
	int id;
}*events[100];

void loadData(){
	
	//VISITORS
	char line[100];
	FILE *fp = fopen("visitors.txt","r");
	int i = 0;

	while(fgets(line, 100, fp)) {
		
		visitors[i] = (struct visitor *) malloc(sizeof(struct visitor));
		
		char * pch= strtok (line," ");
		int j = 0; 
		
		while (pch != NULL && pch !="\n"){
			if (j == 0){
				strcpy(visitors[i]->name, pch);
			}
			else if (j == 1){
				strcpy(visitors[i]->email, pch);
			}
			else if (j == 2){
				visitors[i]->eventId = atoi(pch);
			}
			else if (j == 3){
				strcpy(visitors[i]->timestamp,pch);
			}
			else if (j>3 && j<6){
				strcat(visitors[i]->timestamp," ");
				strcat(visitors[i]->timestamp,pch);
			}
			j++;
			pch = strtok (NULL, " ");
			
		}i++; lastIndex++;
	}
	
	//EVENTS
	char line2[100];
	FILE *fp2 = fopen("events.txt","r");
	int i2 = 0;

	while(fgets(line2, 100, fp2)) {
		
		events[i2] = (struct event *) malloc(sizeof(struct event));
		
		char * pch2= strtok (line2," ");
		int j2 = 0; 
		
		while (pch2 != NULL){
			
			switch(j2){
				case 0:
					strcpy(events[i2]->name, pch2);
					break;
				case 1:
					events[i2]->id = atoi(pch2);
					break;
				default:
					break;
			}j2++;
			pch2 = strtok (NULL, " ");
			
		}i2++; lastIndex2++;
	}
	
}

void saveData(){
	
	//VISITORS
	FILE *f1 = fopen("visitors.txt", "w");
	if (f1 == NULL){
		printf("Hiba a latogatok fajljanak betoltese kozben!\n");
		exit(1);
	}

	int i;
	for (i = 0; i < lastIndex; i++){
		fprintf(f1, "%s %s %d %s", visitors[i]->name, visitors[i]->email, visitors[i]->eventId, visitors[i]->timestamp);
	}
	fclose(f1);
	printf("A latogatok adatainak mentese sikeres!\n");
	
	//EVENTS
	FILE *f2 = fopen("events.txt", "w");
	if (f2 == NULL){
		printf("Hiba a rendezvenyek fajljanak betoltese kozben!\n");
		exit(1);
	}

	int j;
	for (j = 0; j < lastIndex2; j++){
		fprintf(f2, "%s %d\n", events[j]->name,events[j]->id);
	}
	fclose(f2);
	printf("A rendezvenyek adatainak mentese sikeres!\n");
}

void collectData(){
	if (lastIndex2 !=0){
		
		char name_[50];
		printf("Neved: ");
		scanf("%s",name_);
		
		char email_[50];
		printf("Email-cimed: ");
		scanf("%s",email_);
		
		int eventId_;
		printf("Rendezveny azonositoja: ");
		scanf("%d",&eventId_);
		
		int position = 0;
		while (position != lastIndex2 && eventId_ != events[position]->id){
			position++;
		}
		
		if (position > lastIndex2-1){
			printf("A regisztracio nem lehetseges; nem talalhato ilyen rendezveny\n");
		}
		else{
			visitors[lastIndex] = (struct visitor *) malloc(sizeof(struct visitor));
			
			strcpy(visitors[lastIndex]->name,name_);
			strcpy(visitors[lastIndex]->email,email_);
			visitors[lastIndex]->eventId = eventId_;
			
			time_t rawtime;
		    struct tm *info;
		    char buffer[80];

		    time( &rawtime );

		    info = localtime( &rawtime );

		    strftime(buffer,80,"%x - %I:%M%p\n", info);
			
			strcpy(visitors[lastIndex]->timestamp,buffer);
			
			
			printf("\nAdataid:\nNev: %s\nEmail-cim: %s\nRendezveny azonosito: %d\nRegisztracio idopontja: %s \nA regisztracio megtortent %d. helyen.\n",visitors[lastIndex]->name,visitors[lastIndex]->email, visitors[lastIndex]->eventId, visitors[lastIndex]->timestamp, lastIndex+1);
			lastIndex++;
		}
	}
	else{
		printf("Meg nincs rendezveny, amin reszt lehet venni.");
	}
}

void listVisitors(){
	printf("\n");
	int i;
	for (i = 0; i < lastIndex; i++){
		printf("%s %s %d %s",visitors[i]->name, visitors[i]->email, visitors[i]->eventId, visitors[i]->timestamp);
	}
	printf("\n");
}

void modifyVisitorData(){
	char nameToModify[50];
	int position = 0;
	if (lastIndex != 0){
		printf("Add meg a modositani kivant latogato nevet\n");
		scanf("%s",&nameToModify);
		while ((position != lastIndex-1) && (strcmp(visitors[position]->name,nameToModify))){
			position++;
		}

		if (position >= lastIndex){
			printf("A modositas nem lehetseges; nem talalhato ilyen nevu latogato\n");
		}
		else{
			printf("Uj nev: \n");
			scanf("%s",&visitors[position]->name);
			
			printf("Uj email: \n");
			scanf("%s",&visitors[position]->email);
			
			printf("Uj esemenyazonosito: \n");
			scanf("%d",&visitors[position]->eventId);
			
			printf("A modositas sikeres.\n");
		}
	}
	else{
		printf("Meg nincs regisztralt vendeg.\n");
	}
}

void deleteVisitor(){
	if (lastIndex !=0){
		char nameToDelete[50];
		int position = 0;
		printf("Add meg a torolni kivant latogato nevet\n");
		scanf("%s",&nameToDelete);
		while ((position != lastIndex-1) && (strcmp(visitors[position]->name,nameToDelete))){
			position++;
		}
		
		if (position > lastIndex-1){
			printf("A torles nem lehetseges; nem talalhato ilyen nevu latogato\n");
		}
		else{
			int i;
			for (i = position; i < lastIndex - 1 ; i++){
				visitors[i] = visitors[i+1];
			}
			lastIndex--;
			printf("A torles sikeres.\n");
		}
	}
	else{
		printf("Meg nincs regisztralt vendeg.\n");
	}
}

void startNewEvent(){
	events[lastIndex2] = (struct event *) malloc(sizeof(struct event));
	
    printf("Esemeny neve: ");
	scanf("%s",events[lastIndex2]->name);

	printf("Rendezveny azonositoja: ");
	scanf("%d",&events[lastIndex2]->id);
	
	lastIndex2++;
	
	printf("A rendezveny sikeresen rogzitve lett\n");
}

void listEvents(){
	int i;
	for (i = 0; i < lastIndex2; i++){
		printf("%s %d\n",events[i]->name,events[i]->id);
	}
}

void deleteEvent(){
	if (lastIndex2 !=0){
		int idToDelete;
		int position = 0;
		printf("Add meg a torolni kivant esemeny azonositojat\n");
		scanf("%d",&idToDelete);
		
		while (position != lastIndex2 && idToDelete != events[position]->id){
			position++;
		}
		
		if (position > lastIndex2 - 1){
			printf("A torles nem lehetseges.\n");
		}
		else{
			int x;
			int c = 0;
			for (x = 0; x < lastIndex; x++){
				if (visitors[x]->eventId == idToDelete){
					c++;
				}
			}
			int y,k;
			for (y = 0; y < c; y++){
					int vPos = 0;
					while ((vPos != lastIndex-1) && (visitors[vPos]->eventId != idToDelete)){
						vPos++;
					}
					int i;
					for (i = vPos; i < lastIndex - 1 ; i++){
						visitors[i] = visitors[i+1];
					}
				lastIndex--;
			}
			
			int i;
			for (i = position; i < lastIndex2 - 1 ; i++){
				events[i] = events[i+1];
			}
			lastIndex2--;
			
			printf("A torles sikeres.\n");
		}
	}
	else{
		printf("Meg nincs regisztralt rendezveny.\n");
	}
}

void handler(){}

void startEvent(){
	signal(SIGTERM,handler);
	
	int pipefd1[2];
	int pipefd2[2];
	
	pid_t pid;
	char sz[300];
	char sz2[300];

	if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
		perror("Hiba pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid == -1) {
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	//gyerek
	if (pid == 0) {
		close(pipefd1[1]);
		close(pipefd2[0]);
		
		read(pipefd1[0], sz, sizeof(sz));
		
		//Jelzés
		kill(getppid(),SIGTERM); 
		
		read(pipefd1[0], sz2, sizeof(sz2)-1);
		printf("Nekik kellene itt lenni: \n%s\n",sz2);
		close(pipefd1[0]);
		
		//Sikeresség, kik nincsenek itt - > adatok elküldése
		char adatok[200];
		char nemJelentekMeg[200];
		
		srand(time(NULL));
		int rndm = rand() % 100;
		
		char randomNumber[5];
		sprintf(randomNumber, "%d", rndm);
		
		strcpy(adatok,  "Rendezveny sikeressege: ");
		strcat(adatok,randomNumber);
		strcat(adatok,"%\n");
		
		//Nem jelentek meg
		strcpy(nemJelentekMeg, "Nem jelentek meg: \n");
		
		char *string,*splinter;

		string = strdup(sz2);
		
		srand(time(NULL));
		int r = rand() % 100;
		while((splinter = strsep(&string,",")) != NULL ){
			r = (r + 11) % 100;
			if (r <= 10){
				strcat(nemJelentekMeg, splinter);
				strcat(nemJelentekMeg, ",");
			}
		}
		//
		
		strcat(adatok, nemJelentekMeg);
		write(pipefd2[1], adatok,sizeof(adatok)-1);
		fflush(NULL);
		close(pipefd2[1]);
		
		exit(EXIT_SUCCESS);
	}

	//szülő
	else {
		close(pipefd1[0]);
		close(pipefd2[1]);
		
		int eventId_;
		printf("Rendezveny azonositoja: ");
		scanf("%d",&eventId_);
		char str[10];
		sprintf(str, "%d", eventId_);
		
		write(pipefd1[1],str, 10);
		fflush(NULL);
		printf("A rendezveny elindult.\n");
		
		//Signalra várunk
		pause();
		
		char visitorList[200] = "";
		int position = 0;
		
		//Eseményazonosító elküldése
		while (position != lastIndex2 && eventId_ != events[position]->id){
			position++;
		}
		
		if (position > lastIndex2-1){
			printf("Nem található ilyen rendezvény.\n");
			kill(pid, SIGKILL);
		}
		else{
			int h;
			for (h = 0; h < lastIndex; h++){
				if (visitors[h]->eventId == eventId_){
					strcat(visitorList,visitors[h]->name);
					strcat(visitorList,",");
				}
			}
			write(pipefd1[1], visitorList,200);
			fflush(NULL);
			close(pipefd1[1]);
			
			//Gyerektől érkező statisztikák kiíratása
			int i;
			while ((i = read(pipefd2[0], sz, sizeof(sz))) > 0) {
				printf("%s\n",sz);
			}
			close(pipefd2[0]);
			
			//Megvárjuk, amíg befejeződik a gyerekfolyamat, majd megvizsgáljuk, melyik esemény a következő és kiírjuk a képernyőre
			wait();
			
			if (events[position+1]->name != NULL){
				printf("A következő esemény: %s\n", events[position+1]->name);
			}
			else{
				printf("Egyelőre nincs következő esemény :)\n");
			}
			
			kill(pid, SIGKILL);
		}
	}	
}

void menu() {
	puts("\nValasszon az alabbi lehetosegek kozul!\n");
    puts("1 - Latogato regisztralasa a nyilvantartasba");
	puts("2 - Latogatok kilistazasa");
	puts("3 - Latogato adatainak modositasa");
	puts("4 - Latogato adatainak torlese");
	puts("\n");
	puts("5 - Rendezveny hozzaadasa");
	puts("6 - Rendezvenyek kilistazasa");
	puts("7 - Rendezveny adatainak torlese");
	puts("\n");
	puts("8 Adatok fajlba torteno mentese\n");
	puts("9 Rendezveny inditasa\n");
	puts("0 - Kilepes\n");
	return;
}

int validateAnswer(int x) {
	return x>'9' || x<'0';
}

int main(){
	
	loadData();
	
	int valasz;
	do {
		menu();
		while (validateAnswer(valasz=getchar())) ;
		
		switch (valasz) {
			
			case '1':
				collectData();
				break;
				
			case '2':
				listVisitors();
				break;
				
			case '3':
				modifyVisitorData();
				break;
				
			case '4': 
				deleteVisitor();
				break;
				
			case '5':
				startNewEvent();
				break;
				
			case '6':
				listEvents();
				break;
				
			case '7':
				deleteEvent();
				break;
				
			case '8':
				saveData();
			break;
			
			case '9':
				startEvent();
			break;
			
			case '0':
				break;
				
			default:
				puts("Ervenytelen valasztas!");
		}
	} while (valasz!='0');

    return 0;
}