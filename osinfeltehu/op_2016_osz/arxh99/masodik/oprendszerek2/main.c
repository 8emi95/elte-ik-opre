#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef enum {false=0, true=1} bool;

//felhasznalt segedanyag: http://www.tankonyvtar.hu/hu/tartalom/tamop412A/2010-0011_szamalap2/lecke3_lap9.scorml

struct jelentkezes {
	int ID;
	char jelentkezoNev[50];
	char jelentkezoEmail[30];
	int rendezvenyID;
	time_t jelentkezesiIdo;
	struct jelentkezes *next;
};

struct rendezveny {
	int ID;
	char rendezvenyNev[50];
	struct rendezveny *next;
};

struct jelentkezes *jelentkezesek = 0;
struct rendezveny *rendezvenyek = 0;

int is_the_same(int a, int b) {
	if (a != b) return 0;
	return 1;
}
int is_the_same2(char a[], char b[]) {
 int i;
 for (i = 0; i < 30; i++) if (a[i] != b[i]) return 0;
 return 1;
}

void addjelentkezes(char jelentkezoNev[], char jelentkezoEmail[], int rendezvenyID) {
	struct jelentkezes *elem;
	time_t jelentkezesiIdo;
	elem = malloc( sizeof(struct jelentkezes));
	elem->next = jelentkezesek;
	int i;
	for (i = 0; i < 50; i++) elem->jelentkezoNev[i] = jelentkezoNev[i];
	for (i = 0; i < 30; i++) elem->jelentkezoEmail[i] = jelentkezoEmail[i];
	elem->rendezvenyID=rendezvenyID;
	elem->jelentkezesiIdo=time(&jelentkezesiIdo);
	int ujID;
	if (jelentkezesek ==0){
		ujID =1;	

	}
	else{
		ujID=jelentkezesek->ID+1;

	}
	elem->ID = ujID;
	jelentkezesek = elem;
}

void addrendezveny(char rendezvenyNev[], int rendezvenyID) {
	int i;
	struct rendezveny *elem;
	elem = malloc( sizeof(struct rendezveny) );
	elem->next = rendezvenyek;
	for (i = 0; i < 50; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];
	elem->ID = rendezvenyID;
	rendezvenyek = elem;
}

//Jelentkezesek Listázás:
void listazas1() {
	printf("\nA rendszerben talalhato jelentkezesek:\n\n");
	struct jelentkezes *seged = jelentkezesek;
	if (seged != 0) {
	int i = 1;
			do {
				printf("\n%d. ", i);
				printf(" Id: %d |", seged->ID);
				printf("\n");
				printf("Jelentkezo neve: %s", seged->jelentkezoNev);
				printf("\n");
				printf("Jelentkezo email cime: %s", seged->jelentkezoEmail);
				printf("\n");
				printf("Rendezveny azonosito: %d", seged->rendezvenyID);
				printf("\n");
				printf("Jelentkezes ideje: %s", ctime(&seged->jelentkezesiIdo));
				i++;
				seged = seged->next;
			} while (seged != 0);
	}
	else printf("Ures a jelentkezesi lista \n");

}
//jelentkezesek az adottrendezvenyre
void listazas3(int keresettRendezvenyID){
	struct rendezveny *seged = rendezvenyek;
	bool ures = true; 
	int i=0;
	
	if(seged != 0){
		struct jelentkezes *seged2 = jelentkezesek;
		if(seged2 !=0){
			while(seged2 !=0){
				if(seged2->rendezvenyID == keresettRendezvenyID){
					i++;
					printf("\n%d. ", i);
					printf(" Id: %d ", seged2->ID);
					printf("\n");
					printf("Jelentkezo neve: %s", seged2->jelentkezoNev);
					printf("\n");
					printf("Jelentkezo email cime: %s", seged2->jelentkezoEmail);
					printf("\n");
					printf("Jelentkezes ideje: %s", ctime(&seged2->jelentkezesiIdo));
					printf("\n");
					ures = false;
				}
				seged2 = seged2->next;
			}
			if(ures==true){
				printf("Nincs jelentkezes az adott rendezvenyre.");
			}
		}else{
			printf("Nincs jelentkezes egyik rendezvenyre se.");
		}
	}else{
		//
	}

}

//rendezvenyek listazasa
void listazas2(){
    printf("\nA rendszerben talalhato rendezvenyek:\n\n");
	struct rendezveny *elem = rendezvenyek;

	if (elem != 0) {
	int i = 1;
			do {
				printf("\n%d. ", i);
				printf(" Rendezveny azonosito: %d", elem->ID);
				printf(" Nev: %s", elem->rendezvenyNev);
				printf("\n");
				printf("Jelentkezesek:\n");
				listazas3(elem->ID);
				printf("\n");
				i++;
				elem = elem->next;
			} while (elem != 0);
	}
	else printf("Ures a rendezveny lista! \n");
	
}



