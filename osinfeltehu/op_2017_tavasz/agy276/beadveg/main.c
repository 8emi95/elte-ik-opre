#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAXLEN 4096
#define MAXVALASZ 4

void menu(int);
void lista(int);
void hozzaad_kerdes();
void modosit(int);
void torol();
int dont();
void fajl_ment(char *, char *);
void szerk_kerdes(FILE*, int);
void kerdes_veglegesites();
int dontes;
int utolso_kerdes = 0;
time_t raw_date;
int main()
{
    lista(0);
    MAINMENU:menu(1);

    switch(dont()){
        case(1):
            HOZZAD:menu(2);
            switch(dont()){
                case(1):
                    hozzaad_kerdes();
                    goto HOZZAD;
                    break;
                case(2):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.");
                    goto HOZZAD;
                    break;
            }
            break;
        case(2):
                lista(1);
               SZERKESZT: menu(3);
               switch(dont()){
                case(1):
                    printf("Adja meg melyik kerdest szerkesztene.\n");
                    dontes = dont();
                    modosit(dontes);
                    goto SZERKESZT;
                    break;
                case(2):{
                    printf("Adja meg melyik kerdest torolne.\n");
                    dontes = dont();
                    printf("Torles balbla.\n");
                    torol(dontes);
                    goto SZERKESZT;
                    break;
                }
                case(3):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.\n");
                    goto SZERKESZT;
                    break;
               }
            break;
        case(3):
            lista(1);
            LISTA: menu(4);
            switch(dont()){
                case(1):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.\n");
                    goto LISTA;
                    break;
            }
            break;
        case(4):
            kerdes_veglegesites();
            goto MAINMENU;
            break;
        case(5):
            exit(0);
        default:
            printf("Hibas menu.");
            goto MAINMENU;
    }
    FILE *tisztitas;
    FILE *tisztitas2;
    tisztitas = fopen("asd.txt", "w");
    tisztitas2 = fopen("temp.txt", "w");
    fclose(tisztitas);
    fclose(tisztitas2);
    return 0;
}

int dont(){

    char *dontesChar = malloc(sizeof(char));
    dontesChar = fgets(dontesChar, sizeof(dontesChar), stdin);
    dontes = (int) *dontesChar - '0';
    return dontes;
}

void menu(int dont){
    switch(dont){
        case(1):
            printf("1. Uj kerdessor keszitese. \n");
            printf("2. Kerdessor szerkesztese. \n");
            printf("3. Kerdessor megtekintese. \n");
            printf("4. Kerdessor veglegesitese. \n");
            printf("5. Kilepes. \n");
            break;
        case(2):
            printf("1. Uj kerdes megadasa.\n");
            printf("2. Vissza.\n");
            break;
        case(3):
            printf("1. Kerdes szerkesztese.\n");
            printf("2. Kerdes torlese.\n");
            printf("3. Vissza.\n");
            break;
        case(4):
            printf("1. Vissza.\n");
            break;
        default:
            printf("Nincs ilyen menu.\n");
            break;
    }
}

void hozzaad_kerdes(){
    FILE *file;
    file=fopen("asd.txt", "a+");

    int kovet = 0;
    char kerdes[MAXLEN];
    int len = 0;
    char valasz[MAXLEN];
    int valaszok_szama = 0;
    int valasz_id = 1;
    int kerdes_id = utolso_kerdes + 1;

    raw_date = time(NULL);
    struct tm tm = *localtime(&raw_date);
    //printf("Date now: %d.%d.%d.\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    printf("Adja meg a kerdes.\n");
    fgets(kerdes, sizeof(kerdes), stdin);
    fprintf(file, "Q:%d. %s", kerdes_id, kerdes);
    fprintf(file, "Date: %d.%d.%d. - %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Adjon meg legalabb ketto, maximum negy vlaszt.\n");
        VALASZ:if(kerdes != NULL){
            if(valasz_id < 5){
            printf("Adjon meg egy valaszt.\n");
            fgets(valasz, sizeof(valasz), stdin);
            fprintf(file, "\t%d. %s", valasz_id, valasz);
            valasz_id++;
            }else{
                printf("Nem adhat meg tobb mint 4 valaszt.\n");
            }
        }
    if(valasz_id < 3){
        goto VALASZ;
    }else{
        printf("1. Uj valasz megadasa.\n");
        printf("2. Mentes.\n");
        dontes = dont();
        if(dontes == 1){
            goto VALASZ;
        }
    }
    kerdes_id++;
    fclose(file);
    lista(0);
}

void lista(int print){
    FILE *file;
    file = fopen("asd.txt", "r");
    char sor[MAXLEN];
    printf("Kerdesek\n");
    while(fgets(sor, MAXLEN, file) != NULL){
        if(*sor != '\n'){
            if(sor[0] == 'Q'){
                utolso_kerdes = sor[2] - '0';
            }
            if(print == 1){
                printf("%s", sor);
            }
        }
    }
    fclose(file);
}

