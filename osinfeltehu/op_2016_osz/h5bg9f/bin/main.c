#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
	int id, count; //azonosito, szamlalo hanyadik jelentkezoje van
} Esemeny;

typedef struct {
	char nev[50]; //név
	char email[73]; //email
	int ev, ho, nap; //rendszerido
	Esemeny *esemeny; //melyik esemenyen van
} Jelentkezo;

int esemenySzama;
Esemeny* esemeny;
int jelentkezoSzama;
Jelentkezo* jelentkezo;

void menuKiIr(); //kiirja mi mit csinal
void menu(); //meghivja a menukiirt és kezeli
void ujEsemeny();
void esemenyTorlese();
void ujJelentkezo();
void jelentkezoSzerkeszt();
void jelentkezoTorol();
void jelentkezoLista();
int sorokSzama(FILE* f);
int esemenyKeres(int id);//id alapján keres meg egy esemenyt
int jelentkezoKeres(char* nev);//név alapján keres meg egy jelentkezot
void esemenyBeolvas(FILE* esemeny);//esemenyek fájlból beolvasása
void jelentkezoBeolvas(FILE* jelentkezo);//jelentkezok fájlból beolvasása
void esemenyKiIrasa(FILE* esemeny);//esemenyek fájlba kiírása
void jelentkezoKiIrasa(FILE* jelentkezo);//jelentkezok fájlba kiírása



int main(int argc,char ** argv) {
//esemenyek beolvasasa
	FILE* esemenyek;
	if ((esemenyek = fopen("esemenyek.txt", "r")) == NULL) {
		perror("Nem sikerult az esemenyek.txt fajl megnyitasa!\n");
		exit(1);
	}
	esemenyBeolvas(esemenyek);
	fclose(esemenyek);

//jelentkezok beolvasasa
	FILE* jelentkezok;
	if ((jelentkezok = fopen("jelentkezok.txt", "r")) == NULL) {
		perror("Nem sikerult az jelentkezok.txt fajl megnyitasa!\n");
		exit(1);
	}
	jelentkezoBeolvas(jelentkezok);
	fclose(jelentkezok);

//menu kiirasa
	menu();

//esemeny kiirasa fajlba
	if ((esemenyek = fopen("esemenyek.txt", "w")) == NULL) {
		perror("Nem sikerult a esemenyek.txt fajl megnyitasa!\n");
		exit(1);
	}
	esemenyKiIrasa(esemenyek);
	fclose(esemenyek);

//jelentkezok kiirasa fajlba
	if ((jelentkezok = fopen("jelentkezok.txt", "w")) == NULL) {
		perror("Nem sikerult az jelentkezok.txt fajl megnyitasa!\n");
		exit(1);
	}
	jelentkezoKiIrasa(jelentkezok);
	fclose(jelentkezok);

	return 0;
}

void menuKiIr() {
	printf("___________________________________________\n");
	printf("|--------Valassz a menubol!---------------|\n");
	printf("|-----------------------------------------|\n");
	printf("|  1 - Uj esemeny felvetele        		  |\n");
	printf("|  2 - Esemeny torlese             		  |\n");
	printf("|  3 - Uj jelentkezok felvetele    		  |\n");
	printf("|  4 - Jelentkezo adatainak modositasa    |\n");
	printf("|  5 - Jelentkezo torlese 		          |\n");
	printf("|  6 - Jelentkezok listazasa              |\n");
	printf("|  0 - Kilepes                            |\n");
	printf("-------------------------------------------\n");
}

void menu() {
	int c;
	menuKiIr();
	printf(">>>>> ");
	scanf("%i",&c);
	while(c != 0){
		printf("\n");
		switch(c){
		case 1:
			ujEsemeny();
			break;
		case 2:
			esemenyTorlese();
			break;
		case 3:
			ujJelentkezo();
			break;
		case 4:
			jelentkezoSzerkeszt();
			break;
		case 5:
			jelentkezoTorol();
			break;
		case 6:
			jelentkezoLista();
			break;
		default:
			printf("Nincs ilyen menupont!\n");
		}
		menuKiIr();
		printf(">>>>> ");
		scanf("%i",&c);
	}
}

void esemenyBeolvas(FILE* esemenyek) {
	esemenySzama = sorokSzama(esemenyek);
	esemeny = (Esemeny*) malloc(sizeof(Esemeny) * esemenySzama);
	int i;
	for(i = 0; i < esemenySzama; ++i) {
		fscanf(esemenyek, "%d %d\n",
				&esemeny[i].id,
				&esemeny[i].count
				);
	}
}


