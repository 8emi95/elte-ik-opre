#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct {
   char data[256];
   char id[64];
   char first[64];
   char last[64];
   char email[64];    
} Registered;

typedef struct {
    char data[256];
    char id[128];
    char name[128];
} Event;

typedef struct {
    int successRate;
    Registered noShow[256];
} Report;

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
    int j, f, sublen, datalen, start;

    while(fgets(line, sizeof(line),file)){
         memcpy(&to[i].data, line, sizeof(line));
         f = 0;
         sublen = 0;
         start = 0;
         datalen = strlen(to[i].data);
         for(j = 0; j < datalen; ++j){
             if(to[i].data[j] != ','){
                 ++sublen;
             }else{
                 switch(f){
                     case 0 :
                        memcpy(&to[i].id, &to[i].data[start], sublen);
                        to[i].id[sublen] = '\0';
                        f = 1;
                        break;
                    case 1 :
                        memcpy(&to[i].first, &to[i].data[start], sublen);
                        to[i].first[sublen] = '\0';
                        f = 2;
                        break;
                    case 2 :
                        memcpy(&to[i].last, &to[i].data[start], sublen);
                        to[i].last[sublen] = '\0';
                        f = 3;
                        break;
                    case 3 :
                        memcpy(&to[i].email, &to[i].data[start], sublen);
                        to[i].email[sublen] = '\0';
                        break;
                 }
                 start = j+1;
                 sublen = 0;
             }
         }

         ++i;
    }

    fclose(file);
    return i;
}