void modosit(int id){
    FILE *file;
    FILE *temp;
    file = fopen("asd.txt", "a+");
    temp = fopen("temp.txt", "w");
    char sor[MAXLEN];
    int atirando = 0;
    int segit = 0;
    int talalt = 0;
    int locale_id = 0;
    while(fgets(sor, MAXLEN, file) != NULL){
        if(sor[0] == 'Q'){
            if(sor[3] == '.'){
                locale_id = sor[2] - '0';
            }else{
                //a = (sor[2] + sor[3]);
                locale_id = (sor[2] + sor[3]) - '0';
            }
            if(locale_id == id){
                atirando = 1;
                talalt = 1;
            }else{
                atirando = 0;
            }
        }
        if(atirando == 0){
            fprintf(temp,"%s",sor);
        }else{
            if(segit == 0){
                szerk_kerdes(temp, id);
                segit = 1;
            }
        }
    }

    fclose(file);
    fclose(temp);
    if(talalt == 1){
        fajl_ment("temp.txt", "asd.txt");
    }
}

void szerk_kerdes(FILE *file, int kerdesID){
    char kerdes[MAXLEN];
    char valasz[MAXLEN];
    raw_date = time(NULL);
    struct tm tm = *localtime(&raw_date);
    printf("Adja meg a kerdes.\n");
    fgets(kerdes, sizeof(kerdes), stdin);
    fprintf(file, "Q:%d. %s", kerdesID, kerdes);
    fprintf(file, "Date: %d.%d.%d. - %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    int valasz_id = 1;

    printf("Adjon meg legalabb ketto, maximum negy vlaszt.\n");
        VALASZ:if(kerdes != NULL){
            if(valasz_id < 5){
            printf("Adjon meg egy valaszt.\n");
            fgets(valasz, sizeof(valasz), stdin);
            fprintf(file, "\t%d. %s", valasz_id, valasz);
            valasz_id++;
            }else{
                printf("Nem adhat meg tobb mint 4 valaszt.\n");
            }
        }
    if(valasz_id < 3){
        goto VALASZ;
    }else{
        printf("1. Uj valasz megadasa.\n");
        printf("2. Mentes.\n");
        dontes = dont();
        if(dontes == 1){
            goto VALASZ;
        }
    }
}

void torol(int id){
    FILE *file;
    FILE *temp;
    file = fopen("asd.txt", "a+");
    temp = fopen("temp.txt", "w");
    char sor[MAXLEN];
    int torlendo = 0;
    int talalt = 0;
    int locale_id = 0;
    while(fgets(sor, MAXLEN, file) != NULL){
        if(sor[0] == 'Q'){
            if(sor[3] == '.'){
                locale_id = sor[2] - '0';
            }else{
                locale_id = (sor[2] + sor[3]) - '0';
            }
            if(locale_id == id){
                torlendo = 1;
                talalt = 1;
            }else{
                torlendo = 0;
            }
        }
        if(torlendo == 0){
            fprintf(temp,"%s",sor);
        }
    }
    fclose(file);
    fclose(temp);
    if(talalt == 1){
        fajl_ment("temp.txt", "asd.txt");
    }
}

void fajl_ment(char *from, char *to){
    FILE *file1;
    FILE *file2;
    file1 = fopen(from, "r");
    file2 = fopen(to, "w");
    char sor[MAXLEN];

    while(fgets(sor, MAXLEN, file1) != NULL){
        fprintf(file2, "%s", sor);
    }

    fclose(file1);
    fclose(file2);
}

void kerdes_veglegesites(){
    printf("Kerdesek veglegesitese.\n");
    lista(1);
    printf("Vesszovel elvalasztva, melyik kerdesek legyenek elmentve.\n");
    int ch;
    int i,j=0;
    char *dontesek = malloc(MAXLEN);
    dontesek = fgets(dontesek, MAXLEN, stdin);
    int size = strlen(dontesek)/2;
    int szamok[size];
    printf("Size: %d\n", strlen(dontesek));
    for(i = 0; i < strlen(dontesek)-1; i++){
        if(dontesek[i] != ','){
            szamok[j] = dontesek[i] - '0';
            j++;
        }
    }
    FILE *atmeneti;
    FILE *vegleges;
    char sor[MAXLEN];
    int menteni = 0;
    int locale_id = 0;
    atmeneti = fopen("asd.txt", "r");
    vegleges = fopen("vegleges.txt", "w");
    while(fgets(sor, MAXLEN, atmeneti) != NULL){
        if(sor[0] == 'Q'){
            if(sor[3] == '.'){
                locale_id = sor[2] - '0';
            }else{
                locale_id = (sor[2] + sor[3]) - '0';
            }
            for(i = 0; i < sizeof(szamok)/sizeof(szamok[0]); i++){
                if(locale_id == szamok[i]){
                    menteni = 1;
                    break;
                }else{
                    menteni = 0;
                }
            }
        }
        if(menteni == 1){
            fprintf(vegleges, "%s", sor);
        }
    }
    fclose(atmeneti);
    fclose(vegleges);
    free(dontesek);
}