void jelentkezoBeolvas(FILE* jelentkezok) {
	jelentkezoSzama = sorokSzama(jelentkezok);
	jelentkezo = (Jelentkezo*) malloc(sizeof(Jelentkezo) * jelentkezoSzama);
	int i;
	int EsemenyID;
	for(i = 0; i< jelentkezoSzama; ++i ) {
		fscanf(jelentkezok,"%s %s %d %d %d %d",
				&jelentkezo[i].nev,
				&jelentkezo[i].email,
				&jelentkezo[i].ev,
				&jelentkezo[i].ho,
				&jelentkezo[i].nap,
				&EsemenyID);
		printf("%s\n", &jelentkezo[i].nev);
		int j = esemenyKeres(EsemenyID);
		jelentkezo[i].esemeny = &(esemeny[j]);
  }
}

void esemenyKiIrasa(FILE* esemenyek) {
	int i;
	for(i = 0; i < esemenySzama; ++i) {
		fprintf(esemenyek, "%d %d\n",
				esemeny[i].id,
				esemeny[i].count
				);
  }
}


void jelentkezoKiIrasa(FILE* jelentkezok) {
	int i;
	for(i = 0; i < jelentkezoSzama; ++i) {
	fprintf(jelentkezok, "%s %s %d %d %d %d",
				&jelentkezo[i].nev,
				&jelentkezo[i].email,
				jelentkezo[i].ev,
				jelentkezo[i].ho,
				jelentkezo[i].nap,
				jelentkezo[i].esemeny->id);
  }
}

int esemenyKeres(int id) {
	int i = 0;
	while(i < esemenySzama && esemeny[i].id != id) {
		++i;
	}
	return i;
}

int jelentkezoKeres(char* nev) {
  int i = 0;
  while(i < jelentkezoSzama && 0 != strcmp(jelentkezo[i].nev, nev)) {
		++i;
	}
  return i;
}

void ujEsemeny() {
	Esemeny* temp = (Esemeny*) malloc(sizeof(Esemeny) * (esemenySzama + 1));
	int i;
	for (i = 0; i < esemenySzama; ++i) {
		temp[i].id = esemeny[i].id;
		temp[i].count = esemeny[i].count;
	}
	printf("Asd");
	free(esemeny);
	esemeny = temp;
	int id;
	printf("Asd");
	printf("Adja meg az uj esemeny adatait: ");
	printf("\nID: ");
	scanf("%d", &id);
	printf("Asd");
	int j = esemenyKeres(id);
	printf("Asd");
	if(j != esemenySzama) {
		printf("Mar van ilyen azonostoju esemeny!\n");
		return;
	} else {
		esemeny[i].id=id;
		esemeny[i].count=0;
		esemenySzama++;
	}
}

void esemenyTorlese() {
	printf("Adja meg a torledo esemeny ID-ját: ");
	int id;
	scanf("%i",id);
	int i = esemenyKeres(id);
	if(i == esemenySzama) {
		printf("Nincs ilyen nevu esemeny!\n");
		return;
	}
	Esemeny* temp = (Esemeny* )malloc(sizeof(Esemeny) * (esemenySzama - 1));
	int j;
	for(j = 0; j < esemenySzama; ++j) {
		if(i == j) {
		continue;
		} else if(j < i) {
			temp[j].id = esemeny[j].id;
			temp[j].count = esemeny[j].count;
		} else {
			temp[j-1].id = esemeny[j].id;
			temp[j-1].count = esemeny[j].count;
		}
	}
	free(esemeny);
	esemeny = temp;
	esemenySzama --;
}

