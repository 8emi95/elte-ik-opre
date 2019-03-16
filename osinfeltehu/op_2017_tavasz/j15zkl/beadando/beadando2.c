#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#define STRING_SIZE 100
#define VALASZOKAT_IS 0
#define CSAK_KERDEST 1
#define MEGVALASZOLANDOK_SZAMA 3
#define MIN_VALASZOLOK_SZAMA 10
#define MAX_VALASZOLOK_SZAMA 20

struct Kerdes {
    int valaszok_szama;
    char kerdes[STRING_SIZE];
    char valaszok[4][STRING_SIZE];
	time_t felvetel_idopontja;
};

volatile int szamlalo;
void signal_handler(int signum) {
	szamlalo++;
}

void kerdes_beolvasas(struct Kerdes * o_kerdes);
void kerdes_sor_kiir(const struct Kerdes * i_kerdessor, int hossz, int mit_listazzon);
void kerdes_kiir(const struct Kerdes * i_kerdes);
void kerdes_torlese(struct Kerdes * io_kerdessor, int * io_hossz);
void kerdes_modositasa(struct Kerdes * io_kerdessor, int i_hossz);
void kerdessor_veglegesitese(struct Kerdes * io_kerdessor, int * io_hossz);
void veglegesitett_kerdessor_kiir();
int menu();

int main() {
	int menupontok_szama = 7;
	int valasztott_menupont = 0;
	struct Kerdes * kerdes_sor;
	int kerdesek_szama = 0;
	int max_kerdesek = 100;
	
	
	srand(time(NULL));
	while(valasztott_menupont != menupontok_szama) {
		valasztott_menupont = menu(menupontok_szama);
		if(valasztott_menupont == 1) {			
			if(kerdesek_szama == 0) {
				kerdes_sor = (struct Kerdes *) malloc(max_kerdesek * sizeof(struct Kerdes));
			} else if (kerdesek_szama >= max_kerdesek) {
				max_kerdesek = 2 * max_kerdesek;
				kerdes_sor = (struct Kerdes *) realloc(kerdes_sor,max_kerdesek * sizeof(struct Kerdes));
			}
			kerdes_beolvasas(&kerdes_sor[kerdesek_szama]);
			kerdesek_szama++;
		} else if (valasztott_menupont == 2) {
			kerdes_sor_kiir(kerdes_sor,kerdesek_szama,VALASZOKAT_IS);
		} else if (valasztott_menupont == 3) {
			kerdes_sor_kiir(kerdes_sor,kerdesek_szama,CSAK_KERDEST);
			kerdes_torlese(kerdes_sor,&kerdesek_szama);
		} else if (valasztott_menupont == 4){
			kerdes_sor_kiir(kerdes_sor,kerdesek_szama,CSAK_KERDEST);
			kerdes_modositasa(kerdes_sor,kerdesek_szama);
		} else if (valasztott_menupont == 5) {
			kerdessor_veglegesitese(kerdes_sor,&kerdesek_szama);
		} else if (valasztott_menupont == 6) {
			veglegesitett_kerdessor_kiir();
		} else {
			printf("Koszonjuk, hogy az MLSZ csodalatos kerdessorara idot szakitott. \nViszont latasra!\n");
			continue;
			
		}
		printf("\nNyomjon le egy billentyut a folytatashoz...\n");
		getchar();
	}
	
    return 0;
}

void kerdes_beolvasas(struct Kerdes * o_kerdes) {
	char * buffer;
	printf("Irja be a kerdest: ");
	scanf("%[^\n]",o_kerdes->kerdes);
	getchar();
	do {
	printf("Irja be hogy hany valasz lesz [2-4]: ");
	scanf("%d",&o_kerdes->valaszok_szama);
	getchar();
	} while(o_kerdes->valaszok_szama > 4 || o_kerdes->valaszok_szama < 2);
	int i = 0;
	while(i < o_kerdes->valaszok_szama) {
		printf("Irja be a %i. valaszt: ",i+1);
		scanf("%[^\n]",o_kerdes->valaszok[i]);
		getchar();
		i++;
	}
	o_kerdes->felvetel_idopontja = time(0);
}

void kerdes_sor_kiir(const struct Kerdes * i_kerdessor, int hossz, int mit_listazzon) {
	int i = 0;
	while (i < hossz) {
		if(mit_listazzon == VALASZOKAT_IS) {
			kerdes_kiir(&i_kerdessor[i]);
		} else {
			printf("%i. %s\n",i+1,i_kerdessor[i].kerdes);
		}
		i++;
	}
}

