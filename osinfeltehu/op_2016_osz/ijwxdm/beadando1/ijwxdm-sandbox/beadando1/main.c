#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct jelentkezesElem {
    char nev[30];
    char email[30];
	int rendezvenyAzonosito;
	time_t jelentkezesIdo; //Mon Oct 31 21:14:02 2016
	int regisztracioSzam;
	struct jelentkezesElem *next;
};

struct rendezvenyElem {
	int rendezvenyAzonosito;
	char rendezvenyNev[30];
	struct rendezvenyElem *next;
};

struct jelentkezesElem *jelentkezesek = 0;
struct rendezvenyElem *rendezvenyek = 0;

//int is_the_same(char a[], char b[]) {
//	int i;
//	for (i = 0; i < 30; i++) if (a[i] != b[i]) return 0;
//	return 1;
//}

int is_the_same(int a, int b) {
	//int i;
	 if (a!= b) return 0;
	return 1;
}

int kereses(int rendezvenyAzonosito) {
//	char cim[30];
//	printf("\nAdd meg a szam cimet:\n");
//	scanf("%[^\n]s", &cim);
//	getchar();

//	printf("\nA rendszerben talalhato ilyen cimu zenefajlok:\n\n");
    int isExist =0;
	struct rendezvenyElem *seged = rendezvenyek;

	//int i;
	if (seged != 0) {
			do {
				if (is_the_same(rendezvenyAzonosito, seged->rendezvenyAzonosito) == 1) {
					printf("van ilyen");
					isExist = 1;
				}
				seged = seged->next;
			} while (seged != 0);
	}
	return isExist;
}