//Keresés van-e ilyen rendezveny:
int kereses1(int rendezvenyID) {

    int vanE =0;
	struct rendezveny *seged = rendezvenyek;

	if (seged != 0) {
			do {
				if (is_the_same(rendezvenyID, seged->ID) == 1) {
					vanE = 1;
				}
				seged = seged->next;
			} while (seged != 0);
	}
	return vanE;
}


//Jelentkezes Beszúrás:
void beszuras1() {
	char jelentkezoNev[50];
	char jelentkezoEmail[30];
	int rendezvenyID; //
	printf("\njelentkezes:\n");
	printf("Add meg a jelentkezo nevet:");
	scanf("%[^\n]s", &jelentkezoNev);
	getchar();
	printf("\nAdd meg a jelentkezoEmail cimet:\n");
	scanf("%[^\n]s", &jelentkezoEmail);
	getchar();
	printf("\nAdd meg a rendezveny azonositojat:\n");

	int vanE;
    
        scanf("%d", &rendezvenyID);
        getchar();
        vanE = kereses1(rendezvenyID);
        if(vanE==0){
            printf("\nNINCS ilyen azonositoval rendelkezo rendezveny! A program leall!\n");
            exit(1);
        }else{

	addjelentkezes(jelentkezoNev, jelentkezoEmail, rendezvenyID);
	printf("\nA bejegyzes beszurva!\n");
	}
}

//Rendezveny beszúrása
void beszuras2(){
    char rendezvenyNev[50];
	int rendezvenyID;

	printf("\nRendezveny letrehozasa:\n");

	printf("Adja meg a rendezveny nevet:");
	scanf("%[^\n]s", &rendezvenyNev);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");
	scanf("%d", &rendezvenyID);
	getchar();

    addrendezveny(rendezvenyNev, rendezvenyID);

	printf("\nA rendezveny inditasa megtortent!\n");
	listazas2();
}

//Jelentkezes Módosítás:
void modositas() {
	char jelentkezoNev[50];
    char jelentkezoEmail[30];
	int rendezvenyID;

	struct jelentkezes *seged = jelentkezesek;
	struct jelentkezes *elozo = 0;
    if(seged!=0){
	listazas1();
	int melyik;
	printf("\nAdja meg hanyadik sorszamu bejegyzest szeretne modositani! ");
	scanf("%d", &melyik);
	getchar();
    int i = 1;
    while (seged != 0 && i != melyik) {
        elozo = seged;
        seged = seged->next;
        i++;
        }
        if (seged == 0)
            {printf("Nincs ilyen sorszamu jelentkezes a listaban!\n");
                exit(1);
        }

    else{
	printf("\nAdja meg az uj adatokat!\n");

	printf("Jelentkezo neve: ");
	scanf("%[^\n]s", &jelentkezoNev);
	getchar();

	printf("\nJelentkezo email cime: ");
	scanf("%[^\n]s", &jelentkezoEmail);
	getchar();

	printf("\nRendezveny azonosito: ");

    int vanE;
    do{
        scanf("%d", &rendezvenyID);
        getchar();

        vanE = kereses1(rendezvenyID);
            if(vanE==0){
            printf("\nNincs ilyen azonositoju rendezveny!\n");
            printf("a program leall");
            exit(2);
            }
        }while(vanE!=1);

	int i = 1;
			while (seged != 0 && i != melyik) {
				seged = seged->next;
				i++;
			}
			if (seged == 0){
                printf("Nincs ilyen sorszamu elem a listaban!\n");
                printf("a program leall");
			}
			else {
				for (i = 0; i < 50; i++) seged->jelentkezoNev[i] = jelentkezoNev[i];
				for (i = 0; i < 30; i++) seged->jelentkezoEmail[i] = jelentkezoEmail[i];
				seged->rendezvenyID = rendezvenyID;
			}
	
	printf("\nA bejegyzes modositva!\n");
    }
    }
}

//Jelentkezes Törlés:
void torles1() {
	int melyik;
	listazas1();
	struct jelentkezes *seged = jelentkezesek;
	struct jelentkezes *elozo = 0;
	if(seged!=0){
	printf("\nAdja meg hanyadik sorszamu vendeget szeretne torolni!");
	scanf("%d", &melyik);
	getchar();
	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0){ printf("Nincs ilyen sorszamu vendeg a listaban!\n");
			exit(1);
			}
			else {
				if (elozo == 0) jelentkezesek = seged->next;
				else elozo->next = seged->next;
				free(seged);
			}

	printf("\nA jelentkezes torolve!\n");
	}
}

