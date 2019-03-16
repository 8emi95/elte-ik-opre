#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>


#include "segedfuggvenyek.h"

#define STRING_SIZE 100
#define KEDVEZMENY 0.15
#define MIN_CSOPORT_LETSZAM 5

typedef enum e_utvonal {PARLAMENT = 1, HOSOK_TERE = 2, VAR = 3} utvonal;
typedef enum e_kiiras_tipus {NORMAL, RESZLETES, UTVONAL_SZERINT} kiiras_tipus;

typedef struct s_jelentkezo {
	char nev[STRING_SIZE];
	char email[STRING_SIZE];
	char telefonszam[STRING_SIZE];
	utvonal utvonal;
	int jelentkezok_szama;
	int vegosszeg;
	time_t felveteli_datum;
	
} jelentkezo;


int const JEGYAR = 100;

jelentkezo jelentkezo_felvetele();
void bekeres_ellenorzese(char * prompt, char * buffer, size_t buffer_size);
void jelentkezo_modositasa(jelentkezo * jelentkezok, int osszes_jelentkezo);
void jelentkezok_kiirasa(jelentkezo * jelentkezok, int osszes_jelentkezo, kiiras_tipus kiiras_tipus);
void jelentkezo_torlese(jelentkezo * jelentkezok, int osszes_jelentkezo);
void jelentkezo_kiirasa(jelentkezo jelentkezo);
void beolvasas_fajlbol(jelentkezo * jelentkezok, int * osszes_jelentkezo);
void kiiras_fajlba(jelentkezo * jelentkezok, int osszes_jelentkezo);

//Második beadadó
int volatile szamlalo;
void sortura_inditasa(jelentkezo * jelentkezok, int osszes_jelentkezo);
void signal_handler(int signum) {
	szamlalo++;
}

int main()
{
	int valasztott_menupont;
	jelentkezo jelentkezok[100];
	int osszes_jelentkezo = 0;
	
	do{
		system("clear");
		
		printf("Valasszon az alabbi menupontok kozul: \n");
		printf("1. Uj jelentkezo felvetele \n");
		printf("2. Jelentkezo modositasa \n");
		printf("3. Jelentkezo torlese \n");
		printf("4. Jelentkezok kiirasa \n");
		printf("5. Jelentkezok reszletes kiirasa \n");
		printf("6. Jelentkezok kiirasa utvonalak szerint \n");
		printf("7. Jelentkezok beolvasasa fajlbol \n");
		printf("8. Jelentkezok kiirasa fajlba \n");
		printf("9. Sortura inditasa \n");
		printf("10. Kilepes \n");
		
		printf("Valasztott menupont: ");
		scanf("%d",&valasztott_menupont);
		getchar();
		
		switch(valasztott_menupont) {
			case 1: 
				jelentkezok[osszes_jelentkezo] = jelentkezo_felvetele();
				osszes_jelentkezo++;
				break;
			case 2: 
				jelentkezo_modositasa(jelentkezok, osszes_jelentkezo);
				break;
			case 3:
				jelentkezo_torlese(jelentkezok, osszes_jelentkezo);
				osszes_jelentkezo--;
				break;
			case 4: 
				jelentkezok_kiirasa(jelentkezok, osszes_jelentkezo, NORMAL);
				break;
			case 5: 
				jelentkezok_kiirasa(jelentkezok, osszes_jelentkezo, RESZLETES);
				break;
			case 6: 
				jelentkezok_kiirasa(jelentkezok, osszes_jelentkezo, UTVONAL_SZERINT);
				break;
			case 7: 
				beolvasas_fajlbol(jelentkezok, &osszes_jelentkezo);
				break;
			case 8: 
				kiiras_fajlba(jelentkezok, osszes_jelentkezo);
				break;
			case 9: 
				sortura_inditasa(jelentkezok,osszes_jelentkezo);
				break;
			default:
				return;
		}
		printf("\nNyomjon le egy billentyut a folytatashoz...\n");
		getchar();
		
	} while(1);
	
	return 0;
}