void betoltJelentkezes(int regisztracioSzam, char nev[], char email[], int rendezvenyAzonosito, time_t jelentkezesIdo ) {
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

void betoltRendezveny(int rendezvenyAzonosito, char rendezvenyNev[] ) {
    struct rendezvenyElem *elem;
	elem = malloc( sizeof(struct rendezvenyElem) );
	elem->next = rendezvenyek;

	elem->rendezvenyAzonosito = rendezvenyAzonosito;
	int i;
	for (i = 0; i < 30; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];

	rendezvenyek = elem;
}

//Adatok betöltése
void betoltes() {
        char fJelentkezes[]="jelentkezes.txt";
        int fJ;
        fJ = open(fJelentkezes,O_RDWR|O_CREAT,S_IRWXU);
        struct stat st;
        stat(fJelentkezes, &st);
        int sizeJelentkezes = st.st_size;
            if(sizeJelentkezes ==0){
                 printf("OK3!\n");
            }else{
                struct jelentkezesElem seged;
                while (read(fJ, &seged,sizeof(seged)) != 0) {
                   betoltJelentkezes(seged.regisztracioSzam, seged.nev, seged.email,seged.rendezvenyAzonosito,seged.jelentkezesIdo);
                }
            }
            printf("%d",sizeJelentkezes);

            close(fJ);
            /* Through O_CREAT flag the open() system call
           creates a file if it does not exist at the
           specified path.The third argument represents
           the access permissions for the file */

       //   unlink(fn); <-- Remove a directory entry

            char fRendezvenyek[]="rendezvenyek.txt";
            int fR;
            fR = open(fRendezvenyek,O_RDWR|O_CREAT,S_IRWXU);
            struct stat st2;
            stat(fRendezvenyek, &st2);
            int sizeRendezvenyek = st2.st_size;
            if(sizeRendezvenyek ==0){
                 printf("OK3!\n");
            }else{
               struct rendezvenyElem seged2;
                while (read(fR, &seged2,sizeof(seged2)) != 0) {
                  // betoltJelentkezes(seged.regisztracioSzam, seged.nev, seged.email,seged.rendezvenyAzonosito,seged.jelentkezesIdo);
                  betoltRendezveny(seged2.rendezvenyAzonosito, seged2.rendezvenyNev);
                }
            }
            printf("%d",sizeRendezvenyek);

            close(fR);
}

//Adatok kimentése
void mentes() {
    char fJelentkezes[]="jelentkezes.txt";
    int fJ;
    //fJ = open(fJelentkezes,O_RDWR|O_CREAT);
    fJ = open(fJelentkezes,O_RDWR|O_CREAT|O_TRUNC);

    struct jelentkezesElem *seged = jelentkezesek;
	while (seged != 0) {
        write(fJ, seged, sizeof(*seged));
		//fwrite(seged, sizeof(*seged), 1, jelentkezesFile);
		seged = seged->next;
	}
    close(fJ);
    /////
    char fRendezveny[]="rendezvenyek.txt";
    int fR;
    //fR = open(fRendezveny,O_RDWR|O_CREAT);
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
	int regisztracioSzam = 5;
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

	printf("Adja meg a vendeg nevet:");
	scanf("%[^\n]s", &nev);
	getchar();

	printf("\nAdja meg a vendeg email cimet:\n");
	scanf("%[^\n]s", &email);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");
//	scanf("%d", &rendezvenyAzonosito);
//	getchar();

    int isExist;
    do{
        scanf("%d", &rendezvenyAzonosito);
        getchar();

        isExist = kereses(rendezvenyAzonosito);

        if(isExist==0){
            printf("\nNINCS ilyen azonositoju rendezveny!\n");
            printf("\nAdjon meg egy valid rendezveny azonositot!\n");
//            scanf("%d", &rendezvenyAzonosito);
//            getchar();
        }
//        }else if(isExist==1){
//            printf("\nVAN ilyen azonositoju rendezveny!\n");
//        }
    }while(isExist!=1);
//	int isExist = kereses(rendezvenyAzonosito);
//
//	if(isExist==0){
//        printf("\nNINCS ilyen azonositoju rendezveny!\n");
//        printf("\nAdjon meg egy valid rendezveny azonositot!\n");
//        scanf("%d", &rendezvenyAzonosito);
//        getchar();
//	}else if(isExist==1){
//        printf("\nVAN ilyen azonositoju rendezveny!\n");
//	}

	int regisztraciosSzam = createJelentkezes(nev, email, rendezvenyAzonosito);

	printf("\nA vendeg jelentkeztetese megtortent!\n");
	printf("\nRegisztracios szam:" );
	printf("%d",regisztraciosSzam );
}

void jelentkezettek_listazasa(){
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
	else printf("Ures a jelentkezesi lista! \n");
}

void vendeg_torles(){
int melyik;
	jelentkezettek_listazasa();
	printf("\nAdja meg hanyadik bejegyzest szeretne torolni!");
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
/*	char szerzo[30];
	char cim[30];
	char mufaj[30];
	int hossz; //mp
	int melyik;
	listazas();*/

	char nev[30];
    char email[30];
	int rendezvenyAzonosito;
	//time_t jelentkezesIdo;
	//int regisztracioSzam;
	jelentkezettek_listazasa();
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

	printf("\nRendezveny azonosizto: ");
//	scanf("%d", &rendezvenyAzonosito);
//	getchar();
    int isExist;
    do{
        scanf("%d", &rendezvenyAzonosito);
        getchar();

        isExist = kereses(rendezvenyAzonosito);
        if(isExist==0){
            printf("\nNincs ilyen azonositoju rendezveny!\n");
            printf("\nAdjon meg egy valid rendezveny azonositot!\n");
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

void createRendezveny(char rendezvenyNev[], int rendezvenyAzonosito ) {
	int i;

	struct rendezvenyElem *elem;
	elem = malloc( sizeof(struct rendezvenyElem) );

	elem->next = rendezvenyek;
	for (i = 0; i < 30; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;

	rendezvenyek = elem;
}

void rendezvenyekListazasa(){
    printf("\nA rendszerben talalhato rendezvenyek:\n\n");
	struct rendezvenyElem *seged = rendezvenyek;

	if (seged != 0) {
	int i = 1;
			do {
				printf("\n%d. ", i);
				printf(" Rendezveny azonosito: %d", seged->rendezvenyAzonosito);
				printf(" Nev: %s", seged->rendezvenyNev);
				i++;
				seged = seged->next;
			} while (seged != 0);
	}
	else printf("Ures a rendezveny lista! \n");
}

void rendezveny_inditas(){
    char rendezvenyNev[30];
	int rendezvenyAzonosito;

	printf("\nRendezveny Inditasa:\n");

	printf("Adja meg a rendezveny nevet:");
	scanf("%[^\n]s", &rendezvenyNev);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");
	scanf("%d", &rendezvenyAzonosito);
	getchar();

    createRendezveny(rendezvenyNev, rendezvenyAzonosito);

	printf("\nA rendezveny inditasa megtortent!\n");
	rendezvenyekListazasa();
}

void rendezveny_torles(){
int melyik;
	rendezvenyekListazasa();
	printf("\nAdja meg hanyadik bejegyzest szeretned torolni!");
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
				if (elozo == 0) rendezvenyek = seged->next;
				else elozo->next = seged->next;
				free(seged);
			}
	printf("\nA rendezveny torolve!\n");
}

int main()
{
    betoltes();
    printf("\n-----------------------------------\n");
    printf("King of Stands\n");
    printf("Elektronikus Latogatoi Nyilvantartas\n");
	int vege;
	do {
		printf("\n\n-----------------------------------\nMenu\n\n");
		printf("1. Vendeg Jelentkeztetes\n");
		printf("2. Jelentkezettek Listazasa\n");
		printf("3. Vendeg Adatainak Modositasa\n");
		printf("4. Vendeg Torlese\n");
		printf("5. Rendezveny Inditasa\n");
		printf("6. Rendezveny Torlese\n");
		printf("7. Kilepes");
		printf("\n\n-----------------------------------\n");
		scanf("%d", &vege);
		getchar();
		switch (vege) {
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
				//Rendezveny Inditasa
                rendezveny_inditas();
			break;
			case 6:
			    //Rendezveny Torlese
				rendezveny_torles();
			break;
			case 7:
            break;
			default:
				printf("\n\nNem letezo menupont!\n");
			break;
		}
	} while (vege != 7);
	mentes();
    return 0;
}
