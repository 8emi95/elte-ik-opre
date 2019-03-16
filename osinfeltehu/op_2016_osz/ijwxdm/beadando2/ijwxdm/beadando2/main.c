#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h> //waitpid
#include <unistd.h> //includes fork, pipe()
#include <fcntl.h> // manipulate file descriptor
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // for errno, the number of last error
#include <string.h>
#include <signal.h>

struct jelentkezesElem {
    char nev[30];
    char email[30];
	int rendezvenyAzonosito;
	time_t jelentkezesIdo; //(e.g.: Mon Oct 31 21:14:02 2016)
	int regisztracioSzam;
	struct jelentkezesElem *next;
};

struct rendezvenyElem {
	int rendezvenyAzonosito;
	char rendezvenyNev[30];
	char rendezvenyHelyszin[30];
	struct rendezvenyElem *next;
};

struct jelentkezesElem *jelentkezesek = 0;
struct rendezvenyElem *rendezvenyek = 0;

int isEqual(int a, int b) {
    if (a!= b) return 0;
	return 1;
}

int getKovetkezoRegisztracioSzam(){
    struct jelentkezesElem *seged = jelentkezesek;
	int regNumber = 0;

            while (seged != 0) {
                    if(regNumber < seged->regisztracioSzam){
                         regNumber = seged->regisztracioSzam;
                    }
				seged = seged->next;
			}
			regNumber++;
    return regNumber;
}

int findRendezvenyAzonosito(int rendezvenyAzonosito) {
    int isExist =0;
	struct rendezvenyElem *seged = rendezvenyek;

	if (seged != 0) {
			do {
				if (isEqual(rendezvenyAzonosito, seged->rendezvenyAzonosito) == 1) {
					isExist = 1;
				}
				seged = seged->next;
			} while (seged != 0);
	}
	return isExist;
}

int findVendegWithRendezvenyAzonosito(int rendezvenyAzonosito) {
    int isExist =0;
	struct jelentkezesElem *seged = jelentkezesek;

	if (seged != 0) {
			do {
				if (isEqual(rendezvenyAzonosito, seged->rendezvenyAzonosito) == 1) {
					isExist = 1;
					printf(" Regisztracio szam: %d", seged->regisztracioSzam);
                    printf(" Nev: %s", seged->nev);
                    printf(" Email: %s", seged->email);
                    printf(" Rendezveny azonosito: %d", seged->rendezvenyAzonosito);
                    printf(" Jelentkezesi ido: %s", ctime(&seged->jelentkezesIdo));
				}
				seged = seged->next;
			} while (seged != 0);
	}
	return isExist;
}

