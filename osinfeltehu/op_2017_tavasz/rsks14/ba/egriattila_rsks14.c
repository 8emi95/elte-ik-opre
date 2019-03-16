
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#define ADATOK_FILE "adatok.data"
#define MAX_SIZE 100

typedef struct kerdesvalasszal {
	char 				kerdes[MAX_SIZE];
	char				valasz1[MAX_SIZE];
	char				valasz2[MAX_SIZE];
	char				valasz3[MAX_SIZE];
	char				valasz4[MAX_SIZE];
	int					valasz_db;
	char                ido[MAX_SIZE];
};

int hanyadik = 0;
struct kerdesvalasszal tomb[MAX_SIZE];

int clean_stdin()
{
	while (getchar() != '\n');
	return 1;
}

void menupontok();
void menu();

int main(int argc, char* argv[]) {
	menu(argv);
}

void menupontok() {
	printf("Menu:\n");
	printf(" (1) Kerdes hozzaadasa\n");
	printf(" (2) Kerdesek listazasa\n");
	printf(" (3) Kerdes modositasa\n");
	printf(" (4) Kerdesek torlese\n");
	printf(" (5) Kerdoiv veglegesitese es fajlba mentese\n");
	printf(" (6) KILEPES\n");
}

void menu(char* argv[])
{
	int choice;
	do {
		menupontok();
		scanf("%d", &choice);
		clean_stdin();
		switch (choice)
		{
		case 1:
		    printf("Kerdes hozzaadasa... \n");
			kerdeshozzaad();
			break;
		case 2:
			printf("Kerdesek listazasa... \n");
			kerdeseklistazasa();
			break;
		case 3:
			printf("Kerdes modositasa... \n");
			kerdesmodositas();
			break;
		case 4:
			printf("Kerdes torlese... \n");
            kerdestorlese();
			break;
		case 5:
			printf("Kerdoiv veglegesitese, fajlban eltarolasa... \n");
			kerdoivveglegesites();
			break;
		case 6:
			printf("Kilepes..\n");
			break;
		default:
			printf("Nincs ilyen menüpont!");
			break;
		}
	} while (choice != 6);
}

void kerdeshozzaad(){
    struct kerdesvalasszal kv;
    int kerdesszam=0;
    int szamlalo = 0;
    char ibetu[MAX_SIZE];
    int ellen = 1;
    printf("\nAdja meg a kerdest: ");
	fgets(kv.kerdes , MAX_SIZE, stdin);


    printf("\nAdja meg hany valaszt szeretne adni(2-4 kozott): ");
    scanf("%d", &kerdesszam);
    if(4<kerdesszam){
        printf("\n Valasza nagyobb mint 4, igy 4 valaszt kell megadni");
        kerdesszam=4;
    }else if(kerdesszam<2){
        printf("\n Valasza kissebb mint 2, igy 2 valaszt kell megadni");
        kerdesszam=2;
    }
    kv.valasz_db=kerdesszam;
    clean_stdin();
	while(szamlalo < kerdesszam ){
        switch(szamlalo){
            case 0:
                printf("\n(a)Adja meg a valaszt: ");
                fgets(kv.valasz1,MAX_SIZE,stdin);
                break;
            case 1:
                printf("\(b)Adja meg a valaszt: ");
                fgets(kv.valasz2,MAX_SIZE,stdin);
                break;
            case 2:
                printf("\(c)Adja meg a valaszt: ");
                fgets(kv.valasz3,MAX_SIZE,stdin);
                break;
            case 3:
                printf("\(d)Adja meg a valaszt: ");
                fgets(kv.valasz4,MAX_SIZE,stdin);
                break;
        }
        szamlalo++;
	}

    //idokeres
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(kv.ido, 100, "%Y-%m-%d %H:%M:%S", tm_info);
    //printf(kv.ido);
	tomb[hanyadik] = kv;
	hanyadik++;
}

void kerdeseklistazasa(){
    int i = 0;
    while(i<hanyadik){
        printf("%d", i+1);
        printf(". kerdes es valaszai: \n");
        printf(tomb[i].kerdes);
        printf(tomb[i].valasz1);
        printf(tomb[i].valasz2);
        if(3 == tomb[i].valasz_db){
            printf(tomb[i].valasz3);
        }else if(4 == tomb[i].valasz_db){
            printf(tomb[i].valasz3);
            printf(tomb[i].valasz4);
        }
        printf("letrehozva: ");
        printf(tomb[i].ido);
        printf("\n______ \n");
        i++;
    }
}

