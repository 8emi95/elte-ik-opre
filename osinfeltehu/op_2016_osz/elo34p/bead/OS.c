#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define n 10240

struct Room {
	char id[512];
	int available;
};

struct User {
	int id;
	char name[512];
	char mail[512];
	char time[512];
	int available;
	struct Room room;
};

struct User users[n];

struct Room rooms[n];

int room_counter = 0;

int user_counter = 0;

/*
	Megadott vendégeket és szobákat tartalmazó fájlok tartalmát beolvassa a 'users' illetve a 'rooms' struktúrák maximum 'n' elemű tömbjébe.
	Minta sor a vendég fájlból: '1;Nagy Nikolett;nickyteem@inf.elte.hu;2016.10.10;BOLYAI' ahol a vesszővel elválasztott elemek sorra a vendég
	sorszáma, neve, e-mail címe, belépésének ideje és a rendezvény azonosítója. Ezen kívül a ciklusokban egy-egy vendégeket és szobákat számláló
	változó is növelésre kerül, valamint az adott vendég/szoba státusza elérhetőre állítódik ('.available = 1').
	UI.: a ';%[^;];' regex keresés eredménye minden két ';' között található karakter, így lett megoldva, hogy a név ne csak egy szavas lehessen.
*/
void readContent(char* user_file, char* room_file) {
	FILE* f1 = fopen(user_file, "r");
	FILE* f2 = fopen(room_file, "r");
	if(f1 != NULL) {
		while(!feof(f1)) {
			fscanf(f1, "%d;%[^;];%[^;];%[^;];%[^;];\n", &users[user_counter].id, users[user_counter].name, users[user_counter].mail, users[user_counter].time, users[user_counter].room.id);
			if(strcmp(users[user_counter].time, "") != 0) {
				users[user_counter].available = 1;
				user_counter++;
			}
		}
		fclose(f1);
	}
	if(f2 != NULL) {
		while(!feof(f2)) {
			fscanf(f2, "%[^;];\n", rooms[room_counter].id);
			if(strcmp(rooms[room_counter].id, "") != 0) {
				rooms[room_counter].available = 1;
				room_counter++;
			}
		}
		fclose(f2);
	}
}

/*
	Kiírja a megadott fájlokba a 'readContent'-nél található formátumba, a futás végén elérhető vendégeket, szobákat.
*/
void writeContent(char* user_file, char* room_file) {
	FILE* f1 = fopen(user_file, "w");
	FILE* f2 = fopen(room_file, "w");
	int i;
	for(i = 0; i<user_counter; i++) {
		if(users[i].available) {
			fprintf(f1, "%d;%s;%s;%s;%s;\n", users[i].id, users[i].name, users[i].mail, users[i].time, users[i].room.id);
		}
	}
	fclose(f1);
	for(i = 0; i<room_counter; i++) {
		if(rooms[i].available) {
			fprintf(f2, "%s;\n", rooms[i].id);
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
int room_finder(char* room) {
	int i;
	int index = -1;
	for(i = 0; i<room_counter; i++) {
		if(rooms[i].available && strcmp(room, rooms[i].id) == 0) {
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
void new_visitor(char* name, char* mail, char* room) {
	if(room_finder(room) == -1) {
		printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
	}
	else {		
		users[user_counter].id = user_counter;
		strncpy(users[user_counter].name, name, 512);
		strncpy(users[user_counter].mail, mail, 512);
		strncpy(users[user_counter].room.id, room, 512);
	
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
void edit_visitor(int id, char* name, char* mail, char* room) {
	int index = user_finder(id);
	if(index == -1) {
		printf("Sajnos nincs ilyen azonositoju vendeg.\n");
	}
	else {
		if(room_finder(room) == -1) {
			printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
		}
		else {
			strncpy(users[index].name, name, 512);
			strncpy(users[index].mail, mail, 512);
			strncpy(users[index].room.id, room, 512);
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
			printf("%d %s %s %s %s\n", users[i].id, users[i].name, users[i].mail, users[i].time, users[i].room.id);
		}
	}
}

/*
	Új rendezvény rögzítése.
*/
void new_room(char* id) {
	if(room_finder(id) != -1) {
		printf("Sajnos letezik ilyen azonositoju rendezveny.\n");
	}
	else {
		strncpy(rooms[room_counter].id, id, 512);
		rooms[room_counter].available = 1;
		room_counter++;
	}
}

/*
	Kiirja az elérhető rendezvények azonosítóit.
*/
void list_rooms() {
	int i;
	for(i = 0; i<room_counter; i++) {
		if(rooms[i].available) {
			printf("%s\n", rooms[i].id);
		}
	}
}

/*
	'remove_user' el azonos működésű eljárás, csak ez rendezvények között keres és törli a megfelelőt.
*/
void remove_room(char* id) {
	int index = room_finder(id);
	if(index == -1) {
		printf("Sajnos nincs ilyen azonositoju rendezveny.\n");
	}
	else {
		int i;
		for(i=0; i<user_counter; i++) {
			if(strcmp(users[i].room.id, id) == 0) {
				users[i].available = 0;
			}
		}
		rooms[index].available = 0;
	}
}

// ----------------------------------------------------

int main() {
	char* choose = NULL;
	size_t sizes = 0;
	readContent("vendegek.txt", "rendezvenyek.txt");
	while(1) {
		printf("Kerjuk valasszon a kovetkezok kozul:\na) Uj vendeg\nb) Vendeg modositasa\nc) Vendeg torlese\nd) Vendegek listazasa\ne) Uj rendezveny\nf) Rendezveny kiuritese\ng) Rendezvenyek listazasa\nh) Kilepes\n");    
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
			new_room(ask("Add meg a rendezveny azonositojat: "));
			break;
		case 'f':
			remove_room(ask("Add meg az eltavolitando rendezveny azonositojat: "));
			break;
		case 'g':
			list_rooms();
			break;
		case 'h':
			writeContent("vendegek.txt", "rendezvenyek.txt");
			return;
		}
		printf("\n");
	}
}
