#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define n 10240

struct Event {
	char id[512];
	int available;
};

struct User {
	int id;
	char name[512];
	char mail[512];
	char time[512];
	int available;
	struct Event event;
};

struct User users[n];

struct Event events[n];

int event_counter = 0;

int user_counter = 0;

/*
	Megadott vendégeket és renedezvényeket tartalmazó fájlok tartalmát beolvassa a 'users' illetve a 'events' struktúrák maximum 'n' elemű tömbjébe.
	Minta sor a vendég fájlból: '1;Nagy Nikolett;nickyteem@inf.elte.hu;2016.10.10;BOLYAI' ahol a vesszővel elválasztott elemek sorra a vendég
	sorszáma, neve, e-mail címe, belépésének ideje és a rendezvény azonosítója. Ezen kívül a ciklusokban egy-egy vendégeket és rendezvényeket számláló
	változó is növelésre kerül, valamint az adott vendég/rendezvény státusza elérhetőre állítódik ('.available = 1').
	UI.: a ';%[^;];' regex keresés eredménye minden két ';' között található karakter, így lett megoldva, hogy a név ne csak egy szavas lehessen.
*/
void readContent(char* user_file, char* event_file) {
	FILE* f1 = fopen(user_file, "r");
	FILE* f2 = fopen(event_file, "r");
	if(f1 != NULL) {
		while(!feof(f1)) {
			fscanf(f1, "%d;%[^;];%[^;];%[^;];%[^;];\n", &users[user_counter].id, users[user_counter].name, users[user_counter].mail, users[user_counter].time, users[user_counter].event.id);
			if(strcmp(users[user_counter].time, "") != 0) {
				users[user_counter].available = 1;
				user_counter++;
			}
		}
		fclose(f1);
	}
	if(f2 != NULL) {
		while(!feof(f2)) {
			fscanf(f2, "%[^;];\n", events[event_counter].id);
			if(strcmp(events[event_counter].id, "") != 0) {
				events[event_counter].available = 1;
				event_counter++;
			}
		}
		fclose(f2);
	}
}

/*
	Kiírja a megadott fájlokba a 'readContent'-nél található formátumba, a futás végén elérhető vendégeket, szobákat.
*/
void writeContent(char* user_file, char* event_file) {
	FILE* f1 = fopen(user_file, "w");
	FILE* f2 = fopen(event_file, "w");
	int i;
	for(i = 0; i<user_counter; i++) {
		if(users[i].available) {
			fprintf(f1, "%d;%s;%s;%s;%s;\n", users[i].id, users[i].name, users[i].mail, users[i].time, users[i].event.id);
		}
	}
	fclose(f1);
	for(i = 0; i<event_counter; i++) {
		if(events[i].available) {
			fprintf(f2, "%s;\n", events[i].id);
		}
	}
	fclose(f2);
	
}

// ----------------------------------------------------

/*
	Bekér a felhasználótól a megadott üzenettel egy szöveget tartalmazó sort, majd visszatér vele 'char*' formátumban.
*/
char* ask(char* question) {
	printf("%s", question);
	char* answer = NULL;
	size_t length = 0;
	length = getline(&answer, &length, stdin);
	answer[strlen(answer)-1] = '\0';
	return answer;
}

/*
	Pesszimista lin. ker. az elérhető szobák között. Főleg a felhasználótól bekért szoba valódiságának ellenőrzésére van használva.
	Visszatérési értéke a 'user_finder'-el azonos.
*/
int event_finder(char* event) {
	int i;
	int index = -1;
	for(i = 0; i<event_counter; i++) {
		if(events[i].available && strcmp(event, events[i].id) == 0) {
			index = i;
			break;
		}
	}
	return index;
} 

/*
	Pesszimista lin. ker. az elérhető vendégek között. Főleg a felhasználó álltal mdosítandó, eltávolítandó vendég keresésére van használva.
	Találat esetén a vendég 'users' tömbben elfoglalt helyét adja vissza, különben '-1'-et.
*/
int user_finder(int id) {
	int i;
	int index = -1;
	for(i = 0; i<user_counter; i++) {
		if(users[i].available && id == users[i].id) {
			index = i;
			break;
		}
	}
	return index;
}

// ----------------------------------------------------

