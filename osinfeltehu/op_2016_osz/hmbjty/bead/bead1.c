#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nev[40];
    char mail[40];
    char azon[40];
    int ev;
    int ho;
    int nap;
} Jelentkezo;

typedef struct {
    char nev[40];
    char azon[40];
} Rendezveny;

int jelentkezokSzama;
Jelentkezo* jelentkezok;
int rendezvenyekSzama;
Rendezveny* rendezvenyek;

void menuKiIr();
void menu();
void jelentkezes();
void jelentkezesListazasa();
void vendegekModositasa();
void vendegekTorlese();
void ujRendezveny();
void rendezvenyTorlese();
void rendezvenyListazasa();
int sorokSzama();
int jelentkezoKeres(char* nev);
int rendezvenyKeres(char* azon);
void jelentkezokBeolvas(FILE* jelentkezokFile);
void rendezvenyekBeolvas(FILE* rendezvenyekFile);
void jelentkezokKiIrasa(FILE* jelentkezokFile);
void rendezvenyekKiIrasa(FILE* rendezvenyekFile);

int main(int argc,char ** argv) {
//jelentkezok beolvasasa
    FILE* jelentkezokFile;
    if ((jelentkezokFile = fopen("jelentkezok.txt", "r")) == NULL) {
        perror("Nem sikerult a jelentkezok.txt fajl megnyitasa!\n");
        exit(1);
    }
    jelentkezokBeolvas(jelentkezokFile);
    fclose(jelentkezokFile);

//rendezvenyek beolvasasa
    FILE* rendezvenyekFile;
    if ((rendezvenyekFile = fopen("rendezvenyek.txt", "r")) == NULL) {
        perror("Nem sikerult az rendezvenyek.txt fajl megnyitasa!\n");
        exit(1);
    }
    rendezvenyekBeolvas(rendezvenyekFile);
    fclose(rendezvenyekFile);

//menu kiirasa
    menu();

//jelentkezok kiirasa fajlba
    if ((jelentkezokFile = fopen("jelentkezok.txt", "w")) == NULL) {
        perror("Nem sikerult a jelentkezok.txt fajl megnyitasa!\n");
        exit(1);
    }
    jelentkezokKiIrasa(jelentkezokFile);
    fclose(jelentkezokFile);

//rendezvenyek kiirasa fajlba
    if ((rendezvenyekFile = fopen("rendezvenyek.txt", "w")) == NULL) {
        perror("Nem sikerult az rendezvenyek.txt fajl megnyitasa!\n");
        exit(1);
    }
    rendezvenyekKiIrasa(rendezvenyekFile);
    fclose(rendezvenyekFile);

    return 0;
}

void menuKiIr() {
    printf("____________________________________\n");
    printf("|                                  |\n");
    printf("|          King of Stands          |\n");
    printf("|__________________________________|\n");
    printf("|--------Valassz a menubol!--------|\n");
    printf("|----------------------------------|\n");
    printf("|  1 - Jelentkezes                 |\n");
    printf("|  2 - Jelentkezettek listazasa    |\n");
    printf("|  3 - Vendegek modositasa         |\n");
    printf("|  4 - Vendegek torlese            |\n");
    printf("|  5 - Uj rendezveny               |\n");
    printf("|  6 - Rendezveny torlese          |\n");
    printf("|  7 - Rendezveny listazasa        |\n");
    printf("|  0 - Kilepes                     |\n");
    printf("------------------------------------\n");
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
            jelentkezes();
            break;
        case 2:
            jelentkezesListazasa();
            break;
        case 3:
            vendegekModositasa();
            break;
        case 4:
            vendegekTorlese();
            break;
        case 5:
            ujRendezveny();
            break;
        case 6:
            rendezvenyTorlese();
            break;
        case 7:
            rendezvenyListazasa();
            break;
        default:
            printf("Nincs ilyen menupont!\n");
        }
        menuKiIr();
        printf(">>>>> ");
        scanf("%i",&c);
    }
}

void jelentkezokBeolvas(FILE* jelentkezokFile) {
    jelentkezokSzama = sorokSzama(jelentkezokFile);
    jelentkezok = (Jelentkezo*) malloc(sizeof(Jelentkezo) * jelentkezokSzama);
    int i;
    for(i = 0; i < jelentkezokSzama; ++i) {
        fscanf(jelentkezokFile, "%s %s %s %i %i %i \n",
                jelentkezok[i].nev,
                jelentkezok[i].mail,
                jelentkezok[i].azon,
                &jelentkezok[i].ev,
                &jelentkezok[i].ho,
                &jelentkezok[i].nap);
    }
}