int getEventsContent(Event* to){
    FILE *file = fopen("events.txt","r");
    int i = 0;
    int j, f, sublen, datalen, start;
    char line[256];

    while(fgets(line, sizeof(line),file)){
        memcpy(&to[i].data, line, sizeof(line));
        f = 0;
        sublen = 0;
        start = 0;
        datalen = strlen(to[i].data);
         for(j = 0; j < datalen; ++j){
            if(to[i].data[j] == '.'){
                 break;
            }
             if(to[i].data[j] != ','){
                 ++sublen;
             }else{
                 switch(f){
                     case 0 :
                        memcpy(&to[i].id, &to[i].data[start], sublen);
                        to[i].id[sublen] = '\0';
                        f = 1;
                        break;
                    case 1 :
                        memcpy(&to[i].name, &to[i].data[start], sublen);
                        to[i].name[sublen] = '\0';
                        break;
                    default:
                        printf("This is the default");
                        break;
                 }
                 start = j+1;
                 sublen = 0;
             }
         }
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
    strcpy(line, id);
    strcat(line, ",");
    strcat(line, first);
    strcat(line,",");
    strcat(line, last);
    strcat(line, ",");
    strcat(line, mail);
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

    strcpy(line, id);
    strcat(line,",");
    strcat(line, name);
    strcat(line, ",");
    //strcat(line,"\n");

    fprintf(file, line);
    fflush(file);
    ++count;

    printf("A Rendezveny Felveve a Listara\n");
    fclose(file);
    return count;
}

int getRegisteredForEvent(char* eventId, Registered* from, Registered* to, int count){
    int j =0;
    int i;
    for( i = 0; i < count; ++i){
        if(strcmp(eventId, from[i].id) == 0){
            memcpy(to[j].data, from[i].data, sizeof to[j].data);
            memcpy(to[j].id, from[i].id, sizeof to[j].id);
            memcpy(to[j].first, from[i].first, sizeof to[j].first);
            memcpy(to[j].last, from[i].last, sizeof to[j].last);
            memcpy(to[j].email, from[i].email, sizeof to[j].email);
            ++j;
        }
    }
    return j;
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

void exitHandler(){
    printf("Rendezveny Vege\n");
}

void childEventHandler(){
    printf("Partner Elerte a Rendezveny Helyszinet\n");
}

int main(){

    Registered registered[256];
    Event events[256];

    pid_t child;
    int ptocpipe[2];
    int ctoppipe[2];
    char buffer[128];

    int eventCounter = 0;
    int numberOfAttendees;

    static const Report reset;

    int numberOfPeople = getRegisteredContent(registered);
    int numberOfEvents = getEventsContent(events);

    int response;

    do{
        printf("\n*****King of Stands*****\n");
        printf("1. Jelentkezes\n");
        printf("2. Jelentkezesek Listazasa\n");
        printf("3. Uj Rendezveny Felvétele\n");
        printf("4. Rendezveny Adatainak Torlese\n");
        printf("5. Vendeg Adatainak Modositasa\n");
        printf("6. Vendeg Torlese\n");
        printf("7. Rendezveny Inditasa\n");

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
        }else if(response == 7){

            if(eventCounter >= numberOfEvents){
                printf("Nincs Tobb Rendezveny!");
                continue;
            }

             struct sigaction sigact;
            sigact.sa_handler=childEventHandler;
            sigemptyset(&sigact.sa_mask);
            sigact.sa_flags=0;
            sigaction(SIGUSR1,&sigact,NULL);

            signal(SIGTERM,exitHandler);

            //Event
            if (pipe(ptocpipe) == -1 || pipe(ctoppipe) == -1) {
                perror("Pipe");
                exit(EXIT_FAILURE);
            }    

            child = fork();
            if(child < 0){
                perror("Fork");
                exit(EXIT_FAILURE);
            }
            else if(child > 0){ //parent
                int status;
                Report report = reset;
                Registered attendees[256];
                numberOfAttendees = getRegisteredForEvent(events[eventCounter].id, registered, attendees, numberOfPeople);
                printf("Helyszin: ");
                scanf("%s", &buffer);
                
                close(ctoppipe[1]);
                close(ptocpipe[0]);

                write(ptocpipe[1], buffer, sizeof(buffer));
                write(ptocpipe[1], &numberOfAttendees, sizeof(numberOfAttendees));
                write(ptocpipe[1], attendees, sizeof(attendees));

                fflush(NULL);
                close(ptocpipe[1]);
                
                read(ctoppipe[0], &report, sizeof(report));
                close(ctoppipe[0]);
                wait(&status);
                printf("\n*****Az Esemeny Beszamoloja*****\n");
                printf("Siker: %d%%\n", report.successRate);
                printf("Nem jelentek meg:\n");
                int i;
                for(i = 0; i< 256; ++i){
                    if(strcmp(report.noShow[i].data, "") !=0){
                        printf("%s\n", report.noShow[i].data);
                    }else{
                        break;
                    }
                }

            }else{ //child
                Registered attendees[256];
                Report report = reset;
                int j = 0;
                char destination[128];
                srand(time(NULL));
                int success = (rand()%100) + 1;

                close(ptocpipe[1]);
                close(ctoppipe[0]);

                read(ptocpipe[0], buffer, sizeof(buffer));
                strcpy(destination, buffer);

                sleep(2);

                raise(SIGUSR1);
                
                read(ptocpipe[0], &numberOfAttendees, sizeof(numberOfAttendees));
                read(ptocpipe[0], attendees, sizeof(attendees));
                printf("\n*****Resztvevok*****\n");
                printRegisteredContent(attendees, numberOfAttendees);

                int r;
                int i;
                for(i = 0; i < numberOfAttendees; ++i){
                    if(r=(rand()%100) > 90){
                        memcpy(&report.noShow[j], &attendees[i], sizeof(report.noShow[j]));
                        ++j;
                    }
                }
                report.successRate = success;

                write(ctoppipe[1], &report, sizeof(report));
                fflush(NULL);

                close(ptocpipe[0]);
                close(ctoppipe[1]);
                kill(getppid(),SIGTERM);
                break;
            }
            eventCounter++;
            if(eventCounter < numberOfEvents && strcmp(events[eventCounter].data, ".") !=0){
                    printf("A Kovetkezo Esemeny: %s", events[eventCounter].data);
                }else{
                    printf("Jelenleg Nincs Tobb Esemeny.");
                }

        }else if(response == 0){
            return 0;;
        }
        else{
            printf("Nincs Ilyen Menupont\n");
        }
    }while (1);

    return 0;

}