void bekeres_ellenorzese(char * prompt, char * buffer, size_t buffer_size) {
	while(0 != getLine(prompt, buffer, buffer_size)) {
		printf("Nincs input vagy tul hosszu, a maximalis input merete: %i",buffer_size);
	}
}

jelentkezo jelentkezo_felvetele() {
	jelentkezo uj_jelentkezo;
	
	bekeres_ellenorzese("Irja be a jelentkezo nevet: ", uj_jelentkezo.nev, STRING_SIZE);
	bekeres_ellenorzese("Irja be a jelentkezo email cimet: ", uj_jelentkezo.email, STRING_SIZE);
	bekeres_ellenorzese("Irja be a jelentkezo telefonszamat: ", uj_jelentkezo.telefonszam, STRING_SIZE);
	do {
		printf("1. Parlament \n");
		printf("2. Hosok tere \n");
		printf("3. Var \n");
		printf("Irja be hogy melyik utvonalat valasztja [1-3]: ");
		scanf("%d",&uj_jelentkezo.utvonal);
		getchar();
	} while(uj_jelentkezo.utvonal < 1 || uj_jelentkezo.utvonal > 3);
	do {
		printf("Irja be hanyan jelentkeznek (onmagat is beleertve): ");
		scanf("%d",&uj_jelentkezo.jelentkezok_szama);
		getchar();
	} while(uj_jelentkezo.jelentkezok_szama < 1);
	uj_jelentkezo.felveteli_datum = time(0);
	//uj_jelentkezo.vegosszeg = JEGYAR + (JEGYAR*KEDVEZMENY)*(uj_jelentkezo.jelentkezok_szama-1);
	
	return uj_jelentkezo;
}

void jelentkezo_modositasa(jelentkezo * jelentkezok, int osszes_jelentkezo){
	jelentkezok_kiirasa(jelentkezok, osszes_jelentkezo, NORMAL);
	int valasztott;
	do {
		printf("Valasszon egy jelentkezot [1-%i]: ", osszes_jelentkezo);
		scanf("%d",&valasztott);
		getchar();
	} while(valasztott < 1 || valasztott > osszes_jelentkezo);
	printf("Kerjuk adja meg a jelentkezo uj adatait! \n");
	jelentkezo modositott_jelentkezo = jelentkezo_felvetele();
	jelentkezok[valasztott-1] = modositott_jelentkezo;
}

void jelentkezok_kiirasa(jelentkezo * jelentkezok, int osszes_jelentkezo, kiiras_tipus kiiras_tipus) {
	utvonal utvonal_valasztas;
	if(kiiras_tipus == UTVONAL_SZERINT) {
		do {
			printf("1. Parlament \n");
			printf("2. Hosok tere \n");
			printf("3. Var \n");
			printf("Irja be hogy melyik utvonalat valasztja [1-3]: ");
			scanf("%d",&utvonal_valasztas);
			getchar();
		} while(utvonal_valasztas < 1 || utvonal_valasztas > 3);
	}
	
	int i;
	int j;
	for(i = 0; i < osszes_jelentkezo; i++) {
		switch(kiiras_tipus) {
			case NORMAL:
				printf("%i. %s \n", i+1, jelentkezok[i].nev);
				break;
			case RESZLETES:
				jelentkezo_kiirasa(jelentkezok[i]);
				break;
			case UTVONAL_SZERINT:
				if(jelentkezok[i].utvonal == utvonal_valasztas) {
					printf("%i. %s \n", i+1, jelentkezok[i].nev);
					j++;
				}
				else if(i == osszes_jelentkezo-1 && j == 0) {
					printf("Erre az utvonalra meg nincs jelentkezo. \n");
				}
				break;
		}
	}
}

