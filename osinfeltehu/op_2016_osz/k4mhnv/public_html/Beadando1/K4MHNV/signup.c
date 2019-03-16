#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

int NUMBER_OF_PEOPLE;
char *regContent[256];

int getRegisteredContent(){
    NUMBER_OF_PEOPLE = 0;
    FILE *file = fopen("registered.txt", "r");
    char line[256];

    while(fgets(line, sizeof(line), file)){
        regContent[NUMBER_OF_PEOPLE] = (char*)malloc(sizeof(char) * (strlen(line) + 1 ) );
        regContent[NUMBER_OF_PEOPLE] = line;
        NUMBER_OF_PEOPLE++;
    }
    fclose(file);
}

void appendRegistrationList(char* first, char* last, char* mail, char* id){
    getRegisteredContent();
    printf("%s,%s,%s",regContent[0], regContent[0], regContent[0]);
    FILE *file = fopen("registered.txt", "a");
    char line[256];

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strcpy(line, first);
    strcat(line,",");
    strcat(line, last);
    strcat(line, ",");
    strcat(line, mail);
    strcat(line, ",");
    strcat(line, id);
    strcat(line, ",");
    strcat(line, asctime (timeinfo));
    //strcat(line, "\n");

    printf(line);
    fprintf(file, line);
    NUMBER_OF_PEOPLE++;

    printf("Sikeres regisztracio. Maga a %d. jelentkezo.\n\n", NUMBER_OF_PEOPLE);

    fclose(file);
}

void signup(){

    char first[64];
    char last[64];
    char mail[64];
    char id[64];

    printf("Udv a Jelentkezesi Feluleten\n");
    printf("Csaladnev: ");
    scanf("%s", &first);
    printf("Keresztnev: ");
    scanf("%s", &last);
    printf("E-mail: ");
    scanf("%s", &mail);
    printf("Rendezveny Azonositoszama: ");    
    scanf("%s", &id);

    appendRegistrationList(first, last, mail, id);
}