//jelentkezes torlese rendezveny torlese miatt
void torles3(int id){
	struct jelentkezes *seged = jelentkezesek;
	struct jelentkezes *elozo = 0; 
	
	while(seged !=0 && seged->ID !=id)
	{
		elozo = seged;
		seged = seged->next;
	}

	if(seged ==0){
		printf("Nincs ilyen ID-val jelentkezes.");
	}else{
		if(elozo==0){
			jelentkezesek = seged->next;
		}else{
			elozo->next = seged->next;	
		}
		free(seged);
		printf("\nA jelentkezes torolve!\n");
	}
}


//rendezveny torlese
void torles2(){
int melyik;
	listazas2();
	struct rendezveny *seged = rendezvenyek;
	struct rendezveny *elozo = 0;
	
	printf("\nAdja meg hanyadik bejegyzest szeretned torolni!");
	scanf("%d", &melyik);
	getchar();

	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0){
				printf("Nincs ilyen sorszamu rendezveny a listaban!\n");
                		exit(1);
          		}
			else{
				if (elozo == 0) rendezvenyek = seged->next;
				else elozo->next = seged->next;
				
				struct jelentkezes *seged2 = jelentkezesek;
				if(seged2 != 0){
					while(seged2 !=0){
						if(seged2->rendezvenyID == melyik){
							torles3(seged2->ID);
							
						}
						seged2 = seged2->next;
					}

				}

				free(seged);
			
	printf("\nA rendezveny torolve!\n");
	}
	
}

int jelentkezesekDBSzama(int keresettID)
{
	struct rendezveny *seged= rendezvenyek;
	bool ures = true;
	int db = 0;
	
	if(seged != 0)
	{
		struct jelentkezes *seged2 = jelentkezesek;
		if(seged2 != 0)
		{
			while (seged2 != 0) 
			{
				if(seged2->rendezvenyID == keresettID)
				{
					db++;
					ures = false;
				}
				seged2 = seged2->next;
			}
			if(ures == true)
			{
				printf("Nincs jelentkezes az adott esemenyhez.");
			}
		}
		else
		{
			printf("Nincs jelentkezes a nyilvantartasban.");
		}
		
	}
	else
	{
		//
	}
	
	return db;
}


//rendezveny lebonoylitasa

static bool gyerekARendezvenyHelyen = 0;
static key_t key;

static void handleSignal(int jel)
{
	if (jel == SIGTERM)
	{
		gyerekARendezvenyHelyen = 1;
	}
}



void lebonyolit(){
	listazas2();
	int melyik; //melyik rendezveny kerul lebonyolitasra
	printf("Melyik rendezvenyt szeretne lebonyolitani?");
	scanf("%d", &melyik);
	getchar();
	
	if(kereses1(melyik) == false){
		printf("Nincs ilyen rendezveny!\n");
		return;
	}
	struct jelentkezes *seged = jelentkezesek;
	int jelentkezesekSzama=jelentkezesekDBSzama(melyik);
	
	struct uzenet {
		long tipus;
		char szoveg[1000];
	};

	int msgqueue = msgget(key, 0600 | IPC_CREAT);
	if (msgqueue < 0 ) {
		perror("hiba");
		exit(EXIT_FAILURE);
	}
	
	int pipe1[2]; 
	pid_t cpid;
	
	if(pipe(pipe1) == -1){
		perror("Hiba a cso megnyitasakor.");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGTERM, handleSignal);
	cpid = fork();
	if(cpid <0){
		perror("Hiba a Fork megnyitasakor.");
		exit(EXIT_FAILURE);
	}
	else if(cpid>0){
		pause();

		if (gyerekARendezvenyHelyen != true){
			printf("\n megerkezett a helyszinre. \n");
		}
		close(pipe1[0]);
		struct jelentkezes *seged = jelentkezesek;
		struct jelentkezes regisztraltJelentkezesek[jelentkezesekSzama];
		int index=0;
		if(seged !=0){
			while (seged !=0){
				if(seged->rendezvenyID == melyik){
					regisztraltJelentkezesek[index] = *seged;
					index++;
				}
				seged = seged->next;
			 }
		}
		int i;
		for (i =0;i<jelentkezesekSzama;++i){
			char tomb[80];
			strcpy(tomb, regisztraltJelentkezesek[i].jelentkezoNev);
			write(pipe1[1], &tomb, sizeof(tomb));
		}
		
		close(pipe1[1]);
		struct uzenet m;
		msgrcv(msgqueue, &m, 1000, 1, 0);
		
		printf("\n Rendezvenyre megerkezett vendegek: ", m.szoveg);
		
		int status;
		waitpid(cpid, &status, 0);
		
		printf("\n A rendezveny befejezodott.\n");
		
	}
	else{
		sleep(2);
		kill(getpid(),SIGTERM);
		close(pipe1[1]);
		sleep(2);
		printf("\n Rendezveny elkezdodott. \n Vendegek:\n");
		srand(time(NULL));
		int i;
		for(i=0;i<jelentkezesekSzama;i++){
			char tomb[80];
			read(pipe1[0], &tomb, sizeof(tomb));
			if(rand()/ (RAND_MAX+1.0e-12) <0.9){
				printf(" eljott a rendezvenyre \n", tomb);
			}
			else{
				printf(" nem jott el a rendezvenyre \n",tomb);
				struct uzenet vendeg = {2, ""};
				sprintf(vendeg.szoveg,"%s",tomb);
				msgsnd(msgqueue, &vendeg, strlen(vendeg.szoveg), 0);
			}
		}
		
		sleep(2);
		printf("\n Rendezveny vege.\n");
		struct uzenet nezet = {1, ""};
		sprintf(nezet.szoveg, "%d/10", rand() %11);
		msgsnd(msgqueue, &nezet, strlen(nezet.szoveg),0);
		
		close(pipe1[0]);
		exit(EXIT_SUCCESS);

	}
	
}

