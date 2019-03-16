#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//felhasznált segedanyag: http://www.tankonyvtar.hu/hu/tartalom/tamop412A/2010-0011_szamalap2/adatok.html

struct jelentkezes {
    char nev[50];
    char email[50];
	int rendezvenyAzonosito;
	time_t jelentkezesIdo; //Mon Oct 31 21:14:02 2016

	struct jelentkezes *next;
};

struct rendezveny {
	int rendezvenyAzonosito;
	char rendezvenyNev[50];
	struct rendezveny *next;
};

struct jelentkezes *jelentkezesek = 0;
struct rendezveny *rendezvenyek = 0;


int is_the_same(int a, int b) {
	 if (a!= b) return 0;
	return 1;
}

int kereses(int rendezvenyAzonosito) {

    int isExist =0;
	struct rendezveny *seged = rendezvenyek;

	if (seged != 0) {
			do {
				if (is_the_same(rendezvenyAzonosito, seged->rendezvenyAzonosito) == 1) {
					isExist = 1;
				}
				seged = seged->next;
			} while (seged != 0);
	}
	return isExist;
}


void createJelentkezes(char nev[], char email[], int rendezvenyAzonosito ) {
	int i;

	time_t ltime;

	struct jelentkezes *elem;
	elem = malloc( sizeof(struct jelentkezes) );

	elem->next = jelentkezesek;
	for (i = 0; i < 50; i++) elem->nev[i] = nev[i];
	for (i = 0; i < 50; i++) elem->email[i] = email[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;
	elem->jelentkezesIdo = time(&ltime);


	jelentkezesek = elem;

}

void vendeg_jelentkeztetes(){
    char nev[50];
    char email[50];
	int rendezvenyAzonosito;

	printf("\nVendeg jelentkeztetese:\n");

	printf("Adja meg a vendeg nevet:");
	scanf("%[^\n]s", &nev);  //szóköz beolvasás
	getchar();

	printf("\nAdja meg a vendeg email cimet:\n");
	scanf("%[^\n]s", &email);
	getchar();

	printf("\nAdja meg a rendezveny azonositojat:\n");


    int isExist;
    do{
        scanf("%d", &rendezvenyAzonosito);
        getchar();

        isExist = kereses(rendezvenyAzonosito);

        if(isExist==0){
            printf("\nNINCS ilyen azonositoju rendezveny!\n");
            exit(1);

        }

    }while(isExist!=1);


	createJelentkezes(nev, email, rendezvenyAzonosito);

	printf("\nA vendeg jelentkeztetese megtortent!\n");

}

void jelentkezettek_listazasa(){
    printf("\nA rendszerben talalhato jelentkezesek:\n\n");
	struct jelentkezes *seged = jelentkezesek;

	if (seged != 0) {
	int i = 1;
			do {
				printf("\n%d. ", i);

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
			if (seged == 0) printf("Nincs ilyen sorszamu vendeg a listaban!\n");
			else {
				if (elozo == 0) jelentkezesek = seged->next;
				else elozo->next = seged->next;
				free(seged);
			}

	printf("\nA bejegyzes torolve!\n");
	}
}

void vendeg_adat_modositas(){


	char nev[30];
    char email[30];
	int rendezvenyAzonosito;

	struct jelentkezes *seged = jelentkezesek;
    if(seged!=0){
	jelentkezettek_listazasa();
	int melyik;

	printf("\nAdja meg hanyadik sorszamu bejegyzest szeretne modositani! ");
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

        isExist = kereses(rendezvenyAzonosito);
        if(isExist==0){
            printf("\nNincs ilyen azonositoju rendezveny!\n");
            printf("a program leall");
            exit(2);
        }
    }while(isExist!=1);


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
				for (i = 0; i < 30; i++) seged->nev[i] = nev[i];
				for (i = 0; i < 30; i++) seged->email[i] = email[i];
				seged->rendezvenyAzonosito = rendezvenyAzonosito;
			}

	printf("\nA bejegyzes modositva!\n");
    }
    printf("jelentkezok listaja ures!\n");
}

void createRendezveny(char rendezvenyNev[], int rendezvenyAzonosito ) {
	int i;

	struct rendezveny *elem;
	elem = malloc( sizeof(struct rendezveny) );

	elem->next = rendezvenyek;
	for (i = 0; i < 30; i++) elem->rendezvenyNev[i] = rendezvenyNev[i];
	elem->rendezvenyAzonosito = rendezvenyAzonosito;

	rendezvenyek = elem;
}

void rendezvenyekListazasa(){
    printf("\nA rendszerben talalhato rendezvenyek:\n\n");
	struct rendezveny *seged = rendezvenyek;

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
	else {
            printf("Ures a rendezveny lista! \n");
	}
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
	struct rendezveny *seged = rendezvenyek;
	struct rendezveny *elozo = 0;
	if(seged!=0){
	printf("\nAdja meg hanyadik bejegyzest szeretned torolni!");
	scanf("%d", &melyik);
	getchar();

	int i = 1;
			while (seged != 0 && i != melyik) {
				elozo = seged;
				seged = seged->next;
				i++;
			}
			if (seged == 0)
            {printf("Nincs ilyen sorszamu rendezveny a listaban!\n");
                exit(1);
            }
			else {
				if (elozo == 0) rendezvenyek = seged->next;
				else elozo->next = seged->next;
				free(seged);
			}
	printf("\nA rendezveny torolve!\n");
	}

}

int main()
{

	int vege;
	do {
		printf("\nNyomja meg a megfelelo szamot, a kivant funkcio hasznalatahoz:\n");
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
				//Jelentkezes
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

    return 0;
}

