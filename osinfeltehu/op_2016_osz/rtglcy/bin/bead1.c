#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define InputFile "backup.txt"

#define true 1
#define false 0
typedef int bool;

struct Latogato{
    char nev[300];
    char mail[300];
    char rendezveny_azon[100];
    time_t time;
};

void init(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length);
void menu(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length);
void deinit(struct Latogato arr_latogatok[], int L_arr_length);

void Uj_latogato(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length);
void Latogatok_listazasa(struct Latogato arr_latogatok[], int L_arr_length);
void Latogato_tolese(struct Latogato arr_latogatok[], int L_arr_length);
void Latogato_modositasa(struct Latogato arr_latogatok[], int L_arr_length);
void Uj_rendezveny(char arr_rendezenyek[100][100], int* R_arr_length);
void Rendezveny_torlese(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int L_arr_length, int R_arr_length);

int main()
{
    struct Latogato arr_latogatok[500];
    char arr_rendezenyek[100][100];
    int L_arr_length = 0;
    int R_arr_length = 0;

    init(arr_latogatok, arr_rendezenyek, &L_arr_length, &R_arr_length);
    menu(arr_latogatok, arr_rendezenyek, &L_arr_length, &R_arr_length);
    deinit(arr_latogatok, L_arr_length);

    return 0;
}

void init(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length){
    FILE * fPointer;
    fPointer = fopen(InputFile, "r");

    int i = 0;
    int j = 0;
    bool Find = false;
    char x[1024];
    while (fscanf(fPointer, " %1023s", x) == 1) {
        switch(j%4){
            case 0:
                memcpy(arr_latogatok[i].nev, x, strlen(x)+1);
                j++;
                break;
            case 1:
                memcpy(arr_latogatok[i].mail, x, strlen(x)+1);
                j++;
                break;
            case 2:
                memcpy(arr_latogatok[i].rendezveny_azon, x, strlen(x)+1);
                j++;
                break;
            case 3:
                arr_latogatok[i].time = atoi(x);
                j++;  // x.nev = ctime(&x.time);
                i++;
                break;
        }
    }
    *L_arr_length = i;

    fclose(fPointer);

    for(i = 0; i < *L_arr_length; i++){
        memcpy(x, arr_latogatok[i].rendezveny_azon, strlen(arr_latogatok[i].rendezveny_azon)+1);

        for(j = 0; j < *R_arr_length; j++){
            if (!strcmp(arr_rendezenyek[j], x)){
                Find = true;
            }
        }

        if (!Find){
            memcpy(arr_rendezenyek[*R_arr_length], x, strlen(x)+1);
            *R_arr_length = *R_arr_length + 1;
        }
    }
}

void menu(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length){
    char command = ' ';
    printf("OPRE  1. beadando               Takacs Patrik (RTGLCY)\n\n");

    printf("  1.) Uj latogato\n");
    printf("  2.) Latogatok listazasa\n");
    printf("  3.) Latogato tolese\n");
    printf("  4.) Latogato modositasa\n");
    printf("  5.) Uj rendezveny\n");
    printf("  6.) Rendezveny torlese\n\n");
    printf("  0.) Exit\n");

    while(command != '0'){
        printf("\nParancs: ");
        scanf("%c", &command);
        if(command == '\n'){ scanf("%c", &command); }       // buffer

        if(command == '1'){
            Uj_latogato(arr_latogatok, arr_rendezenyek, L_arr_length, R_arr_length);
        }
        if(command == '2'){
            Latogatok_listazasa(arr_latogatok, *L_arr_length);
        }
        if(command == '3'){
            Latogato_tolese(arr_latogatok, *L_arr_length);
        }
        if(command == '4'){
            Latogato_modositasa(arr_latogatok, *L_arr_length);
        }
        if(command == '5'){
            Uj_rendezveny(arr_rendezenyek, R_arr_length);
        }
        if(command == '6'){
            Rendezveny_torlese(arr_latogatok, arr_rendezenyek, *L_arr_length, *R_arr_length);
        }
    }
}

void deinit(struct Latogato arr_latogatok[], int L_arr_length){

    FILE *f = fopen(InputFile, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
    } else {
        int i;

        for(i=0;i<L_arr_length;i++){
            if(strcmp(arr_latogatok[i].nev, "")){
                fprintf(f, "%s ", arr_latogatok[i].nev);
                fprintf(f, "%s ", arr_latogatok[i].mail);
                fprintf(f, "%s ", arr_latogatok[i].rendezveny_azon);
                fprintf(f, "%d\n", arr_latogatok[i].time);
            }
        }




        /*
        // print some text
        const char *text = "Write this to the file";
        fprintf(f, "Some text: %s\n", text);

        // print integers and floats
        int i = 1;
        float py = 3.1415927;
        fprintf(f, "Integer: %d, float: %f\n", i, py);
        */
        fclose(f);
    }
}


