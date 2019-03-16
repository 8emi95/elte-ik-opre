#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Kerdes {
    int id;
    char kerdes[100];
    char valaszok[10][100];
    time_t felvetel_datum;
};


void menu_megjelenit();
void listaz(struct Kerdes *adatok);
void kerdes_megjelenit(struct Kerdes *adatok);
void kerdes_modosit(struct Kerdes *adatok);
void kerdes_torles(struct Kerdes *adatok, int *vegleges);
void veglegesit(struct Kerdes *adatok, int *vegleges);
void mentes(struct Kerdes *adatok, int *vegleges);
void uj_kerdes(struct Kerdes *adatok, int *vegleges);

void beolvas(FILE fajl, struct Kerdes *adatok, int *vegleges);



int main (int argc, char *argv[]) {
    
    int menu;
    FILE *fajl;
    struct Kerdes adatok[20];
    int vegleges[20];
    
    if (argc > 1) {
        fajl = fopen(argv[1], "w+"); 
    }

    menu_megjelenit();
    scanf("%d", &menu);

    do {
        switch (menu) {
        case 1:
            if (adatok[0].id != 0) {
            listaz(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 2:
            if (adatok[0].id != 0) {
            kerdes_megjelenit(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 3:
            if (adatok[0].id != 0) {
                kerdes_modosit(adatok);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 4:
            if (adatok[0].id != 0) {            
                kerdes_torles(adatok,vegleges);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }
            break;
        case 5:
            if (adatok[0].id != 0) {
                veglegesit(adatok,vegleges);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }            
            break;
        case 6:
            if (adatok[0].id != 0) {
                mentes(adatok,vegleges);
            } else {
                printf("Nincs meg beolvasott adat!\n");
            }           
            break;
        case 7:
            uj_kerdes(adatok,vegleges);
            break;
        case 9:
            menu_megjelenit();
            break;
        }
        printf("(Emlekezteto: menu megjelenites 9-es gomb)\n");
        scanf("%d", &menu);
    } while (menu!=0);

}

void menu_megjelenit() {
    printf("MLSZ Kerdoiv\n\
    A menupontok szamainak megadasaval navigalhat a funkciok kozott\n\
    1) Kerdesek listazas\n\
    2) Kerdes megjelenites\n\
    3) Kerdes modositas\n\
    4) Kerdes torles\n\
    5) Veglegesites\n\
    6) Mentes\n\
    7) Uj kerdes letrehozas\n\
    9) Menu megjelenites\n\
    0) Kilep\n");
}


void listaz(struct Kerdes *adatok) {
    int i = 0;
    while ( adatok[i].id != 0 ) {
        printf("#%d - %s - %s\n", adatok[i].id, adatok[i].kerdes, ctime(&adatok[i].felvetel_datum));
        int j = 0;
        while ( strlen(adatok[i].valaszok[j]) != 0) {
            printf("%d) %s\n",j+1,adatok[i].valaszok[j]);
            j++;
        }
        i++;
    }
}

void kerdes_megjelenit(struct Kerdes *adatok) {

}

void kerdes_modosit(struct Kerdes *adatok) {

}

void kerdes_torles(struct Kerdes *adatok, int *vegleges) {

}

void veglegesit(struct Kerdes *adatok, int *vegleges) {

}

void mentes(struct Kerdes *adatok, int *vegleges) {
printf("Adja meg a fajl nevet (valami.txt)\n");
    char fajlnev[50];
    fgets (fajlnev, 50, stdin);
}

void uj_kerdes(struct Kerdes *adatok, int *vegleges) {
    int i = 0;
    while ( i < sizeof(adatok))
    {
        if (adatok[i].id == 0) {
            break;
        } else {            
            i++;
        }
    }
    adatok[i].id=i+1;
    printf("Adja meg a kerdest!\n");
    char kerdes[100];
    fflush(stdin);
    //fgets(kerdes,100,stdin);
    scanf("%s",kerdes);
    strcpy(adatok[i].kerdes,kerdes);

    printf("Hany darab valasz lehetoseg tartozik a kerdeshez?\n");
    int valaszdb;
    scanf("%d", &valaszdb);
    int j = 0;
    while ( j < valaszdb) {
        printf("Adja meg a %d. valasz lehetoseget\n", j+1);
        char valasz[100];
        //fgets(valasz,100,stdin);
        scanf("%s",valasz);        
        strcpy(adatok[i].valaszok[j],valasz);
        j++;
    }

    adatok[i].felvetel_datum = time(0);
    
    printf("#%d - %s - %s", adatok[0].id, adatok[0].kerdes, ctime(&adatok[0].felvetel_datum));

}

void beolvas(FILE fajl, struct Kerdes *adatok, int *vegleges) {

}