//Adatok betöltése
void betoltes() {
    if( access("rendezvenyek.bin", 0 ) != -1 ) {
        FILE *rendezvenyekfile;
        rendezvenyekfile = fopen("rendezvenyek.bin", "r");
            if (rendezvenyekfile != NULL) {
                struct rendezveny seged;
                while (fread(&seged, sizeof(seged), 1, rendezvenyekfile)) {
                    addrendezveny(seged.rendezvenyNev, seged.ID);

                }
            }
        fclose(rendezvenyekfile);

        if( access("jelentkezesek.bin", 0 ) != -1 ) {
            FILE *jelentkezesekfile;
            jelentkezesekfile = fopen("jelentkezesek.bin", "r");
                if (jelentkezesekfile != NULL) {
                    struct jelentkezes seged2;
                    while (fread(&seged2, sizeof(seged2), 1, jelentkezesekfile)) {
                        addjelentkezes(seged2.jelentkezoNev, seged2.jelentkezoEmail, seged2.rendezvenyID);
                    }
                }
            fclose(jelentkezesekfile);
        }
    }
}

//Adatok kimentése
void mentes() {
	FILE *rendezvenyekfile;
	rendezvenyekfile = fopen("rendezvenyek.bin", "w");
	struct rendezveny *seged = rendezvenyek;
	while (seged != 0) {
		fwrite(seged, sizeof(*seged), 1, rendezvenyekfile);
		seged = seged->next;
	}
	fclose(rendezvenyekfile);

	FILE *jelentkezesekfile;
	jelentkezesekfile = fopen("jelentkezesek.bin", "w");
	struct jelentkezes *seged2 = jelentkezesek;
	while (seged2 != 0) {
		fwrite(seged2, sizeof(*seged2), 1, jelentkezesekfile);
		seged2 = seged2->next;
	}
	fclose(jelentkezesekfile);
}


int main() {
	betoltes();
	printf("Ez a program a jelentkezesek es rendezvenyek adatait tartja nyilvan!\n");
	int vege;
	do {
		printf("\n\n---------------\nMenu\n\n");
		printf("1. jelentkezesek Listazasa\n");
		printf("2. Jelentkezes Beszuras\n");
		printf("3. jelentkezes Modositas\n");
		printf("4. jelentkezes Torles\n");
		printf("5. rendezvenyek Listazasa\n");
		printf("6. rendezvenyek Beszuras\n");
		printf("7. rendezvenyek Torles\n");
		printf("8. rendezvenyek lebonyolitasa\n");
		printf("9. Kilepes");
		printf("\n\n---------------\n");
		scanf("%d", &vege);
		getchar();
		switch (vege) {
			case 1:
				//Listazas
				listazas1();
			break;
			case 2:
				//Beszuras
				beszuras1();
			break;
			case 3:
				//Modositas
				modositas();
			break;
			case 4:
				//Torles
				torles1();
			break;
			case 5:
			    //rendezveny listazasa
			    listazas2();
			break;
			case 6:
			    //rendezveny beszurasa
			    beszuras2();
			break;
			case 7:
			    //rendezveny torlese
			    torles2();
			break;
			case 8:
			    //rendezveny lebonyolitasa
			    lebonyolit();
			break;

			case 9:
            		break;
			default:
				printf("\n\nNem letezo menupont!\n");
				printf("\nA program leall az adatok nem kerulnek mentesre!");
				exit(1);
			break;
		}
	} while (vege != 9);
	mentes();
	return 0;
}