void Uj_latogato(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int* L_arr_length, int* R_arr_length){
    int i;
    char x1[1024];
    char x2[1024];
    char x3[1024];
    bool helyes_rendezveny_azon = false;

    printf("Nev: ");
    scanf("%s", x1);

    printf("Mail: ");
    scanf("%s", x2);

    printf("Rendezveny azonosito: ");
    scanf("%s", x3);

    for(i=0;i<*R_arr_length;i++){
        if(!strcmp(arr_rendezenyek[i], x3)){
            helyes_rendezveny_azon = true;
        }
    }

    if(helyes_rendezveny_azon){
        memcpy(arr_latogatok[*L_arr_length].nev, x1, strlen(x1)+1);
        memcpy(arr_latogatok[*L_arr_length].mail, x2, strlen(x2)+1);
        memcpy(arr_latogatok[*L_arr_length].rendezveny_azon, x3, strlen(x3)+1);

        arr_latogatok[*L_arr_length].time = time(NULL);
        *L_arr_length = *L_arr_length + 1;
    } else {
        printf("\nNemletezo rendezveny azonosito\n");
    }
}

void Latogatok_listazasa(struct Latogato arr_latogatok[], int L_arr_length){
    int i;
    //char x[1024];
    //scanf("");

    for(i=0;i<L_arr_length;i++){
        if(strcmp(arr_latogatok[i].nev, "")){
            printf("  %s, ", arr_latogatok[i].nev);
            printf("%s, ", arr_latogatok[i].mail);
            printf("%s, ", arr_latogatok[i].rendezveny_azon);
            printf("%s", ctime(&arr_latogatok[i].time));
        }
    }
}

void Latogato_tolese(struct Latogato arr_latogatok[], int L_arr_length){
    int i;
    char x[1024];
    printf("Melyik felhasznalot szeretned torolni?\n");
    scanf("%s", x);

    for(i=0;i<L_arr_length;i++){
        if(!strcmp(arr_latogatok[i].nev, x)){
            memcpy(arr_latogatok[i].nev, "", 1);
            memcpy(arr_latogatok[i].mail, "", 1);
            memcpy(arr_latogatok[i].rendezveny_azon, "", 1);
            arr_latogatok[i].time = 0;
        }
    }
}

void Latogato_modositasa(struct Latogato arr_latogatok[], int L_arr_length){
    int i;
    char x[1024];
    char UjNev[1024];
    char UjMail[1024];
    printf("Melyik felhasznalot szeretned modositani?\n");
    scanf("%s", x);

    printf("Uj felhasznalo nev: ");
    scanf("%s", UjNev);
    printf("Uj mail: ");
    scanf("%s", UjMail);

    for(i=0;i<L_arr_length;i++){
        if(!strcmp(arr_latogatok[i].nev, x)){
            memcpy(arr_latogatok[i].nev, UjNev, strlen(UjNev)+1);
            memcpy(arr_latogatok[i].mail, UjMail, strlen(UjMail)+1);
        }
    }
}

void Uj_rendezveny(char arr_rendezenyek[100][100], int* R_arr_length){
    char x[1024];
    printf("Add meg az uj rendezveny azonositot!\n");
    scanf("%s", x);

    memcpy(arr_rendezenyek[*R_arr_length], x, strlen(x)+1);
    *R_arr_length = *R_arr_length + 1;
}

void Rendezveny_torlese(struct Latogato arr_latogatok[], char arr_rendezenyek[100][100], int L_arr_length, int R_arr_length){
    int i;
    char x[1024];
    printf("Melyik rendezvenyt szeretned torolni?\n");
    scanf("%s", x);

    for(i=0;i<L_arr_length;i++){
        if(!strcmp(arr_latogatok[i].rendezveny_azon, x)){
            memcpy(arr_latogatok[i].nev, "", 1);
            memcpy(arr_latogatok[i].mail, "", 1);
            memcpy(arr_latogatok[i].rendezveny_azon, "", 1);
            arr_latogatok[i].time = 0;
        }
    }

    for(i=0;i<R_arr_length;i++){
        if(!strcmp(arr_rendezenyek[i], x)){
            memcpy(arr_rendezenyek[i], "", 1);
        }
    }
}
