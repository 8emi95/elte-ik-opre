#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

typedef struct {
   char data[256];
} Registered;

typedef struct {
    char data[256];
} Event;

void printRegisteredContent(Registered* from, int count){
    int i;
    for(i = 0; i < count; ++i){
        printf("%d. %s",i+1,from[i].data);
    }
}


void printEventContent(Event* from, int count){
    int i;
    for(i = 0; i < count; ++i){
        printf("%d. %s",i+1,from[i].data);
    }
}

void saveEvents(Event* from, int count){
    FILE *file = fopen("events.txt","w");
    int i;
    for(i = 0; i < count; ++i){
        fprintf(file, from[i].data);
    }
    fclose(file);
}

void saveRegistered(Registered* from, int count){
    FILE *file = fopen("registered.txt","w");
    int i;
    for(i = 0; i < count; ++i){
        fprintf(file, from[i].data);
    }
    fclose(file);
}

int deleteEventData(int count, Event* from){
    int response;

    printf("*****Esemenyek Listaja*****\n");
    printEventContent(from, count);
    printf("\nHanyadik Esemeny Adatait Szeretne Torolni? ");
    scanf("%d",&response);

    if(response <= count && response >= 0){
        strcpy(from[response-1].data,".\n");
        saveEvents(from, count);
    }else{
        printf("Nincs Ennyi Bejegyzes!");
    }

}

int getRegisteredContent(Registered* to){
    FILE *file = fopen("registered.txt","r");
    char line[256];
    int i = 0;

    while(fgets(line, sizeof(line),file)){
         memcpy(&to[i].data, line, sizeof(line));
         ++i;
    }

    fclose(file);
    return i;
}

int getEventsContent(Event* to){
    FILE *file = fopen("events.txt","r");
    int i = 0;
    char line[256];

    while(fgets(line, sizeof(line),file)){
        memcpy(&to[i].data, line, sizeof(line));    
        ++i;    
    }
    fclose(file);
    return i;
}

void modifyRegisteredData(Registered* to, int count){
    int response;
    char first[64];
    char last[64];
    char mail[64];
    char id[64];

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("*****Jelentkezettek Listaja*****\n");
    printRegisteredContent(to, count);
    printf("Hanyadik Vendeg Adatait Szeretne Modositani? ");
    scanf("%d",&response);
    
    if(response <= count && response >= 0){
        char line[256];

        printf("Csaladnev: ");
        scanf("%s", &first);
        printf("Keresztnev: ");
        scanf("%s", &last);
        printf("E-mail: ");
        scanf("%s", &mail);
        printf("Rendezveny Azonositoszama: ");    
        scanf("%s", &id);

        strcpy(line, first);
        strcat(line,",");
        strcat(line, last);
        strcat(line, ",");
        strcat(line, mail);
        strcat(line, ",");
        strcat(line, id);
        strcat(line, ",");
        strcat(line, asctime (timeinfo));
        //strcat(line,"\n");

        memcpy(&to[response-1].data, line, sizeof(line));
    }else{
        printf("Nincs Ilyen Bejegyzes");
    }
    saveRegistered(to, count);

}

int deleteRegistered(Registered* from, int count){
    int response;

    printf("*****Jelentkezettek Listaja*****\n");
    printRegisteredContent(from,count);
    printf("Hanyadik Jelentkezot Szeretne Torolni? ");
    scanf("%d",&response);

    if(response>=0 && response <= count){
        FILE *file = fopen("registered.txt", "w");
        int i;
        for(i = 0; i<count;++i){
            if(i != response-1){
                fprintf(file,from[i].data);
            }
        }
        fclose(file);
    }else{
        printf("Nincs Ilyen Bejegyzes!");
    }
    return --count;
}

int appendRegistrationList(char* first, char* last, char* mail, char* id, int count){
    FILE *file = fopen("registered.txt", "a");
    char line[256];

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    //strcpy(line, "\n");
    strcpy(line, first);
    strcat(line,",");
    strcat(line, last);
    strcat(line, ",");
    strcat(line, mail);
    strcat(line, ",");
    strcat(line, id);
    strcat(line, ",");
    strcat(line, asctime (timeinfo));
    //strcat(line,"\n");
    

    fprintf(file, line);
    ++count;


    printf("Sikeres Regisztracio. Maga a %d. Jelentkezo.\n\n", count);

    fclose(file);
    return count;
}

int signUp(int count){
    char first[64];
    char last[64];
    char mail[64];
    char id[64];

    printf("*****Udv a Jelentkezesi Feluleten*****\n");
    printf("Csaladnev: ");
    scanf("%s", &first);
    printf("Keresztnev: ");
    scanf("%s", &last);
    printf("E-mail: ");
    scanf("%s", &mail);
    printf("Rendezveny Azonositoszama: ");    
    scanf("%s", &id);

    count = appendRegistrationList(first, last, mail, id, count);
    return count;
}

int appendEventList(char* name, char* id,int count, Event* events){
    FILE *file = fopen("events.txt", "a");
    char line[256];

    strcpy(line, name);
    strcat(line,",");
    strcat(line, id);
    strcat(line,"\n");

    fprintf(file, line);
    fflush(file);
    ++count;

    printf("A Rendezveny Felveve a Listara\n");
    fclose(file);
    return count;
}

int newEvent(int count, Event* events){
    char name[128];
    char id[128];

    printf("****Uj Rendezveny Inditasa******\n");
    printf("Rendezveny Neve: ");
    scanf("%s", &name);
    printf("Azonosito: ");
    scanf("%s", &id);

    count = appendEventList(name,id,count, events);
    return count;
}

int main(){

    Registered registered[256];
    Event events[256];


    int numberOfPeople = getRegisteredContent(registered);
    int numberOfEvents = getEventsContent(events);

    int response;

    do{
        printf("\n*****King of Stands*****\n");
        printf("1. Jelentkezes\n");
        printf("2. Jelentkezesek Listazasa\n");
        printf("3. Uj Rendezveny Inditasa\n");
        printf("4. Rendezveny adatainak torlese\n");
        printf("5. Vendeg Adatainak Modositasa\n");
        printf("6. Vendeg Torlese\n");
        printf("0. Kilepes\n");

        scanf("%d", &response);
        if(response == 1){
            numberOfPeople = signUp(numberOfPeople);
            continue;
        }else if(response == 2){
            printf("*****Jelentkezettek Listaja*****\n");
            printRegisteredContent(registered, getRegisteredContent(registered));
            continue;
        }
        else if(response == 3){
            numberOfEvents = newEvent(numberOfEvents, events);
        }else if(response == 4){
            getEventsContent(events);
            deleteEventData(numberOfEvents,events);
            numberOfEvents = getEventsContent(events);
        }else if(response == 5){
            modifyRegisteredData(registered, numberOfPeople);
            numberOfPeople = getRegisteredContent(registered);
        }else if(response == 6){
            numberOfPeople = deleteRegistered(registered, numberOfPeople);
        }
        else if(response == 0){
            return 0;;
        }
        else{
            printf("Nincs Ilyen Menupont\n");
        }
    }while (1);

    return 0;

}