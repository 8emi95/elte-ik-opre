#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>

// C99 or C11 mode required

void menu();
void kerdesUj();
void kerdesModosit();
void kerdesTorol();
void kerdesListaz();
void veglegesit();

int main() {
    int valasztas;
    menu();
}

void menu() {
    int opcio;
    printf("\nUdvozoljuk, mit szeretne tenni?\n");
    printf("1:Uj kerdes\n");
    printf("2:Kerdes modositasa\n");
    printf("3:Kerdes torlese\n");
    printf("4:Kerdesek listazasa\n");
    printf("5:Veglegesites\n");
    printf("0:Kilepes\n");
    scanf(" %i", &opcio);
    switch(opcio) {
    case 1:
        kerdesUj();
        break;
    case 2:
        kerdesModosit();
        break;
    case 3:
        kerdesTorol();
        break;
    case 4:
        kerdesListaz();
        break;
    case 5:
        veglegesit();
        break;
    case 0:
        exit(1);
        }
}

void kerdesUj() {
    FILE *input;
    input = fopen("bemenet.txt", "a");
    char kerdes[150];
    int valszam = 0;
    char valaszok[4][150];
    int i = 0;
    while(fgets(kerdes,150,stdin)&&i!=1) {
        printf("Kerdes: ");
        i++;
    }
    while(valszam < 2 || valszam > 4) {
        printf("Valaszok szama(2-4): ");
        scanf("%i", &valszam);
    }

    printf("Minden valasz utan nyomjon meg egy entert!\n");
    fprintf(input, "%s%i\n", &kerdes, valszam);
    for(int i = 0; i < valszam; i++) {
        int q = 0;
        while(fgets(valaszok[i], 150, stdin) && q < 1) {
            printf("Valasz %i: ", i + 1);
            q += 1;
        }
        fprintf(input, "%s", valaszok[i]);
    }
    time_t ido;
    struct tm * idoadat;
    time(&ido);
    idoadat = localtime(&ido);
    fprintf(input, "%s", asctime(idoadat));
    fclose(input);
    menu();
}
void kerdesModosit(){
    int s = 0;
    while(s < 1) {
        printf("Az alabbi kerdesekbol valaszthat\n");
        FILE *input = fopen("bemenet.txt", "r");
        char sor[150];
        while(fgets(sor, sizeof(sor), input)) {
            printf("kerdes: %s", sor);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                printf("valasz %i.:%s", i + 1, sor);
            }
        printf("\n");
        fgets(sor, sizeof(sor), input);
        s++;
    }
    fclose(input);
    }

    char kerdesBol[150];
    char kerdesBe[150];
    int i = 0;
    FILE *input = fopen("bemenet.txt", "r");
    FILE *uj = fopen("valtozott", "w");
    if(input == NULL) {
        printf("Nem jo a bemeneti fajl\n");
        exit(1);
    }
    while(fgets(kerdesBol,150,stdin)&&i!=1) {
        printf("Regi kerdes: ");
        i++;
    }
    i = 0;
    while(fgets(kerdesBe,150,stdin)&&i!=1) {
        printf("Uj kerdes: ");
        i++;
    }
    char sor[150];
    while(fgets(sor, sizeof(sor), input)) {
        if(strcmp(sor, kerdesBol) == 0) {
            fprintf( uj, "%s", kerdesBe);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam + 1; i++) {
                fgets(sor, sizeof(sor), input);
            }
            int valszam = 0;
            while(valszam < 2 || valszam > 4) {
                printf("Valaszok szama(2-4): ");
                scanf("%i", &valszam);
            }
            fprintf(uj, "%i\n", valszam);
            char valaszok[valszam][150];
            printf("Minden valasz utan nyomjon meg egy entert!\n");
            for(int i = 0; i < valszam; i++) {
                int q = 0;
                while(fgets(valaszok[i], 150, stdin) && q < 1) {
                    printf("Valasz %i: ", i + 1);
                    q += 1;
                }
                fprintf(uj, "%s", valaszok[i]);
            }
            time_t ido;
            struct tm * idoadat;
            time(&ido);
            idoadat = localtime(&ido);
            fprintf(uj, "%s", asctime(idoadat));
        }
        else {
            fprintf(uj,"%s",sor);
        }
    }
    fclose(input);
    fclose(uj);
    remove("bemenet.txt");
    rename("valtozott", "bemenet.txt");


    menu();
}
void kerdesTorol(){
    int s = 0;
    while(s < 1) {
        printf("Az alabbi kerdesekbol valaszthat\n");
        FILE *input = fopen("bemenet.txt", "r");
        char sor[150];
        while(fgets(sor, sizeof(sor), input)) {
            printf("kerdes: %s", sor);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                printf("valasz %i.:%s", i + 1, sor);
            }
        printf("\n");
        fgets(sor, sizeof(sor), input);
        s++;
    }
    fclose(input);
    }
    char kerdesBol[150];
    int i = 0;
    FILE *input = fopen("bemenet.txt", "r");
    FILE *uj = fopen("csere", "w");
    if(input == NULL) {
        printf("Nem jo a bemeneti fajl\n");
        exit(1);
    }
    while(fgets(kerdesBol,150,stdin)&&i!=1) {
        printf("Torlendo kerdes: ");
        i++;
    }
    char sor[150];
    while(fgets(sor, sizeof(sor), input)) {
        if(strcmp(sor, kerdesBol) == 0) {
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam + 1; i++) {
                fgets(sor, sizeof(sor), input);
            }
        }
        else {
            fprintf(uj,"%s",sor);
        }
    }
    fclose(input);
    fclose(uj);
    remove("bemenet.txt");
    rename("csere", "bemenet.txt");
    menu();
}
void kerdesListaz(){
    FILE *input = fopen("bemenet.txt", "r");
    char sor[150];
    while(fgets(sor, sizeof(sor), input)) {
        printf("kerdes: %s", sor);
        fgets(sor, sizeof(sor), input);
        int szam = sor[0] - '0';
        for(int i = 0; i < szam; i++) {
            fgets(sor, sizeof(sor), input);
            printf("valasz %i.:%s", i + 1, sor);
        }
        printf("\n");
        fgets(sor, sizeof(sor), input);
    }
    fclose(input);
    menu();
}
void veglegesit() {
    int s = 0;
    while(s < 1) {
        printf("Az alabbi kerdesekbol valaszthat\n");
        FILE *input = fopen("bemenet.txt", "r");
        char sor[150];
        while(fgets(sor, sizeof(sor), input)) {
            printf("kerdes: %s", sor);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                printf("valasz %i.:%s", i + 1, sor);
            }
        printf("\n");
        fgets(sor, sizeof(sor), input);
        s++;
    }
    fclose(input);
    }
    char kerdesBol[150];
    int i = 0;
    FILE *input = fopen("bemenet.txt", "r");
    FILE *uj = fopen("vegleges.txt", "a");
    if(input == NULL) {
        printf("Nem jo a bemeneti fajl\n");
        exit(1);
    }
    while(fgets(kerdesBol,150,stdin)&&i!=1) {
        printf("Veglegesitendo kerdes: ");
        i++;
    }
    char sor[150];
    while(fgets(sor, sizeof(sor), input)) {
        if(strcmp(sor, kerdesBol) == 0) {
            fprintf( uj, "%s", sor);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                fprintf( uj, "%s", sor);
            }
        }
    }
    fclose(input);
    fclose(uj);
    menu();
}