void kerdes_kiir(const struct Kerdes * i_kerdes) {
	printf("Kerdes: %s \n%i darab valasz: \n",i_kerdes->kerdes,i_kerdes->valaszok_szama);
	int i = 0;
	while(i < i_kerdes->valaszok_szama) {
		printf("\t%i. %s\n",i+1,i_kerdes->valaszok[i]);
		i++;
	}
	printf("Felvetel idoponja: %s \n",ctime(&i_kerdes->felvetel_idopontja));
}

void kerdes_torlese(struct Kerdes * io_kerdessor, int * io_hossz) {
	int valasz;
	do {
		printf("Melyik kerdest szeretne torolni [1-%i]: ", *io_hossz);
		//getchar();
		scanf("%d",&valasz);
		getchar();
	} while(valasz < 1 || valasz > *io_hossz);
	int i;
	for(i = valasz - 1;i<(*io_hossz)-1;i++) {
		io_kerdessor[i] = io_kerdessor[i+1];
	}
	(*io_hossz)--; 
	printf("%i kerdes torlese sikeres \n",valasz);
}

void kerdes_modositasa(struct Kerdes * io_kerdessor, int i_hossz) {
	int valasz;
	do {
		printf("Melyik kerdest szeretne modositani [1-%i]: ", i_hossz);
		scanf("%d",&valasz);
		getchar();
	} while(valasz < 1 || valasz > i_hossz);
	printf("Kerem irja be az uj kerdest es a valaszait! \n");
	struct Kerdes a_kerdes;
	kerdes_beolvasas(&a_kerdes);
	io_kerdessor[valasz-1] = a_kerdes;
}