void ujJelentkezo() {
	time_t aktIdo;
	time(&aktIdo);
	struct tm helyi = *(localtime(&aktIdo));
	int ev = helyi.tm_year + 1900;
	int ho = helyi.tm_mon + 1;
	int nap = helyi.tm_mday;

	Jelentkezo* temp = (Jelentkezo*) malloc(sizeof(Jelentkezo) * (jelentkezoSzama + 1));
	int i;
	for(i = 0; i < jelentkezoSzama; ++i) {
		strcpy(temp[i].nev, jelentkezo[i].nev);
		strcpy(temp[i].email, jelentkezo[i].email);
		temp[i].ev = jelentkezo[i].ev;
		temp[i].ho = jelentkezo[i].ho;
		temp[i].nap = jelentkezo[i].nap;
	}
	free(jelentkezo);
	jelentkezo = temp;
	int id;
	jelentkezo[jelentkezoSzama].ev = ev;
	jelentkezo[jelentkezoSzama].ho = ho;
	jelentkezo[jelentkezoSzama].nap = nap;
	printf("Adja meg az uj jelentkezo adatait: \n");
	printf("Nev: ");
	fscanf(stdin, "\n%[^;\n]", jelentkezo[i].nev);
	printf("ID: ");
	scanf("%i", id);
	printf("Email: ");
	fscanf(stdin, "\n%[^;\n]", jelentkezo[i].email);
	int j = esemenyKeres(id);
	if(j != esemenySzama){
		jelentkezo[jelentkezoSzama].esemeny = &esemeny[j];
		jelentkezo[jelentkezoSzama].esemeny->count++;
		printf("A jelentkezes sorszama:%i",jelentkezo[jelentkezoSzama].esemeny->count);
		}
	else{
	jelentkezo[jelentkezoSzama].esemeny = NULL;}
	jelentkezoSzama ++;
}

void jelentkezoSzerkeszt() {
	printf("Adja meg a modositani kivant jelentkezo nevet: ");
	char nev[40];
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = jelentkezoKeres(nev);
	if(i == jelentkezoSzama) {
		printf("Nincs ilyen nevu jelentkezo!\n");
		return;
	}
	printf("Melyik adaton szeretne modositani?\n"
			"  1. Nev\n"
			"  2. Esemeny\n"
			"  3. Email\n"
			">>>>> ");
	int input,id;
	fscanf(stdin, "%i", &input);
	switch(input) {
	case 1:
		printf("Adja meg az uj nevet: ");
		fscanf(stdin, "\n%[^;\n]", nev);
		strcpy(jelentkezo[i].nev, nev);
		break;
	case 2:
		printf("Adja meg az uj esemeny ID-ját: ");
		fscanf(stdin, "%i", &id);
		break;
	case 3:
		printf("Adja meg az uj emailt: ");
		fscanf(stdin, "\n%[^;\n]", jelentkezo[i].email);
		break;
	default:
		printf("Nem tortenet valtoztatas!\n");
		return;
	}
	if(input != 3)
		return;
	int j = esemenyKeres(id);
	if(j >= esemenySzama) {
		printf("Nem letezo esemeny!\n");
		return;
	}
	jelentkezo[i].esemeny = &esemeny[j];
}

void jelentkezoTorol() {
	printf("Adja meg a torlendo jelentkezo nevet: ");
	char nev[40];
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = jelentkezoKeres(nev);
	if(i == jelentkezoSzama) {
		printf("Nincs ilyen nevu jelentkezo!\n");
		return;
	}
	Jelentkezo* tmp = (Jelentkezo*) malloc(sizeof(Jelentkezo) * (jelentkezoSzama - 1));
	int j = 0;
	for(; j < jelentkezoSzama; ++j) {
		if(i == j) {
			continue;
		} else if(j < i) {
			strcpy(tmp[j].nev, jelentkezo[j].nev);
			strcpy(tmp[j].email, jelentkezo[j].email);
			tmp[j].ev = jelentkezo[j].ev;
			tmp[j].ho = jelentkezo[j].ho;
			tmp[j].nap = jelentkezo[j].nap;
			tmp[j].esemeny = jelentkezo[j].esemeny;
		} else {
			strcpy(tmp[j-1].nev, jelentkezo[j].nev);
			strcpy(tmp[j-1].email, jelentkezo[j].email);
			tmp[j-1].ev = jelentkezo[j].ev;
			tmp[j-1].ho = jelentkezo[j].ho;
			tmp[j-1].nap = jelentkezo[j].nap;
			tmp[j-1].esemeny = jelentkezo[j].esemeny;
		}
	}
	free(jelentkezo);
	jelentkezo = tmp;
	jelentkezoSzama--;
}

void jelentkezoLista() {
	int i = 0;
	for(; i < jelentkezoSzama; ++i) {
		printf("%s %d %d %d %d %s\n",
			&jelentkezo[i].nev,
			jelentkezo[i].ev,
			jelentkezo[i].ho,
			jelentkezo[i].nap,
			jelentkezo[i].esemeny->id,
			&jelentkezo[i].email);
	}
}


int sorokSzama(FILE* f) {
  int i = 0;
  int sorVegeChar;
  do {
    sorVegeChar = fgetc(f);
    if (sorVegeChar == '\n')
      ++i;
  } while(sorVegeChar != EOF);
  rewind(f);
  if(i < 1)
    ++i;
  return i;
}
