#include <stdlib.h> 
#include <stdio.h> 
#include <time.h>
#include <string.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

//konstansok
const char statuses[][30] = {"foglalt", "szabad", "rossz"};
const char stations[][30] = {"Moszkva ter", "Deli PU"};
const char actions[][30] = {"kolcsonzes", "visszahozas"};

//structok
struct bike { 
	int id;
	char station[30];
	char status[30];
	struct bike *next;
};

struct useraction {
	int bike_id;
	char name[30];
	char action[30];
	char time[12];
	struct useraction *next;
};

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ];
};

//global variables
int last_id;
struct bike *bikes = 0;
struct useraction *useractions = 0;
char now[30];

int pipe1fd[2], pipe2fd[2];
char sz[100]; // char array for reading from pipe
pid_t pid1, pid2;
int uzenetsor;
int status;


// sendig a message
int kuld(int uzenetsor, const struct uzenet uz) 
{ 
     int status;
     
     status = msgsnd(uzenetsor, &uz, strlen (uz.mtext) + 1 , 0);	
     if (status < 0) 
          perror("msgsnd");
	 else
          printf("A beirt uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext);		  
     return 0;
} 
     
// receiving a message. 
int fogad(int uzenetsor, long mtype) 
{ 
     struct uzenet uz;
     int status;    
     status = msgrcv(uzenetsor, &uz, 1024, mtype, 0);
     int allapot = 1;
	 
     if (status < 0) 
          perror("msgsnd");
     else {
          printf("A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext);
		  allapot = atoi(uz.mtext);
	 }
     return allapot;
} 

//következõ új kerékpár id-ja
init_id() {
	struct bike *seged = bikes;
	struct bike *elozo = 0;
	if (seged != 0) { 
		do {
			elozo = seged;
			seged = seged->next;
			if (elozo->id > last_id)
				last_id = elozo->id;
		} while (seged != 0);
	} else
		last_id = 0;
}

//Kerékpár listázás: 
void list_bikes() { 
	printf("\nA rendszerben talalhato kerekparok:\n");
	struct bike *seged = bikes;
	if (seged != 0) { 
		do { 
			printf("\nId: %d", seged->id);
			printf(" Alomas: %s", seged->station);
			printf(" Allapot: %s", seged->status);
			seged = seged->next;
		} while (seged != 0);
	} else 
		printf("\nUres lista \n");
} 

//Állomás listázás: 
void list_station() { 
	int i;
	for (i = 0;i < 2;i++) {
		printf("\n%d.", i+1);
		printf(" %s", stations[i]);
	}
} 

//Állomás kerékpárjainak listázása:
void list_station_bikes() { 
	char station[30];
	int number;
	int i;
	
	list_station();
	printf("\nAdd meg az allomas szamat:");
	scanf("%d", &number);
	getchar();
	for (i = 0;i < 30;i++) 
		station[i] = stations[number-1][i];
		
	printf("\nA(z) %s allomason talalhato kerekparok:\n", station);
	struct bike *seged = bikes;
	if (seged != 0) { 
		do { 
			if(strcmp(station, seged->station) == 0) {
				printf("\nId: %d", seged->id);
				printf(" Alomas: %s", seged->station);
				printf(" Allapot: %s", seged->status);				
			}
			seged = seged->next;
		} while (seged != 0);
	} else 
		printf("\nUres lista \n");
} 

//Rossz kerékpárok listázása:
void list_wrong_bikes() { 
	char status[30];
	int i;
	for (i = 0;i < 30;i++) 
		status[i] = statuses[2][i];
	
	printf("\nRossz kerekparok:\n");
	struct bike *seged = bikes;
	if (seged != 0) { 
		int i = 1;
		do { 
			if(strcmp(status, seged->status) == 0) {
				printf("\nId: %d", seged->id);
				printf(" Alomas: %s", seged->station);
				printf(" Allapot: %s", seged->status);
				i++;
			}
			seged = seged->next;
		} while (seged != 0);
	} else 
		printf("\nUres lista \n");
} 

//új kerékpár hozzáadása állomáshoz
void addbike(int id, const char station[], const char status[]) { 
	int i;
	struct bike *elem;
	elem = malloc(sizeof(struct bike));
	elem->next = bikes;
	for (i = 0;i < 30;i++) 
		elem->status[i] = status[i]; 
	for (i = 0;i < 30;i++) 
		elem->station[i] = station[i];
	elem->id = id;
	bikes = elem;
} 

//új kölcsönzés
void addrent(int id, const char name[], const char action[], const char time[]) { 
	int i;
	struct useraction *elem;
	elem = malloc(sizeof(struct useraction));
	elem->next = useractions;
	for (i = 0;i < 30;i++) 
		elem->name[i] = name[i]; 
	for (i = 0;i < 30;i++) 
		elem->action[i] = action[i];
	elem->bike_id = id;
	for (i = 0;i < 12;i++) 
		elem->time[i] = time[i];
	useractions = elem;
} 

//Hozzáadás: 
void hozzaadas() { 
	int id = ++last_id;
	int station;
	char status[30];
	int i;
	for (i = 0;i < 30;i++) 
		status[i] = statuses[1][i];
	
	printf("\nKerekpar hozzaadasa:\n");
	list_station();
	printf("\nAdd meg az allomas szamat:");
	scanf("%d", &station);
	getchar();
	addbike(id, stations[station-1], status);	
	printf("\nA kerekpar hozzaadva!\n");
} 

//kerékpár kivétele állomásról
int get_bike(const char station[]) {
	char status[30];
	int i;
	for (i = 0;i < 30;i++) 
		status[i] = statuses[1][i];
	
	struct bike *seged = bikes;
	if (seged != 0) { 
		do { 
			if(strcmp(station, seged->station) == 0 && strcmp(status, seged->status) == 0) {
				int i;
				for (i = 0;i < 30;i++) 
					seged->status[i] = statuses[0][i];
				return seged->id;
			} else
				seged = seged->next;
		} while (seged != 0);		
	} 
	printf("\nNincs kolcsonozheto kerekpar!\n");
		
	return 0;
}

//idõpont lekérése
void get_time(){
	time_t timet;
	struct tm * timeinfo;
	
	time (&timet);
	timeinfo = localtime(&timet);

	strftime(now,12,"%y%m%d%H%M",timeinfo);
	
}

//felhasználó kerékpárja
int user_bike(const char name[]) {
	char time[12];
	int id = 0;
	struct useraction *seged = useractions;
	
	if (seged != 0) { 
		do { 
			if(strcmp(name, seged->name) == 0) {
				if(atoi(seged->time) > atoi(time)) {
					int i;
					for (i = 0;i < 12;i++) 
						time[i] = seged->time[i];
					id = seged->bike_id;
				}
			} 
			seged = seged->next;
		} while (seged != 0);
		
	}
	return id;
}

//viaszahozás állomásra
void addback(int id, const char name[], const char action[], const char time[], int station, int status) { 
	int i;
	struct useraction *elem;
	elem = malloc(sizeof(struct useraction));
	elem->next = useractions;
	for (i = 0;i < 30;i++) 
		elem->name[i] = name[i]; 
	for (i = 0;i < 30;i++) 
		elem->action[i] = action[i];
	elem->bike_id = id;
	for (i = 0;i < 12;i++) 
		elem->time[i] = time[i];
	useractions = elem;
	
	struct bike *seged = bikes;
	if (seged != 0) { 
		do { 
			if(id == seged->id) {
				for (i = 0;i < 30;i++) 
					seged->station[i] = stations[station-1][i]; 
				for (i = 0;i < 30;i++) 
					seged->status[i] = statuses[status][i];				
			}
			seged = seged->next;
		} while (seged != 0);
	} 
} 

//Visszahozás:
void back(const char name[]) {
	
	char action[30];
	int i;
	for (i = 0;i < 30;i++) 
		action[i] = actions[1][i];
	
	srand(time(NULL));//the starting value of random number generation
	int r=(rand()%2)+1;//number between 1-2	
	int station = r;	
	int status;
	int bike_id;	
	bike_id = user_bike(name);
	
	char adatok[30];
	sprintf(adatok, "id: %d, nev: %s", bike_id, name);
	struct uzenet uz = { station, "" };
	for (i = 0;i < 30;i++) 
		uz.mtext[i] = adatok[i];
	kuld(uzenetsor, uz);
	if(station == 1){
		kill(pid1, SIGUSR2);
	} else if (station == 2){
		kill(pid2, SIGUSR2);
	}
	pause();
	status = fogad(uzenetsor, station);
	
	if(bike_id != 0) {
		get_time();
		addback(bike_id, name, action, now, station, status);			
		printf("\nA visszahozas sikeres!");			
	} 
	
}

//childback
int childback(int mtype) {
	fogad(uzenetsor, mtype);
	srand(time(NULL));//the starting value of random number generation
	int r=(rand()%2)+1;//number between 1-2	
	int status = r;	
	char allapot[15];
	sprintf(allapot, "%d", status);
	struct uzenet uz = { mtype, "" };
	int i;
	for (i = 0;i < 15;i++) 
		uz.mtext[i] = allapot[i];
	kuld(uzenetsor, uz); 
	kill(getppid(),SIGUSR2);
}	

//van-e kölcsönzése a visszahozónak
int have_bike(const char name[]) { 
	int count = 0;
	struct useraction *seged = useractions;
	if (seged != 0) { 
		do { 
			if(strcmp(name, seged->name) == 0) {
				count++;
			}
			seged = seged->next;
		} while (seged != 0);
	} 
	
	int mod = count % 2;
	if(mod == 0) {
		return 0;
	} 
	
	return 1;	
} 

//Kölcsönzés
void rent() {
	char action[30];
	int i;
	for (i = 0;i < 30;i++) 
		action[i] = actions[0][i];
	
	char name[30];
	int station;
	int bike_id;
	
	printf("\nAdd meg a kolcsonzo nevet:");
	scanf("%[^\n]s", &name);
	getchar();
	if(have_bike(name) == 0) {
		list_station();
		printf("\nAdd meg az allomas szamat:");
		scanf("%d", &station);
		getchar();
		bike_id = get_bike(stations[station-1]);
		if(bike_id != 0) {
			if(station == 1){
				char str[15];
				sprintf(str, "%d", bike_id);
				close(pipe1fd[0]);//Usually we close unused read end
				write(pipe1fd[1], str, sizeof(str));
				close(pipe1fd[1]);// Closing write descriptor 
				fflush(NULL);			
				printf("Szulo beirta az adatokat a csobe!\n");
				kill(pid1, SIGUSR1);
			}else if (station == 2){
				char str[15];
				sprintf(str, "%d", bike_id);
				close(pipe2fd[0]);//Usually we close unused read end
				write(pipe2fd[1], str, sizeof(str));
				close(pipe2fd[1]);// Closing write descriptor 
				fflush(NULL);			
				printf("Szulo beirta az adatokat a csobe!\n");
				kill(pid2, SIGUSR1);
			} 
			pause();
			printf("Szulo megkapta az engedelyt!\n");
			get_time();
			addrent(bike_id, name, action, now);			
			printf("\nA kolcsonzes sikeres!");
			printf("\nA kolcsonzott kerekpar azonositoja: %d \n", bike_id);
			
			sleep(5);
			back(name);
		} 
	} else
		printf("\n%s nem tud ujabb kerekpart kolcsonozni, amig vissza nem hozza az elozot!",name);
}

//childrent
void childrent(int *pipefd){
	close(pipefd[1]); //Usually we close the unused write end
	//printf("%i Gyerek elkezdi olvasni a csobol az adatokat!\n", getpid());
	read(pipefd[0],sz,sizeof(sz));// reading max 100 chars
	printf("%i Gyerek olvasta uzenet: %s",getpid() ,sz);
	printf("\n");
	close(pipefd[0]);// finally we close the used read end			
	printf("A kerekpar kiadhato\n");
	kill(getppid(),SIGUSR1);
}


//Kölcsönzés listázás: 
void list_actions() { 
	printf("\nA rendszerben talalhato kolcsonzesek:\n");
	struct useraction *seged = useractions;
	if (seged != 0) { 
		do { 
			printf("\nBike_Id: %d", seged->bike_id);
			printf(" Nev: %s", seged->name);
			printf(" Akcio: %s", seged->action);
			printf(" Ido: %s", seged->time);
			seged = seged->next;
		} while (seged != 0);
	} else 
		printf("\nUres lista \n");
			
} 

//Adott user kölcsönzéseinek listázása: 
void list_useractions() { 
	char name[30];
	
	printf("\nAdd meg a keresett nevet:");
	scanf("%[^\n]s", &name);
	getchar();
	
	printf("\%s kolcsonzesei:\n", name);
	struct useraction *seged = useractions;
	if (seged != 0) { 
		do { 
			if(strcmp(name, seged->name) == 0) {
				printf("\nBike_Id: %d", seged->bike_id);
				printf(" Nev: %s", seged->name);
				printf(" Akcio: %s", seged->action);
				printf(" Ido: %s", seged->time);
			}
			seged = seged->next;
		} while (seged != 0);
	} else 
		printf("\nUres lista \n");
			
} 

//Törlés: 
void torles() { 
	int melyik;
	char status[30];
	int i;
	for (i = 0;i < 30;i++) 
		status[i] = statuses[2][i];
	
	list_wrong_bikes();
	printf("\nAdd meg melyik azonositoju kerekpart szeretned torolni! ");
	scanf("%d", &melyik);
	getchar();
	struct bike *seged = bikes;
	struct bike *elozo = 0;
	i = 1;
	while (seged != 0 && seged->id != melyik) { 
		elozo = seged;
		seged = seged->next;
		i++;
	} 
	if (seged == 0) 
		printf("Nincs ilyen azonositoju elem a listaban!\n");
	else { 
		if(strcmp(status, seged->status) != 0){
			printf("A megadott azonositoju kerekpar nem torolheto!\n");
		} else {
			if (elozo == 0) 
				bikes = seged->next;
			else 
				elozo->next = seged->next;
			free(seged);
			printf("\nA kerekpar torolve!\n");
		}
	} 	
} 

//signal handler child1
void handlerparent(int signumber){
	printf("Parent received signal \n");
}

//signal handler child1
void handlerone(int signumber){
  //printf("Signal with number %i has arrived to handler\n",signumber);
   switch(signumber)
    {
        case SIGUSR1:
            printf("Process %d: received SIGUSR1 \n", getpid());
			childrent(pipe1fd);
			break;
		case SIGUSR2:
            printf("Process %d: received SIGUSR2 \n", getpid());
           	childback(1);				
            break;			
		default:
            break;
	}
}

//signal handler child2
void handlertwo(int signumber){
  //printf("Signal with number %i has arrived to handler\n",signumber);
   switch(signumber)
    {
        case SIGUSR1:
            printf("Process %d: received SIGUSR1 \n", getpid());
			childrent(pipe2fd);			
            break;
		case SIGUSR2:
            printf("Process %d: received SIGUSR2 \n", getpid());
            childback(2);			
            break;			
		default:
            break;
	}
}

//Adatok betöltése 
void betoltes() { 
 	if(access("bikelista.bin", 0) != -1) { 
		FILE *bikelistafile;
		bikelistafile = fopen("bikelista.bin", "rb");
		if (bikelistafile != NULL) { 
			struct bike seged;
			while (fread(&seged, sizeof(seged), 1, bikelistafile)) { 
				addbike(seged.id, seged.station, seged.status);
			} 
		} 
		fclose(bikelistafile);
	} 
	
	if(access("actionlista.bin", 0) != -1) { 
		FILE *actionlistafile;
		actionlistafile = fopen("actionlista.bin", "rb");
		if (actionlistafile != NULL) { 
			struct useraction seged2;
			while (fread(&seged2, sizeof(seged2), 1, actionlistafile)) { 
				addrent(seged2.bike_id, seged2.name, seged2.action, seged2.time);
			} 
		} 
		fclose(actionlistafile);
	} 
} 

//Adatok kimentése 
void mentes() { 
	FILE *bikelistafile;
	bikelistafile = fopen("bikelista.bin", "w");
	struct bike *seged = bikes;
	while (seged != 0) { 
		fwrite(seged, sizeof(*seged), 1, bikelistafile);
		seged = seged->next;
	} 
	fclose(bikelistafile);
	
	FILE *actionlistafile;
	actionlistafile = fopen("actionlista.bin", "w");
	struct useraction *seged2 = useractions;
	while (seged2 != 0) { 
		fwrite(seged2, sizeof(*seged2), 1, actionlistafile);
		seged2 = seged2->next;
	} 
	fclose(actionlistafile);
} 

//main
int main(int argc, char *argv[]) { 
	betoltes();
	init_id();
	printf("\nEz a program kerekparok adatait tartja nyilvan!\n");
	int vege;
	
	//signal
	signal(SIGUSR1, handlerparent);
	signal(SIGUSR2, handlerparent);
	//signal(SIGALARM,handler);
	
	//pipie-ok	
	if (pipe(pipe1fd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	if (pipe(pipe2fd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}	
	
	//messagequeue
	key_t kulcs;
	kulcs = ftok(argv[0],1);
	printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	if (uzenetsor < 0) { 
		perror("msgget");
		return 1;
	} 
	
	//childprocess
	pid1 = fork();	// creating parent-child processes
	if (pid1 == -1) 
	{
		perror("Fork hiba");
	    exit(EXIT_FAILURE);
	}
	
	if(pid1 > 0){
		
		//childprosecc
		pid2 = fork();	// creating parent-child processes
		if (pid2 == -1) 
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}
		
		if(pid2 > 0){
			//parent
			do { 
				printf("\n\n---------------\nMenu\n\n");
				printf("1. Osszes kerekpar listazasa\n");
				printf("2. Egy allomas kerekparjainak listazasa\n");		
				printf("3. Uj kerekpar hozzaadasa\n");	
				printf("4. Rossz kerekpar torlese\n");
				printf("5. Kerekpar kolcsonzese\n");				
				printf("6. Kolcsonzesek listazasa\n");
				printf("7. Adott szemely kolcsonzeseinek listazasa\n");
				printf("8. Kilepes");
				printf("\n\n---------------\n");
				scanf("%d", &vege);
				getchar();
				switch (vege) { 
					case 1: 
						//Teljes listazas 
						list_bikes();
						break;
					case 2: 
						//Allomas kerekparjainak listazasa 
						list_station_bikes();
						break;
					case 3: 
						//Hozzaadas 
						hozzaadas();
						break;
					case 4: 
						//Torles 
						torles();
						break;
					case 5: 
						//Kolcsonzes és visszahozás
						rent();						
						break;					
					case 6: 
						//Kolcsonzes lista
						list_actions();
						break;
					case 7: 
						//Adott személy lista
						list_useractions();
						break;
					case 8: 				
						break;			
					default: 
						printf("\n\nNem letezo menupont!\n");
						break;
				} 
			} while (vege != 8);
			mentes();
			
			// After terminating child process, the message queue is deleted. 
			status = msgctl(uzenetsor, IPC_RMID, NULL);
			if (status < 0) 
				 perror("msgctl");
				
			kill(pid1, SIGKILL);
			kill(pid2, SIGKILL);
		} else {			
			//child pid2
			signal(SIGUSR1, handlertwo);
			signal(SIGUSR2, handlertwo);
			while(1){
				
			}
			//printf("Masodik gyerek vege\n");
		}
		
	} else {
		//child pid1
		signal(SIGUSR1, handlerone);
		signal(SIGUSR2, handlerone);
		while(1){
			
		}
		//printf("Elso gyerek vege\n");
	}
	
	return 0;
}