void loadJelentkezes(int regisztracioSzam, char nev[], char email[], int rendezvenyAzonosito, time_t jelentkezesIdo ) {
	struct jelentkezesElem *elem;
	elem = malloc( sizeof(struct jelentkezesElem) );
	elem->next = jelentkezesek;

	int i;
	for (i = 0; i < 30; i++) elem->nev[i] = nev[i];
	for (i = 0; i < 30; i++) elem->email[i] = email[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;
	elem->jelentkezesIdo = time(&jelentkezesIdo);
	elem->regisztracioSzam = regisztracioSzam;

	jelentkezesek = elem;
}

void loadRendezveny(int rendezvenyAzonosito, char rendezvenyNev[] ,  char rendezvenyHelyszin[]) {
    struct rendezvenyElem *elem;
	elem = malloc( sizeof(struct rendezvenyElem) );
	elem->next = rendezvenyek;

	elem->rendezvenyAzonosito = rendezvenyAzonosito;
	int i;
	for (i = 0; i < 30; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];
	for (i = 0; i < 30; i++) elem->rendezvenyHelyszin[i] = rendezvenyHelyszin[i];

	rendezvenyek = elem;
}

//Adatok betöltése
void load() {
        char fJelentkezes[]="jelentkezes.bin";
        int fJ;
        fJ = open(fJelentkezes,O_RDWR|O_CREAT,S_IRWXU);

        struct stat st;
        stat(fJelentkezes, &st);
        int sizeJelentkezes = st.st_size;
        if(sizeJelentkezes !=0){
            struct jelentkezesElem seged;
            while (read(fJ, &seged,sizeof(seged)) != 0) {
                   loadJelentkezes(seged.regisztracioSzam, seged.nev, seged.email,seged.rendezvenyAzonosito,seged.jelentkezesIdo);
            }
        }
        close(fJ);

        char fRendezvenyek[]="rendezvenyek.bin";
        int fR;
        fR = open(fRendezvenyek,O_RDWR|O_CREAT,S_IRWXU);

        struct stat st2;
        stat(fRendezvenyek, &st2);
        int sizeRendezvenyek = st2.st_size;

        if(sizeRendezvenyek !=0){
            struct rendezvenyElem seged2;
            while (read(fR, &seged2,sizeof(seged2)) != 0) {
                 loadRendezveny(seged2.rendezvenyAzonosito, seged2.rendezvenyNev, seged2.rendezvenyHelyszin);
            }
        }
        close(fR);
}

//Adatok kimentése
void save() {
    char fJelentkezes[]="jelentkezes.bin";
    int fJ;
    fJ = open(fJelentkezes,O_RDWR|O_CREAT|O_TRUNC);

    struct jelentkezesElem *seged = jelentkezesek;
	while (seged != 0) {
        write(fJ, seged, sizeof(*seged));
		seged = seged->next;
	}
    close(fJ);

    char fRendezveny[]="rendezvenyek.bin";
    int fR;
    fR = open(fRendezveny,O_RDWR|O_CREAT|O_TRUNC);

    struct rendezvenyElem *seged2 = rendezvenyek;
	while (seged2 != 0) {
        write(fR, seged2, sizeof(*seged2));
		seged2 = seged2->next;
	}
    close(fR);
}

int createJelentkezes(char nev[], char email[], int rendezvenyAzonosito ) {
	int i;
	int regisztracioSzam = getKovetkezoRegisztracioSzam();
	time_t ltime;

	struct jelentkezesElem *elem;
	elem = malloc( sizeof(struct jelentkezesElem) );

	elem->next = jelentkezesek;
	for (i = 0; i < 30; i++) elem->nev[i] = nev[i];
	for (i = 0; i < 30; i++) elem->email[i] = email[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;
	elem->jelentkezesIdo = time(&ltime);
	elem->regisztracioSzam = regisztracioSzam;

	jelentkezesek = elem;
	return regisztracioSzam;
}

void vendeg_jelentkeztetes(){
    char nev[30];
    char email[30];
	int rendezvenyAzonosito;

	printf("\nVendeg jelentkeztetese:\n");

	printf("Adja meg a vendeg nevet:\n");
	scanf("%[^\n]s", &nev);
	getchar();

	printf("\nAdja meg a vendeg email cimet:\n");
	scanf("%[^\n]s", &email);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");

    int isExist;
    do{
        scanf("%d", &rendezvenyAzonosito);
        getchar();

        isExist = findRendezvenyAzonosito(rendezvenyAzonosito);

        if(isExist==0){
            printf("\nNincs ilyen azonositoju rendezveny!\n");
            printf("Adjon meg egy valid rendezveny azonositot!\n");
        }
    }while(isExist!=1);

	int regisztraciosSzam = createJelentkezes(nev, email, rendezvenyAzonosito);

	printf("\nA vendeg jelentkeztetese megtortent!\n");
	printf("Regisztracios szam: " );
	printf("%d",regisztraciosSzam );
}

int jelentkezettek_listazasa(){
    int isListEmpty = 0;
    printf("\nA rendszerben talalhato jelentkezesek:\n\n");
	struct jelentkezesElem *seged = jelentkezesek;

	if (seged != 0) {
        int i = 1;
			do {
				printf("\n%d. ", i);
				printf(" Regisztracio szam: %d", seged->regisztracioSzam);
				printf(" Nev: %s", seged->nev);
				printf(" Email: %s", seged->email);
				printf(" Rendezveny azonosito: %d", seged->rendezvenyAzonosito);
				printf(" Jelentkezesi ido: %s", ctime(&seged->jelentkezesIdo));
				i++;
				seged = seged->next;
			} while (seged != 0);
	}
	else {
        printf("Ures a jelentkezesi lista! \n");
        isListEmpty = 1;
	}
	return isListEmpty;
}

void vendeg_torles(){
int melyik;
	if(jelentkezettek_listazasa()==1){
        return;
	}
	printf("\nAdja meg hanyadik bejegyzest szeretne torolni!\n");
	scanf("%d", &melyik);
	getchar();

	struct jelentkezesElem *seged = jelentkezesek;
	struct jelentkezesElem *elozo = 0;

	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0) printf("Nincs ilyen sorszamu bejegyzes a listaban!\n");
			else {
				if (elozo == 0) jelentkezesek = seged->next;
				else elozo->next = seged->next;
				free(seged);
			}

	printf("\nA bejegyzes torolve!\n");
}

void vendeg_adat_modositas(){
	char nev[30];
    char email[30];
	int rendezvenyAzonosito;

	if(jelentkezettek_listazasa()==1){
        return;
	}

	int melyik;

	printf("\nAdja meg hanyadik bejegyzest szeretne modositani! ");
	scanf("%d", &melyik);
	getchar();
	printf("\nAdja meg az uj adatokat!\n");

	printf("Vendeg neve: ");
	scanf("%[^\n]s", &nev);
	getchar();

	printf("\nVendeg email cime: ");
	scanf("%[^\n]s", &email);
	getchar();

	printf("\nRendezveny azonosito: ");

    int isExist;
    do{
        scanf("%d", &rendezvenyAzonosito);
        getchar();

        isExist = findRendezvenyAzonosito(rendezvenyAzonosito);
        if(isExist==0){
            printf("\nNincs ilyen azonositoju rendezveny!\n");
            printf("Adjon meg egy valid rendezveny azonositot!\n");
        }
    }while(isExist!=1);

	struct jelentkezesElem *seged = jelentkezesek;

	int i = 1;
			while (seged != 0 && i != melyik) {
				seged = seged->next;
				i++;
			}
			if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
			else {
				for (i = 0; i < 30; i++) seged->nev[i] = nev[i];
				for (i = 0; i < 30; i++) seged->email[i] = email[i];
				seged->rendezvenyAzonosito = rendezvenyAzonosito;
			}

	printf("\nA bejegyzes modositva!\n");
}

void createRendezveny(char rendezvenyNev[], int rendezvenyAzonosito, char rendezvenyHelyszin[] ) {
	int i;

	struct rendezvenyElem *elem;
	elem = malloc( sizeof(struct rendezvenyElem) );

	elem->next = rendezvenyek;
	for (i = 0; i < 30; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;
    for (i = 0; i < 30; i++) elem->rendezvenyHelyszin[i] = rendezvenyHelyszin[i];
	rendezvenyek = elem;
}

int rendezvenyekListazasa(){
    int isListEmpty = 0;
    printf("\nA rendszerben talalhato rendezvenyek:\n\n");
	struct rendezvenyElem *seged = rendezvenyek;

	if (seged != 0) {
	int i = 1;
			do {
				printf("\n%d. ", i);
				printf(" Rendezveny azonosito: %d", seged->rendezvenyAzonosito);
				printf(" Nev: %s", seged->rendezvenyNev);
				printf(" Helyszin: %s", seged->rendezvenyHelyszin);
				i++;
				seged = seged->next;
			} while (seged != 0);
	}
	else {
        printf("Ures a rendezveny lista! \n");
        isListEmpty = 1;
	}
	return isListEmpty;
}

void rendezveny_letrehozas(){
    char rendezvenyNev[30];
	int rendezvenyAzonosito;
	char rendezvenyHelyszin[30];

	printf("\nRendezveny Letrehozasa:\n");

	printf("Adja meg a rendezveny nevet:\n");
	scanf("%[^\n]s", &rendezvenyNev);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");
	scanf("%d", &rendezvenyAzonosito);
	getchar();

	printf("Adja meg a rendezveny helyszinet:\n");
	scanf("%[^\n]s", &rendezvenyHelyszin);
	getchar();

    createRendezveny(rendezvenyNev, rendezvenyAzonosito,rendezvenyHelyszin);

	printf("\nA rendezveny inditasa megtortent!\n");
	rendezvenyekListazasa();
}

void rendezveny_torles(){
    int melyik;

	if(rendezvenyekListazasa()==1){
        return;
	}

	printf("\nAdja meg hanyadik bejegyzest szeretne torolni!\n");
	scanf("%d", &melyik);
	getchar();

	struct rendezvenyElem *seged = rendezvenyek;
	struct rendezvenyElem *elozo = 0;

	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0) printf("Nincs ilyen sorszamu rendezveny a listaban!\n");
			else {
				    if(findVendegWithRendezvenyAzonosito(seged->rendezvenyAzonosito)==1){
                        printf("A listazott vendegek mar jelentkeztek a rendezvenyre!\n");
                        printf("A rendezveny torlese csak az erintett bejegyzesek modositasa utan lehetseges!\n");
                        return;
				    }else if(elozo == 0){
				    rendezvenyek = seged->next;
				    }else{
				    elozo->next = seged->next;
                    free(seged);
				    }
			}
	printf("\nA rendezveny torolve!\n");
}

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int findResztvevok(int rendezvenyAzonosito) {
    int isExist =0;
	struct jelentkezesElem *segedJelentkezesElem = jelentkezesek;
	char names[20];

	if (segedJelentkezesElem != 0) {
			do {
				if (isEqual(rendezvenyAzonosito, segedJelentkezesElem->rendezvenyAzonosito) == 1) {
					isExist = 1;
					strcat(names, " ");
					strcat(names, segedJelentkezesElem->nev); //strcat(s1, s2): Concatenates string s2 onto the end of string s1.
                   // printf(" Nev: %s", seged->nev);
				}
				segedJelentkezesElem = segedJelentkezesElem->next;
			} while (segedJelentkezesElem != 0);
	}
	 printf("Resztvevok: ");
	printf(names);
	return isExist;
}

void rendezveny_lebonyolitas(){
    signal(SIGUSR1,handler);

    int sikeresseg=rand()%100; //number between 0-99

    int melyik;

	if(rendezvenyekListazasa()==1){
        return;
	}

	printf("\nAdja meg hanyadik rendezveny ideje jott el!\n");
	scanf("%d", &melyik);
	getchar();

	struct rendezvenyElem *seged = rendezvenyek;
	struct rendezvenyElem *elozo = 0;

	char rendezvenyHelyszin[30];

	char *resztvevokListaja;
	resztvevokListaja = (char*)malloc(100*sizeof(char));
	int resztvevokSzama = 0;

 	char *hianyzoResztvevokListaja;
 	hianyzoResztvevokListaja = (char*)malloc(100*sizeof(char));
	int hianyzokSzama;

	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}

			if (seged == 0) printf("Nincs ilyen sorszamu rendezveny a listaban!\n");

			else {
                for (i = 0; i < 30; i++) rendezvenyHelyszin[i] = seged ->rendezvenyHelyszin[i];

                struct jelentkezesElem *segedJelentkezesElem = jelentkezesek;

                if (segedJelentkezesElem != 0) {

                    do {

                            if (isEqual(seged->rendezvenyAzonosito, segedJelentkezesElem->rendezvenyAzonosito) == 1) {
                                ++resztvevokSzama;
                                if(resztvevokSzama == 1){
                                    strcpy(hianyzoResztvevokListaja,segedJelentkezesElem->nev);
                                }

                               strcpy(resztvevokListaja,strcat(resztvevokListaja, segedJelentkezesElem->nev));
                               strcpy(resztvevokListaja,strcat(resztvevokListaja, " "));
                            }
                            segedJelentkezesElem = segedJelentkezesElem->next;

                    } while (segedJelentkezesElem != 0);

                }

			}

    int pipefd[2]; // unnamed pipe file descriptor array
    int pipeJelentes[2];
    int pipeSikeresseg[2];
    pid_t pid;
    char sz[100];  // char array for reading from pipe
    char jelentesSzoveg[100];

   int sikeressegSzoveg;
	int status; // for waiting

    if (pipe(pipefd) == -1){
       perror("Hiba a pipe nyitaskor!");
       exit(EXIT_FAILURE);
    }

     if (pipe(pipeJelentes) == -1){
       perror("Hiba a pipe jelentes nyitaskor!");
       exit(EXIT_FAILURE);
    }

    if (pipe(pipeSikeresseg) == -1){
       perror("Hiba a pipe sikeresseg nyitaskor!");
       exit(EXIT_FAILURE);
    }

    pid = fork();	// creating parent-child processes
    if (pid == -1){
        perror("Fork hiba");
        exit(EXIT_FAILURE);
     }

     if (pid == 0){// child process
         printf("Gyerek folyamat indul!\n");
        //pipe: gyerek elolvassa hol van a rendezveny
        sleep(3);	// sleeping a few seconds, not necessary
        close(pipefd[1]);  //Usually we close the unused write end
        printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: Helyszin: %s",sz);
        printf("\n");

        //jelzés: gyerek jelzést küld szülőnek, hogy megérkezett a helyszínre
        sleep(3);
        printf("Gyerek jelzest kuld: A helyszinen vagyok!\n");
        kill(getppid(),SIGUSR1);

        //pipe: gyerek elolvassa, hogy kik a résztvevők, és kiírja a képernyőre
        read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
        printf("Gyerek olvasta uzenet: Resztvevok: %s",sz);
        printf("\n");
        close(pipefd[0]); // finally we close the used read end

        //pipe: gyerek elküldi a jelentést arról, hogy mennyire sikeres volt a rendezvény, és, hogy kik nem jöttek el (10%)
        printf("Gyerek elküldi a jelentest!\n");
        close(pipeJelentes[0]); //Usually we close unused read end

       write(pipeJelentes[1], hianyzoResztvevokListaja,100);

        close(pipeJelentes[1]);
        fflush(NULL);

        sleep(3);
        close(pipeSikeresseg[0]);
        write(pipeSikeresseg[1], &sikeresseg,sizeof(sikeresseg));
        close(pipeSikeresseg[1]);
        //printf("Random number %i\n",sikeresseg);
        fflush(NULL);
        printf("Gyerek folyamat vege!\n");
        exit(0);
     }
     else{    // parent process
        //pipe: szülõ elküldi gyereknek hol van a rendezveny
        printf("Szulo folyamat indul!\n");
        printf("Szulo elkuldi a gyereknek hol a rendezveny!\n");
        close(pipefd[0]); //Usually we close unused read end
        write(pipefd[1], &rendezvenyHelyszin,19);
        fflush(NULL); 	// flushes all write buffers (not necessary)

        //jelzes: szülõ jelzest fogad a gyerektől, hogy az megérkezett
        printf("Szulo elkezd jelzest fogadni...\n");
        pause(); //waits till a signal arrive
        printf("Szulo jelzest fogad, hogy a gyerek megerkezett!\n");

        //pipe: szülõ elküldi gyereknek hol a résztvevőket
        printf("Szulo elkuldi a gyereknek a resztvevoket...\n");
        //write(pipefd[1], &resztvevokListaja  , sizeof(resztvevokListaja));
        write(pipefd[1], resztvevokListaja  , 100);
        close(pipefd[1]); // Closing write descriptor
        printf("Szulo beirta az adatokat a csobe!\n");
        fflush(NULL); 	// flushes all write buffers (not necessary)

        //pipe: jelentés: -szülő olvas: siker: véletlenszám; kik nem érkezek meg (10%)
        sleep(3);	// sleeping a few seconds, not necessary
        close(pipeJelentes[1]);  //Usually we close the unused write end
        printf("Szulo elkezdi olvasni a jelentest!\n");
        read(pipeJelentes[0],&jelentesSzoveg,sizeof(jelentesSzoveg));
        printf("A kovetkezo vendegek nem jelentek meg: %s\n",jelentesSzoveg);
        close(pipeJelentes[0]);

        close(pipeSikeresseg[1]);
        read(pipeSikeresseg[0],&sikeressegSzoveg,sizeof(sikeressegSzoveg));
        printf("Rendezveny sikeressege: (%d /100)\n",sikeressegSzoveg);
        close(pipeSikeresseg[0]);

        wait(&status);		// waiting for child process (not necessary)

        printf("Szulo folyamat vege!\n");
     }

}