void jelentkezo_torlese(jelentkezo * jelentkezok, int osszes_jelentkezo) {
	jelentkezok_kiirasa(jelentkezok, osszes_jelentkezo, NORMAL);
	int valasztott;
	do {
		printf("Valasszon egy jelentkezot [1-%i]: ", osszes_jelentkezo);
		scanf("%d",&valasztott);
		getchar();
	} while(valasztott < 1 || valasztott > osszes_jelentkezo);
	int i;
	for(i = valasztott - 1; i < osszes_jelentkezo - 1; i++) {
		jelentkezok[i] = jelentkezok[i + 1];
	}
}

void jelentkezo_kiirasa(jelentkezo jelentkezo) {
	printf("Jelentkezo neve: %s \n", jelentkezo.nev);
	printf("Jelentkezo email cime: %s \n", jelentkezo.email);
	printf("Jelentkezo telefonszama: %s \n", jelentkezo.telefonszam);
	printf("Utvonal: ");
	switch(jelentkezo.utvonal) {
		case 1:
			printf("Parlament \n");
			break;
		case 2:
			printf("Hosok tere \n");
			break;
		case 3:
			printf("Var \n");
			break;
	}
	printf("Jelentkezok szama: %d \n", jelentkezo.jelentkezok_szama);
	printf("Felvetel idoponja: %s \n",ctime(&jelentkezo.felveteli_datum));
	printf("\n");
}

void beolvasas_fajlbol(jelentkezo * jelentkezok, int * osszes_jelentkezo) {
	char fajlnev[STRING_SIZE];
	bekeres_ellenorzese("Irja be a fajlnevet: ", fajlnev, STRING_SIZE);
	int fd = open(fajlnev,O_RDONLY);
	if(fd < 0){
		printf("HIBA A MEGNYITASKOR! \n");
		return;
	}
	if(read(fd,osszes_jelentkezo,sizeof(int)) < 0) {
		printf("HIBA A BEOLVASASKOR! \n");
		return;
	}
	if(read(fd, jelentkezok, sizeof(jelentkezo) * *osszes_jelentkezo) < 0) {
		printf("HIBA A BEOLVASASKOR! \n");
		return;
	}
	printf("Sikeres beolvasas! \n");
}

void kiiras_fajlba(jelentkezo * jelentkezok, int osszes_jelentkezo) {
	char fajlnev[STRING_SIZE];
	bekeres_ellenorzese("Irja be a fajlnevet: ", fajlnev, STRING_SIZE);
	int fd = open(fajlnev,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd < 0){
		printf("HIBA A MEGNYITASKOR! \n");
		return;
	}
	if(write(fd,&osszes_jelentkezo,sizeof(int)) < 0) {
		printf("HIBA AZ IRASKOR! \n");
		return;
	}
	if(write(fd, jelentkezok, sizeof(jelentkezo) * osszes_jelentkezo) < 0) {
		printf("HIBA AZ IRASKOR! \n");
		return;
	}
	printf("Sikeres mentes!\n");
}