/*
	Megadott paraméterekkel felvesz egy új vendéget a 'users' tömb következő elérhető helyére, majd amennyiben valós szobát adott meg a felhasználó,
	'available'-re állítja a sátuszát és a szobát felveszi az indexelésbe ('user_counter++').
*/
void new_visitor(char* name, char* mail, char* event) {
	if(event_finder(event) == -1) {
		printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
	}
	else {		
		users[user_counter].id = user_counter;
		strncpy(users[user_counter].name, name, 512);
		strncpy(users[user_counter].mail, mail, 512);
		strncpy(users[user_counter].event.id, event, 512);
	
		// Rendszeridő lekérése, szöveggé alakítása, sor vége jel eltávolítása.
		time_t t;
	    	time(&t);
		char* tmp = ctime(&t);
		tmp[strlen(tmp)-1] = '\0';
		strncpy(users[user_counter].time, tmp, 512);
		// -------------------------------------------------------------------

		users[user_counter].available = 1;
		printf("Udvozoljuk on a(z) %d. szamu vendeg.\n", users[user_counter].id);
		user_counter++;
	}
}

/*
	'user_finder' és 'room_finder' eljárások segítségével módosíthatóak egy vendég adatai.
*/
void edit_visitor(int id, char* name, char* mail, char* event) {
	int index = user_finder(id);
	if(index == -1) {
		printf("Sajnos nincs ilyen azonositoju vendeg.\n");
	}
	else {
		if(event_finder(event) == -1) {
			printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
		}
		else {
			strncpy(users[index].name, name, 512);
			strncpy(users[index].mail, mail, 512);
			strncpy(users[index].event.id, event, 512);
		}
	}
}

/*
	A 'user_finder' eljárás segítségével elérhetetlenné teszi a megadott azonosítóval rendelkező felhasználót. Hibás azonosító esetén hibaüzenetet ír ki.
*/
void remove_visitor(int id) {
	int index = user_finder(id);
	if(index == -1) {
		printf("Sajnos nincs ilyen azonositoju vendeg.\n");
	}
	else {
		users[index].available = 0;
	}
}

/*
	Kilistázza az elérhető vendégeket azok minden adatával.
*/
void list_visitors() {
	int i;
	for(i = 0; i<user_counter; i++) {
		if(users[i].available) {
			printf("%d %s %s %s %s\n", users[i].id, users[i].name, users[i].mail, users[i].time, users[i].event.id);
		}
	}
}

int count_visitors(char* event_id) {
	int c = 0;
	int i = 0;
	for(i = 0; i<user_counter; i++) {
		if(users[i].available && strcmp(event_id, users[i].event.id) == 0) {
			c++;
		}
	}
	return c;
}

int* collect_visitors(char* event_id) {
	int c = 0;
	int i = 0;
	int indexes[count_visitors(event_id)];
	for(i = 0; i<user_counter; i++) {
		if(users[i].available && strcmp(event_id, users[i].event.id) == 0) {
			indexes[c] = i;
			c++;
		}
	}
	return indexes;
}

/*
	Új rendezvény rögzítése.
*/
void new_event(char* id) {
	if(event_finder(id) != -1) {
		printf("Sajnos letezik ilyen azonositoju rendezveny.\n");
	}
	else {
		strncpy(events[event_counter].id, id, 512);
		events[event_counter].available = 1;
		event_counter++;
	}
}

/*
	Kiirja az elérhető rendezvények azonosítóit.
*/
void list_events() {
	int i;
	for(i = 0; i<event_counter; i++) {
		if(events[i].available) {
			printf("%s\n", events[i].id);
		}
	}
}

/*
	'remove_user' el azonos működésű eljárás, csak ez rendezvények között keres és törli a megfelelőt.
*/
void remove_event(char* id) {
	int index = event_finder(id);
	if(index == -1) {
		printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
	}
	else {
		int i;
		for(i=0; i<user_counter; i++) {
			if(strcmp(users[i].event.id, id) == 0) {
				users[i].available = 0;
			}
		}
		events[index].available = 0;
	}
}