void kerdessor_veglegesitese(struct Kerdes * io_kerdessor, int * io_hossz) {
	int valasz;
	int veglegesek[*io_hossz];
	int veglegesek_szama = 0;
	int i;
	for(i = 0; i < *io_hossz; i++){
		veglegesek[i] = 0;
	}
	kerdes_sor_kiir(io_kerdessor,*io_hossz,CSAK_KERDEST);
	printf("%i. Veglegesites\n%i. Megse\n",(*io_hossz)+1,(*io_hossz)+2);
	do {
		printf("Melyik kerdest szeretne belerakni a vegleges listaba [1-%i]: ", (*io_hossz)+2);
		scanf("%d",&valasz);
		getchar();
		if(valasz == (*io_hossz)+1) {
			printf("Irja be a fajl nevet: ");
			char buffer[100];
			scanf("%[^\n]",buffer);
			getchar();
			int fd = open(buffer,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
			if(fd < 0){
				printf("HIBA A MEGNYITASKOR! \n");
				return;
			}
			if(write(fd,&veglegesek_szama,sizeof(int)) < 0) {
				printf("HIBA AZ IRASKOR! \n");
				return;
			}
			for(i = 0; i < *io_hossz; i++){
				if(veglegesek[i] == 1 && write(fd,&io_kerdessor[i],sizeof(struct Kerdes)) < 0) {
					printf("HIBA AZ IRASKOR! \n");
					return;
				}
			}
			printf("Sikeres mentes!\n");
			
			/* A masodik beadando kezdete */
			
			printf("Irja be a varos nevet: ");
			char buffer2[100];
			scanf("%[^\n]",buffer2);
			getchar();
			
			int pipefd[2];
			if(pipe(pipefd) < 0) {
				printf("Nem sikerult a csovezeteket letrehozni.\n");
				return;
			}
			
			signal(SIGUSR1,signal_handler);
			
			pid_t child = fork();
			
			if(child < 0){
				printf("Nem sikerult a kerdezobiztost a helyszinre kuldeni.\n");
				return;
			} else if (child == 0) {
				// Gyerek resze
				szamlalo = 0;
				signal(SIGUSR1,signal_handler);
				kill(getppid(),SIGUSR1);
				while(szamlalo == 0) {}
				
				struct Kerdes kerdezendo_kerdessor[MEGVALASZOLANDOK_SZAMA];
				read(pipefd[0],kerdezendo_kerdessor,sizeof(struct Kerdes) * MEGVALASZOLANDOK_SZAMA);
				
				int valaszadok_szama = MIN_VALASZOLOK_SZAMA + rand() % (MAX_VALASZOLOK_SZAMA + 1 - MIN_VALASZOLOK_SZAMA);
				int valaszok[MEGVALASZOLANDOK_SZAMA][4];
				for(i = 0; i < MEGVALASZOLANDOK_SZAMA; i++) {
					int j;
					for(j = 0; j < 4; j++) {
						valaszok[i][j] = 0;
					}
				}
				
				for(i = 0; i < valaszadok_szama; i++) {
					int j;
					for(j = 0; j < MEGVALASZOLANDOK_SZAMA; j++) {
						int valasztott_valasz = rand() % kerdezendo_kerdessor[j].valaszok_szama;
						valaszok[j][valasztott_valasz]++;
					}
				}
				
				write(pipefd[1],&valaszadok_szama,sizeof(int));
				write(pipefd[1],valaszok,sizeof(int)*MEGVALASZOLANDOK_SZAMA*4);
				close(pipefd[0]);
				close(pipefd[1]);
				kill(getppid(),SIGUSR1);
				
				exit(0);
				
				
			} else {
				// Szulo resze
				szamlalo = 0;
				while(szamlalo == 0) {}
				int sorrend[veglegesek_szama];
				for(i = 0; i < veglegesek_szama; i++) {
					sorrend[i] = i;
				}
				for(i = 0; i < veglegesek_szama; i++) {
					int rand_index1 = rand() % veglegesek_szama;
					int rand_index2 = rand() % veglegesek_szama;
					
					int tmp = sorrend[rand_index1];
					sorrend[rand_index1] = sorrend[rand_index2];
					sorrend[rand_index2] = tmp;
				}
				
				struct Kerdes kerdezendo_kerdessor[MEGVALASZOLANDOK_SZAMA];
				for(i = 0; i < MEGVALASZOLANDOK_SZAMA; i++) {
					kerdezendo_kerdessor[i] = io_kerdessor[sorrend[i % veglegesek_szama]];
				}
				write(pipefd[1],kerdezendo_kerdessor,sizeof(struct Kerdes)*MEGVALASZOLANDOK_SZAMA);
				close(pipefd[1]);
				kill(child,SIGUSR1);
				szamlalo = 0;
				while(szamlalo == 0) {}
				
				int valaszadok_szama;
				int valaszok[MEGVALASZOLANDOK_SZAMA][4];
				read(pipefd[0],&valaszadok_szama,sizeof(int));
				read(pipefd[0],valaszok,sizeof(int)*MEGVALASZOLANDOK_SZAMA*4);
				
				printf("\nOsszesen %i valaszado volt.\n",valaszadok_szama);
				for(i = 0; i < MEGVALASZOLANDOK_SZAMA; i++) {
					printf("%s\n",kerdezendo_kerdessor[i].kerdes);
					int j;
					for(j = 0; j < kerdezendo_kerdessor[i].valaszok_szama; j++) {
						printf("\t%s: %i db\n",kerdezendo_kerdessor[i].valaszok[j],valaszok[i][j]);
					}
					printf("\n");
				}
				
			}
			/* A masodik beadando vege */
			return;
			
		} else if(valasz == (*io_hossz)+2) {
			printf("Kilepett a veglegesitesbol!\n");
			return;
		} else if(valasz >= 1 && valasz <= (*io_hossz)) {
			if(veglegesek[valasz - 1] != 1) {
				veglegesek_szama++;
				veglegesek[valasz - 1] = 1;
			}
		}
	} while(1);
}

void veglegesitett_kerdessor_kiir() {
	printf("Irja be a kerdessor nevet: ");
	char buffer[100];
	scanf("%[^\n]",buffer);
	getchar();
	int fd;
	fd = open(buffer,O_RDONLY);
	if(fd < 0) {
		printf("HIBA A MEGNYITASKOR! \n");
		return;
	}
	int kerdesek_szama;
	read(fd,&kerdesek_szama,sizeof(int));
	struct Kerdes veglegesitett_kerdessor[kerdesek_szama];
	read(fd,veglegesitett_kerdessor,sizeof(struct Kerdes)*kerdesek_szama);
	kerdes_sor_kiir(veglegesitett_kerdessor,kerdesek_szama,VALASZOKAT_IS);
	
}

int menu(const int menupontok_szama) {
	system("clear");
	printf("A string beolvasi merete %i karakter! \nEnnel hosszabb input beirasa eseten NEM megfelelo mukodes jelentkezhet! \n",STRING_SIZE);
	printf("\n --------- :) --------- \n\n");
	printf("Menu: \n");
	printf("1. Uj kerdes felvetele...\n");
	printf("2. Kerdesek kilistazasa...\n");
	printf("3. Kerdes torlese...\n");
	printf("4. Kerdes modositasa...\n");
	printf("5. Kerdessor veglegesitese...\n");
	printf("6. Veglegesitett kerdessor kiiratasa\n");
	printf("%i. Kilepes\n",menupontok_szama);
	
	int valasz;
	do {
		printf("Valasszon [1-%i]: ",menupontok_szama);
		scanf("%d",&valasz);
		getchar();
	} while( valasz < 1 || valasz > menupontok_szama);
	return valasz;
	
}