//Második beadadó
void sortura_inditasa(jelentkezo * jelentkezok, int osszes_jelentkezo) {
	int jelentkezok_szama_utvonalankent[3] = { 0 };
	int van_indithato_csoport = 0;
	
	int i;
	for(i = 0; i < osszes_jelentkezo; i++) {
		jelentkezok_szama_utvonalankent[jelentkezok[i].utvonal-1] += jelentkezok[i].jelentkezok_szama;
		if(jelentkezok_szama_utvonalankent[jelentkezok[i].utvonal-1] >= MIN_CSOPORT_LETSZAM) {
			van_indithato_csoport = 1;
		}
	}
	
	if(van_indithato_csoport == 0) {
		printf("Nincs indithato csoport!\n");
		return;
	}
	
	char* helyek[3] = {"Parlament", "Hosok tere", "Var"};
	for(i = 0; i < 3; i++) {
		printf("%i. %s: %d fo",i+1,helyek[i], jelentkezok_szama_utvonalankent[i]);
		if(jelentkezok_szama_utvonalankent[i] < MIN_CSOPORT_LETSZAM) {
			printf(" - Nem indithato!");
		}
		printf("\n");
	}
	int valasztott;
	do {
		printf("Valasszon egy utvonalat [1-3]: ");
		scanf("%d",&valasztott);
		getchar();
	} while(valasztott < 1 || valasztott > 3 || jelentkezok_szama_utvonalankent[valasztott-1] < MIN_CSOPORT_LETSZAM);
	
	int kiv_jelentkezok_meret = 0;
	jelentkezo kiv_jelentkezok[osszes_jelentkezo];
	for(i = 0; i < osszes_jelentkezo; i++) {
		if(jelentkezok[i].utvonal == valasztott) {
			kiv_jelentkezok[kiv_jelentkezok_meret] = jelentkezok[i];
			kiv_jelentkezok_meret++;
		}
	}
	
	int pipefd[2];
	if(pipe(pipefd) < 0) {
		printf("Nem sikerult a csovezeteket letrehozni.\n");
		return;
	}
	
	signal(SIGUSR1,signal_handler);
	
	pid_t child = fork();
	if(child < 0){
		printf("Nem sikerult a kocsisfiunak elindulni tankolni, mert beleallt a \"villa\".\n");
		return;
	} else if(child == 0) {
		//kocsis fiu
		signal(SIGUSR1, signal_handler);
		
		szamlalo = 0;
		kill(getppid(), SIGUSR1);
		while(szamlalo == 0) {
			usleep(100);
		}
		
		int resztvevok_szama;
		int jegyar;
		int ossz_jegyar = 0;
		int ossz_sor_liter = 0;
		read(pipefd[0], &resztvevok_szama, sizeof(int));
		jelentkezo resztvevok[resztvevok_szama];
		read(pipefd[0], resztvevok, sizeof(jelentkezo)*resztvevok_szama);
		read(pipefd[0], &jegyar, sizeof(int));
		
		printf("SBD: %s\n", helyek[resztvevok[0].utvonal-1]);
		printf("SBD: jegyar: %d HUF\n--Jelentkezok--\n", jegyar);
		for(i = 0; i < resztvevok_szama; i++) {
			ossz_jegyar += jegyar + (jegyar*KEDVEZMENY)*(resztvevok[i].jelentkezok_szama-1);
			ossz_sor_liter += resztvevok[i].jelentkezok_szama * 5;
			printf("SBD: %d. %s - %d fo \n", i+1, resztvevok[i].nev, resztvevok[i].jelentkezok_szama);
		}
		
		write(pipefd[1], &ossz_jegyar, sizeof(int));
		write(pipefd[1], &ossz_sor_liter, sizeof(int));
		printf("SBD: elindult a tura\n");
		
		
		kill(getppid(), SIGUSR1);
		srand(time(0) * getpid());
		usleep(random() % 3000);
		
		printf("SBD: veget ert a tura\n");
		kill(getppid(), SIGUSR1);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(0);
		
	} else {
		//SKB fohadiszallas
		szamlalo = 0;
		while(szamlalo == 0) {
			usleep(100);
		}
		
		write(pipefd[1], &kiv_jelentkezok_meret, sizeof(int));
		write(pipefd[1], kiv_jelentkezok, sizeof(jelentkezo)*kiv_jelentkezok_meret);
		write(pipefd[1], &JEGYAR, sizeof(int));
		
		szamlalo = 0;
		kill(child, SIGUSR1);
		while(szamlalo == 0) {
			usleep(100);
		}
		
		int ossz_jegyar = 0;
		int ossz_sor_liter = 0;
		read(pipefd[0], &ossz_jegyar, sizeof(int));
		read(pipefd[0], &ossz_sor_liter, sizeof(int));
		printf("SKB: SBD elindult...\n");
		szamlalo = 0;
		while(szamlalo == 0) {
			usleep(100);
		}
		printf("SKB: SBD befejezte biztonsagosan a turajat\n");
		printf("Bevetele: %d HUF\n", ossz_jegyar);
		printf("Elfogyott sor: %d\n", ossz_sor_liter);
	}
	
}