void kerdesmodositas(){
    printf("\nHanyadik kerdest szeretne modositani?");
    char hnydk[MAX_SIZE];
    int valasz;
    fgets(hnydk , MAX_SIZE, stdin);
    int int_hnydk=atoi(hnydk);
    //hanyadik --> hany darab kerdes van
    if(int_hnydk < 1 || int_hnydk > hanyadik){
        printf("Rossz kerdes szamot adott meg.\n");
    }else{
        //struct kerdesvalasszal modositando = tomb[int_hnydk-1];
        printf("%d", int_hnydk);
        printf(". kerdest valasztotta: \n");
        printf("\nOpciok: (1) kerdes mod. (2) 1. valasz mod. (3) 2. valasz mod (4) 3. valasz mod (5) 4. valasz mod:\n");
        scanf("%d", &valasz);
		clean_stdin();
		char erremodosit[MAX_SIZE];
		printf("\nAdja meg a modositott szoveget: \n");
		fgets(erremodosit,MAX_SIZE,stdin);
		switch (valasz)
		{
		case 1:
            strcpy(tomb[int_hnydk-1].kerdes,erremodosit);
		    break;
        case 2:
            strcpy(tomb[int_hnydk-1].valasz1,erremodosit);
		    break;
        case 3:
            strcpy(tomb[int_hnydk-1].valasz2,erremodosit);
		    break;
        case 4:
            strcpy(tomb[int_hnydk-1].valasz3,erremodosit);
		    break;
        case 5:
            strcpy(tomb[int_hnydk-1].valasz4,erremodosit);
		    break;
        default:
            printf("Nincs ilyen menupont... visszalep fomenu... \n");
            break;
		}
    }
}

void kerdestorlese(){
    printf("\nHanyadik kerdest szeretne torolni?");
    char hnydk[MAX_SIZE];
    int valasz;
    fgets(hnydk , MAX_SIZE, stdin);
    int int_hnydk=atoi(hnydk);
    //hanyadik --> hany darab kerdes van
    if(int_hnydk < 1 || int_hnydk > hanyadik){
        printf("Rossz kerdes szamot adott meg.");
    }else{
        strcpy(tomb[int_hnydk-1].kerdes,tomb[hanyadik-1].kerdes);
        strcpy(tomb[int_hnydk-1].valasz1,tomb[hanyadik-1].valasz1);
        strcpy(tomb[int_hnydk-1].valasz2,tomb[hanyadik-1].valasz2);
        strcpy(tomb[int_hnydk-1].valasz3,tomb[hanyadik-1].valasz3);
        strcpy(tomb[int_hnydk-1].valasz4,tomb[hanyadik-1].valasz4);
    }
    hanyadik--;
}

void kerdoivveglegesites(){
    int tarold = -1;
    printf("\nHanyadik kerdeseket szeretne eltarolni? valasszon az alabbiak kozul: \n");
    int i = 0;
    while(i<hanyadik){
        printf("%d", i+1);
        printf(". kerdes: \n");
        printf(tomb[i].kerdes);
        i++;
    }
        //write mod, azaz veglegesitettet modositani nem lehet, ujat lehet csak letrehozni.
        FILE *f = fopen("foci.txt", "w");
		fprintf(f, "-----\Magyar Labdajatekok Szervezete (MLSZ) nemzeti konzultacio-----\n");
		fprintf(f, "-------------------------------------------------------------------");

	while(tarold != 0){
        printf("\nAdja meg a kerdes sorszamat amit tarolni szeretne, kilepes(0): \n");
        scanf("%d", &tarold);
		clean_stdin();

        if(0 < tomb[tarold-1].valasz_db){
           fprintf(f, "\n\nKerdes: ");
            fprintf(f, tomb[tarold-1].kerdes);
            fprintf(f, "\n1. valasz: ");
            fprintf(f, tomb[tarold-1].valasz1);
            fprintf(f, "2. valasz: ");
            fprintf(f, tomb[tarold-1].valasz2);
            if(3 == tomb[tarold-1].valasz_db){
            fprintf(f, "3. valasz: ");
            fprintf(f, tomb[tarold-1].valasz3);
            }else if(4 == tomb[tarold-1].valasz_db){
                fprintf(f, "3. valasz: ");
                fprintf(f, tomb[tarold-1].valasz3);
                fprintf(f, "4. valasz: ");
                fprintf(f, tomb[tarold-1].valasz4);
            }
            fprintf(f, "kerdes felvetelenek datuma: ");
            fprintf(f, tomb[tarold-1].ido);
        }


	}
	printf("\n...eltarolta a program a valasztott kerdeseit es valaszait\n");
	fclose(f);

}
