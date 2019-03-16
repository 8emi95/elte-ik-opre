#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>


int NUMBER_OF_EVENTS;
char *content[256];

void getEventsContent(){
    NUMBER_OF_EVENTS = 0;
    FILE *file = fopen("events.txt", "r");
    char line[256];

    while(fgets(line, sizeof(line), file)){
        content[NUMBER_OF_EVENTS] = line;
        printf("%s",content[NUMBER_OF_EVENTS]);
        NUMBER_OF_EVENTS++;
    }
    fclose(file);
}

void appendEventList(char* name, char* id){
    FILE *file = fopen("events.txt", "a");
    char line[256];

    strcpy(line, name);
    strcat(line,",");
    strcat(line, id);
    strcat(line, "\n");

    fprintf(file, line);

    printf("Sikeresen felveve!\n\n");

    fclose(file);
}

void newEvent(){

    char name[128];
    char id[64];

    printf("Esemeny neve: ");
    scanf("%s", &name);
    printf("Azonosito: ");
    scanf("%s", &id);
    appendEventList(name, id);
}

void modifyEvent(){
    getEventsContent();
    printf("%s,%s,%s",content[0],content[1],content[2]);
    int response;
    FILE *file = fopen("events.txt", "r");
    char line[256];

    printf("*****Rendezvenyek listaja*****\n\n");

    while(fgets(line, sizeof(line), file)){
       printf("%s", line);
    }
    fclose(file);
    if(NUMBER_OF_EVENTS > 0){
    printf("\n\n");
    printf("Hanyadik Rendezveny adataid szeretne torolni? ");
    scanf("%d", &response);
    file = fopen("events.txt", "w+");
    fclose(file);
    file = fopen("events.txt", "a");
    for(int i = 0; i< response-1; ++i){
        fprintf(file,"%s\n",content[i]);
    }
    fprintf(file, ".\n");
    for(int i = response;i<NUMBER_OF_EVENTS;++i){
        fprintf(file,"%s\n", content[i]);
    }
    fclose(file);
    printf("Siker!\n\n");
    }
    else{
        printf("Nincsen Modosithato Rendezveny.");
    }

}

void adminMenu(){
    do{
        printf("*****Udv Az Admin Feluleten*****\n");
        printf("1. Uj Rendezveny Inditasa\n");
        printf("2. Rendezveny adatainak torlese\n");
        printf("3. Vendeg Adatainak Modositasa\n");
        printf("4. Vendeg Torlese\n");
        printf("5. Vissza A Fomenube\n");

        int response;
        scanf("%d", &response);
        if(response == 1){
            newEvent();
            continue;
        }else if(response == 2){
            modifyEvent();
            continue;
        }
        else if(response == 4){

        }else if(response == 5){
            break;
        }
        else{
            printf("Nincs ilyen menupont\n");
        }
    }while(1);
}