void rendezvenyekBeolvas(FILE* rendezvenyekFile) {
    rendezvenyekSzama = sorokSzama(rendezvenyekFile);
    rendezvenyek = (Rendezveny*) malloc(sizeof(Rendezveny) * rendezvenyekSzama);
    int i;
    for(i = 0; i< rendezvenyekSzama; ++i ) {
        fscanf(rendezvenyekFile, "%s %s\n",
                rendezvenyek[i].nev,
                rendezvenyek[i].azon);
  }
}

void jelentkezokKiIrasa(FILE* jelentkezokFile) {
    int i;
    for(i = 0; i < jelentkezokSzama; ++i) {
    fprintf(jelentkezokFile, "%s %s %s %i %i %i \n",
                jelentkezok[i].nev,
                jelentkezok[i].mail,
                jelentkezok[i].azon,
                jelentkezok[i].ev,
                jelentkezok[i].ho,
                jelentkezok[i].nap);
  }
}


void rendezvenyekKiIrasa(FILE* rendezvenyekFile) {
    int i;
    for(i = 0; i < rendezvenyekSzama; ++i) {
    fprintf(rendezvenyekFile, "%s %s\n",
                rendezvenyek[i].nev,
                rendezvenyek[i].azon);
  }
}

int jelentkezoKeres(char* nev) {
    int i = 0;
    while(i < jelentkezokSzama && 0 != strcmp(jelentkezok[i].nev, nev)) {
        ++i;
    }
    return i;
}

int rendezvenyKeres(char* azon) {
    int i = 0;
    while(i < rendezvenyekSzama && 0 != strcmp(rendezvenyek[i].azon, azon)) {
        ++i;
    }
    return i;
}

void jelentkezes() {

    time_t aktIdo;
    time(&aktIdo);
    struct tm helyi = *(localtime(&aktIdo));
    int ev = helyi.tm_year + 1900;
    int ho = helyi.tm_mon + 1;
    int nap = helyi.tm_mday;

    Jelentkezo* temp = (Jelentkezo*) malloc(sizeof(Jelentkezo) * (jelentkezokSzama + 1));
    int i;
    for (i = 0; i < jelentkezokSzama; ++i) {
        strcpy(temp[i].nev,jelentkezok[i].nev);
        strcpy(temp[i].mail,jelentkezok[i].mail);
        strcpy(temp[i].azon,jelentkezok[i].azon);
        temp[i].ev = jelentkezok[i].ev;
        temp[i].ho = jelentkezok[i].ho;
        temp[i].nap = jelentkezok[i].nap;
    }
    free(jelentkezok);
    jelentkezok = temp;
    char nev[40];
    printf("Adja meg az uj jelentkezo adatait: ");
    printf("\nNev: ");
    scanf("%s",nev);
    int j = jelentkezoKeres(nev);
    if(j != jelentkezokSzama) {
        printf("Mar van ilyen nevu jelentkezo!\n");
        return;
    } else {
        strcpy(jelentkezok[i].nev, nev);
        char mail[40];
        printf("Mail: ");
        scanf("%s",mail);
        strcpy(jelentkezok[i].mail, mail);
        char azon[40];
        printf("Azon: ");
        scanf("%s",azon);
        strcpy(jelentkezok[i].azon, azon);
        printf("Idopont: ");
        jelentkezok[i].ev = ev;
        jelentkezok[i].ho = ho;
        jelentkezok[i].nap =nap;
        jelentkezokSzama++;
        printf("On a %i. jelentkezo\n", jelentkezokSzama);
    }
}

void jelentkezesListazasa() {
    int i = 0;
    for(; i < jelentkezokSzama; ++i) {
        printf("%s %s %s %i. %i. %i. \n",
                jelentkezok[i].nev,
                jelentkezok[i].mail,
                jelentkezok[i].azon,
                jelentkezok[i].ev,
                jelentkezok[i].ho,
                jelentkezok[i].nap);
    }
}