int main()
{
    load();
    printf("\n------------------------------------\n");
    printf("King of Stands\n");
    printf("Elektronikus Latogatoi Nyilvantartas\n");
	int menuNumber;

	do {
		printf("\n------------------------------------\nMenu\n\n");
		printf("1. Vendeg Jelentkeztetes\n");
		printf("2. Jelentkezettek Listazasa\n");
		printf("3. Vendeg Adatainak Modositasa\n");
		printf("4. Vendeg Torlese\n");
		printf("5. Rendezveny Letrehozasa\n");
		printf("6. Rendezveny Torlese\n");
		printf("7. Rendezveny Lebonyolitasa\n");
		printf("8. Kilepes");
		printf("\n\n------------------------------------\n");
		scanf("%d", &menuNumber);
		getchar();
		switch (menuNumber) {
			case 1:
				//Vendeg Jelentkeztetes
				vendeg_jelentkeztetes();
                break;
			case 2:
				//Jelentkezettek Listazasa
				jelentkezettek_listazasa();
                break;
			case 3:
				//Vendeg Adatainak Modositasa
				vendeg_adat_modositas();
                break;
			case 4:
				//Vendeg Torlese
				vendeg_torles();
                break;
			case 5:
				//Rendezveny Letrehozasa
                rendezveny_letrehozas();
                break;
			case 6:
			    //Rendezveny Torlese
				rendezveny_torles();
                break;
            case 7:
			    //Rendezveny lebonyolitasa
				rendezveny_lebonyolitas();
                break;
			case 8:
                break; //exit
			default:
				printf("\n\nNem letezo menupont!\n");
			break;
		}
	} while (menuNumber != 8);

	save();
    return 0;
}