void event_execution(char* event_id) {
	int pipefd[2]; 		//egy cso, amibe ír a szülő, olvas a gyerek
	int pipefd2[2]; 	//egy cso, amibe ír a gyerek, olvas a szulo
	pid_t pid; 			//folyamat azonosító (h gyerek - v szulo)
	
	//megnyitjuk az egyik csovet:
	if(pipe(pipefd) == -1) {
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	//megnyitjuk a masik csovet:
	if(pipe(pipefd2) == -1) {
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	//forkkal letrejon a gyerekfolyamat
	pid = fork();
	if(pid == -1) {
		perror("Hiba a forknal.");
		exit(EXIT_FAILURE);
	}
	
	if(pid == 0) {
		//gyerekfolyamat
		sleep(1);
		write(pipefd2[1],"Megerkeztem",1000);
		
		int i;
		int db = 0;
		read(pipefd[0],&db,sizeof(db));
		printf("DB : %d\n",db);
		char kapott_vendeg[1000];
		char vendegek[db][1000];
		int nem_erkezett_meg = 0;
		for(i = 0; i<db;i++){
			int random = rand() % 100;			
			read(pipefd[0],kapott_vendeg,1000);
			if(random > 10){
				printf("kapott_vendeg: %s\n", kapott_vendeg);
			}else{
				stpcpy(&vendegek[nem_erkezett_meg],kapott_vendeg);
				nem_erkezett_meg++;
			}
		}
		int r = rand() % 5 + 1;
		write(pipefd2[1],&r,sizeof(r));
		write(pipefd2[1],&nem_erkezett_meg,sizeof(nem_erkezett_meg));
		for(i = 0; i<nem_erkezett_meg;i++){
			write(pipefd2[1],&vendegek[i],1000);
		}
		
		exit(EXIT_SUCCESS);
	} else {
		int db = count_visitors(event_id);
		char vendegek[db][1000];
		//szulofolyamat
		printf("Szulo folyamat indul.\n");

		char vendeg[1000];
		read(pipefd2[0],vendeg,1000);
		printf("A gyerek uzenete: %s\n", vendeg);
		
		//beolvassuk az osszes vendeget egy tombbe
		int i;
		int* indexes = collect_visitors(event_id);
		for(i=0; i<db; i++) {
			stpcpy(&vendegek[i], users[*(indexes + i)].name);
		}
		
		//elkuldjuk a gyerekfolyamatnak a vendegek szamat, majd a vendegeket
		write(pipefd[1],&db,sizeof(db));
		for(i = 0; i<db;i++){
			write(pipefd[1],&vendegek[i],1000);
		}
		
		wait();
		int r;
		read(pipefd2[0],&r,sizeof(r));
		printf("Rendezveny sikeressege(1-5): %d\n",r);
		read(pipefd2[0],&db,sizeof(db));
		printf("Nem erkzett meg: %d\n",db);
		for(i = 0; i<db;i++){	
			read(pipefd2[0],vendeg,1000);
			printf("vendeg: %s\n", vendeg);
		}
		
		// Lebonyolitott rendezveny eltavolitasa
		remove_event(event_id);
	}
}

// ----------------------------------------------------

int main() {
	char* choose = NULL;
	size_t sizes = 0;
	readContent("vendegek.txt", "rendezvenyek.txt");
	while(1) {
		printf("Kerjuk valasszon a kovetkezok kozul:\na) Uj vendeg\nb) Vendeg modositasa\nc) Vendeg torlese\nd) Vendegek listazasa\ne) Uj rendezveny\nf) Rendezveny kiuritese\ng) Rendezvenyek listazasa\nh) Rendezvenyek lebonyolitasa\ni) Kilepes\n");    
		getline(&choose, &sizes, stdin);
		switch(choose[0]) {
		case 'a':
			new_visitor(ask("Add meg a neved: "), ask("Add meg a cimed: "), ask("Add meg a rendezveny azonositojat: "));
			break;
		case 'b':
			edit_visitor(atoi(ask("Add meg a modositando vendeg azonositojat: ")), ask("Add meg az uj neved: "), ask("Add meg az uj cimed: "), ask("Add meg az uj rendezveny azonositojat: "));
			break;
		case 'c':
			remove_visitor(atoi(ask("Add meg az eltavolitando vendeg azonositojat: ")));
			break;
		case 'd':
			list_visitors();
			break;
		case 'e':
			new_event(ask("Add meg a rendezveny azonositojat: "));
			break;
		case 'f':
			remove_event(ask("Add meg az eltavolitando rendezveny azonositojat: "));
			break;
		case 'g':
			list_events();
			break;
		case 'h':
			event_execution(ask("Add meg a lebonyolitando rendezveny azonositojat: "));
			break;
		case 'i':
			writeContent("vendegek.txt", "rendezvenyek.txt");
			return;
		}
		printf("\n");
	}
}
