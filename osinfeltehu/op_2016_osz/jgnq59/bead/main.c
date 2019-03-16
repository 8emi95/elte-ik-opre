#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Réti Marcell
 * JGNQ59
 * 3-as csoport
*/

struct Szemely {
    char nev[50];
    char email[40];
    char datum[11];
    int sorszam;
    int rendezv;
};

void UjVendeg(struct Szemely** szemelyek, int *mennyiseg) {
    struct Szemely uj;
    uj.sorszam = *mennyiseg+1;

    printf("Nev:\n");
    scanf("%s", &uj.nev);

    printf("Email:\n");
    scanf("%s", uj.email);

    printf("Rendezvenyazonosito:\n");
    scanf("%d", &uj.rendezv);

    time_t tt = time(0);
    struct tm t = *localtime(&time);
    strftime(uj.datum, 10, "%Y.%m.%d", &t);
    struct Szemely* bovitett = malloc((*mennyiseg+1)*202*sizeof(int));
    memcpy(bovitett, *szemelyek, (*mennyiseg)*202*sizeof(int));
    memcpy(*mennyiseg+bovitett, &uj, 202*sizeof(int));
    *mennyiseg = *mennyiseg+1;
    *szemelyek = bovitett;
    FILE *file = fopen("egy.txt", "w");
    fprintf(file, "%d\n", mennyiseg);
    int i;
    for (i = 0; i < mennyiseg; ++i) {
        if (bovitett[i].sorszam != 0) {
            fprintf(file, "%s %s %s %d %d\n", bovitett[i].nev, bovitett[i].email, bovitett[i].datum, bovitett[i].sorszam, bovitett[i].rendezv);
        }
    }
    fclose(file);
}

void VendegekListaja(struct Szemely* szemelyek, int mennyiseg) {
    int i;
    for (i = 0; i < mennyiseg; ++i) {
        if (szemelyek[i].sorszam != -1) {
            printf("%s, %s, Azonosito:%d, Rendezvenyszam:%d\n",szemelyek[i].nev,szemelyek[i].email,szemelyek[i].sorszam,szemelyek[i].rendezv);
        }
    }
}

void VendegAdatModositas(struct Szemely* szemelyek, int mennyiseg) {
    int id;
    printf("Modositashoz adjon meg szemelyi sorszamot!");
    scanf("%d",id);
    int i;
    int l = 0;
    for (i = 0; i < mennyiseg && l == 0; ++i) {
        if (szemelyek[i].sorszam == id) {
            l = i;
        }
    }
    if (l != 0) {
        printf("Nev:\n");
        scanf("%s", szemelyek[l].nev);
        printf("Email:\n");
        scanf("%s", szemelyek[l].email);
        printf("Rendezveny:\n");
        scanf("%d", szemelyek[l].rendezv);
        FILE *file = fopen("egy.txt", "w");
        fprintf(file, "%d\n", mennyiseg);
        int i;
        for (i = 0; i < mennyiseg; ++i) {
            if (szemelyek[i].sorszam != 0) {
                fprintf(file, "%s %s %s %d %d\n", szemelyek[i].nev, szemelyek[i].email, szemelyek[i].datum, szemelyek[i].sorszam, szemelyek[i].rendezv);
            }
        }
        fclose(file);
    } else {
        printf("Nincs ilyen szemely!");
    }
}

void VendegTorles(struct Szemely* szemelyek, int mennyiseg) {
    int id;
    printf("Torleshez adjon meg szemelyi sorszamot!");
    scanf("%d",id);
    int i;
    int l = 0;
    for (i = 0; i < mennyiseg && l == 0; ++i) {
        if (szemelyek[i].sorszam == id) {
            l = i;
        }
    }
    if (l != 0) {
        szemelyek[l].sorszam = -1;
        FILE *file = fopen("egy.txt", "w");
        fprintf(file, "%d\n", mennyiseg);
        int i;
        for (i = 0; i < mennyiseg; ++i) {
            if (szemelyek[i].sorszam != 0) {
                fprintf(file, "%s %s %s %d %d\n", szemelyek[i].nev, szemelyek[i].email, szemelyek[i].datum, szemelyek[i].sorszam, szemelyek[i].rendezv);
            }
        }
    } else {
        printf("Nincs ilyen szemely!");
    }
}

void RendezvenyTorles(struct Szemely* szemelyek, int mennyiseg) {
    int ra;
    printf("Adja meg a rendezveny azonositojat!");
    scanf("%d", ra);
    int i;
    for (i = 0; i < mennyiseg; ++i) {
        if (szemelyek[i].rendezv == ra) {
            szemelyek[i].sorszam = -1;
        }
    }
    FILE *file = fopen("egy.txt", "w");
    fprintf(file, "%d\n", mennyiseg);
    for (i = 0; i < mennyiseg; ++i) {
        if (szemelyek[i].sorszam != 0) {
            fprintf(file, "%s %s %s %d %d\n", szemelyek[i].nev, szemelyek[i].email, szemelyek[i].datum, szemelyek[i].sorszam, szemelyek[i].rendezv);
        }
    }
}

int main()
{
    FILE *file = fopen("egy.txt", "rw");
    if (!file) {
        printf("Nincs ilyen fajl");
        return 1;
    }

    int mennyiseg;
    fscanf(file, "%d", &mennyiseg);
    struct Szemely* szemelyek = malloc(mennyiseg*202*sizeof(int));

    int i;
    for (i = 0; i < mennyiseg; ++i) {
        fscanf(file, "%s %s %d %d %s", &szemelyek[i].nev, &szemelyek[i].email, &szemelyek[i].sorszam, &szemelyek[i].rendezv, &szemelyek[i].datum);
    }
    fclose(file);

    //MENÜ

    int c;

    do {
        printf("Uj vendeg: 1\n");
        printf("Vendegek listazasa: 2\n");
        printf("Vendegadat modositasa: 3\n");
        printf("Vendegadat torlese: 4\n");
        printf("Rendezveny torlese: 5\n");
        printf("Kilepes: 6\n");
        scanf("%d", &c);
        switch(c) {
            case 1:
                UjVendeg(&szemelyek, &mennyiseg);
                break;
            case 2:
                VendegekListaja(szemelyek, mennyiseg);
                break;
            case 3:
                VendegAdatModositas(szemelyek, mennyiseg);
                break;
            case 4:
                VendegTorles(szemelyek, mennyiseg);
                break;
            case 5:
                RendezvenyTorles(szemelyek, mennyiseg);
                break;
            case 6:
                printf("Viszlat!");
                break;
            default:
                printf("Rossz utasitas\n");
                break;
        }
    } while (c != 6);

    return 0;
}