void vendegekModositasa() {
    printf("Adja meg a modositani kivant jelentkezo nevet: ");
    char nev[40];
    fscanf(stdin, "\n%[^;\n]", nev);
    int i = jelentkezoKeres(nev);
    if(i == jelentkezokSzama) {
        printf("Nincs ilyen nevu jelentkezo!\n");
        return;
    }
    printf("Melyik adaton szeretne modositani?\n"
            "  1. Nev\n"
            "  2. Mail\n"
            "  3. Azon\n"
            ">>>>> ");
    int input;
    fscanf(stdin, "%i", &input);
    switch(input) {
    case 1:
        printf("Adja meg az uj nevet: ");
        fscanf(stdin, "\n%[^;\n]", nev);
        strcpy(jelentkezok[i].nev, nev);
        break;
    case 2:
        printf("Adja meg az uj mail-t: ");
        fscanf(stdin, "\n%[^;\n]", nev);
        strcpy(jelentkezok[i].mail, nev);
        break;
    case 3:
        printf("Adja meg az uj azonositot: ");
        fscanf(stdin, "\n%[^;\n]", nev);
        break;
    default:
        printf("Nem tortenet valtoztatas!\n");
        return;
    }
    if(input != 3)
        return;
    int j = rendezvenyKeres(nev);
    if(j >= rendezvenyekSzama) {
        printf("Nem letezo rendezveny!\n");
        return;
    }
    strcpy(jelentkezok[i].azon, rendezvenyek[j].azon);
}

void vendegekTorlese() {
    printf("Adja meg a torlendo jelentkezo nevet: ");
    char nev[40];
    fscanf(stdin, "\n%[^;\n]", nev);
    int i = jelentkezoKeres(nev);
    if(i == jelentkezokSzama) {
        printf("Nincs ilyen nevu jelentkezo!\n");
        return;
    }
    Jelentkezo* tmp = (Jelentkezo*) malloc(sizeof(Jelentkezo) * (jelentkezokSzama - 1));
    int j = 0;
    for(; j < jelentkezokSzama; ++j) {
        if(i == j) {
            continue;
        } else if(j < i) {
            strcpy(tmp[j].nev, jelentkezok[j].nev);
            strcpy(tmp[j].mail, jelentkezok[j].mail);
            strcpy(tmp[j].azon, jelentkezok[j].azon);
            tmp[j].ev = jelentkezok[j].ev;
            tmp[j].ho = jelentkezok[j].ho;
            tmp[j].nap = jelentkezok[j].nap;
        } else {
            strcpy(tmp[j-1].nev, jelentkezok[j].nev);
            strcpy(tmp[j-1].mail, jelentkezok[j].mail);
            strcpy(tmp[j-1].azon, jelentkezok[j].azon);
            tmp[j-1].ev = jelentkezok[j].ev;
            tmp[j-1].ho = jelentkezok[j].ho;
            tmp[j-1].nap = jelentkezok[j].nap;
        }
    }
    free(jelentkezok);
    jelentkezok = tmp;
    jelentkezokSzama--;
}

void ujRendezveny() {
    Rendezveny* temp = (Rendezveny*) malloc(sizeof(Rendezveny) * (rendezvenyekSzama + 1));
    int i;
    for (i = 0; i < rendezvenyekSzama; ++i) {
        strcpy(temp[i].nev,rendezvenyek[i].nev);
        strcpy(temp[i].azon,rendezvenyek[i].azon);
    }
    free(rendezvenyek);
    rendezvenyek = temp;
    char azon[40];
    printf("Adja meg az uj rendezveny adatait: ");
    printf("\nAzon: ");
    scanf("%s",azon);
    int j = rendezvenyKeres(azon);
    if(j != rendezvenyekSzama) {
        printf("Mar van ilyen azonositoju rendezveny!\n");
        return;
    } else {
        strcpy(rendezvenyek[i].azon, azon);
        char nev[40];
        printf("Nev: ");
        scanf("%s",nev);
        strcpy(rendezvenyek[i].nev, nev);
        rendezvenyekSzama++;
    }
}

void rendezvenyTorlese() {
    printf("Adja meg a torlendo rendezveny azonositojat: ");
    char azon[40];
    fscanf(stdin, "\n%[^;\n]", azon);
    int i = rendezvenyKeres(azon);
    if(i == rendezvenyekSzama) {
        printf("Nincs ilyen azonositoju rendezveny!\n");
        return;
    }
    Rendezveny* tmp = (Rendezveny*) malloc(sizeof(Rendezveny) * (rendezvenyekSzama - 1));
    int j = 0;
    for(; j < rendezvenyekSzama; ++j) {
        if(i == j) {
            continue;
        } else if(j < i) {
            strcpy(tmp[j].nev, rendezvenyek[j].nev);
            strcpy(tmp[j].azon, rendezvenyek[j].azon);
        } else {
            strcpy(tmp[j-1].nev, rendezvenyek[j].nev);
            strcpy(tmp[j-1].azon, rendezvenyek[j].azon);
        }
    }
    free(rendezvenyek);
    rendezvenyek = tmp;
    rendezvenyekSzama--;
}

void rendezvenyListazasa(){
    int i = 0;
    for(; i < rendezvenyekSzama; ++i) {
        printf("%s %s \n",
                rendezvenyek[i].nev,
                rendezvenyek[i].azon);
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