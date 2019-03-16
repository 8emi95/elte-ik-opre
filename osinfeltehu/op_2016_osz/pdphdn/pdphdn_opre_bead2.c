#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>
#include <string.h>
#include  <sys/types.h>
#include <sys/wait.h>

struct guest {
    char name[30];
    char email[50];
    time_t date;
    int eventNumber;
};

struct event {
    int eventNumber;
    char eventName[30];
    int guestCount;
    struct guest guests[20];
};
int eventCount;
struct event eventPointer[10];

void clearBuffer(char *array) {
    int i = 0;
    for (i = 0; i < 256; i++) {
        array[i] = (char) 0;
    }
}

int writeEventsToFile() {
    FILE *fileToWrite;
    fileToWrite = fopen("events.bin", "wb");
    if (!fileToWrite) {
        printf("nem lehet fajlt letrehozni");
        return 1;
    }

    fwrite(&eventCount, sizeof (int), sizeof (int), fileToWrite);
    fwrite(eventPointer, sizeof (struct event), sizeof (struct event)*eventCount, fileToWrite);

    fclose(fileToWrite);
    return 0;
}

int readEventsFromFile() {
    FILE *fileToRead;
    fileToRead = fopen("events.bin", "rb");
    if (!fileToRead) {
        printf("Nem sikerult megnyitni a fajlt!");
        return 1;
    }

    fread(&eventCount, sizeof (int), sizeof (int), fileToRead);
    fread(eventPointer, sizeof (struct event), sizeof (struct event)*eventCount, fileToRead);

    fclose(fileToRead);
    return 0;
}

void showEvents() {
    int i;
    for (i = 0; i < eventCount; i++) {
        printf("%d %s \n", eventPointer[i].eventNumber, eventPointer[i].eventName);
    }
}

void handleExit() {
    writeEventsToFile();
    exit(0);
}

void deleteGuest() {

    printf("\n Add meg a torolni kivant vendeg email cimet \n");
    char buffer[256];
    read(1, buffer, 256);
    char emailtodelete[256];
    strcpy(emailtodelete, buffer);

    int i;
    for (i = 0; i < eventCount; i++) {
        int j;
        for (j = 0; j < eventPointer[i].guestCount; j++) {
            if (strcmp(eventPointer[i].guests[j].email, emailtodelete) == 0) {
                int k;
                for (k = j; k < eventPointer[i].guestCount - 1; k++) {
                    eventPointer[i].guests[k] = eventPointer[i].guests[k + 1];
                }
                printf("toroltunk");
                eventPointer[i].guestCount--;
            }
        }
    }

}

void registerNewGuest() {
    char buffer[256];
    printf("\n Add meg az esemeny sorszamat \n");
    read(1, buffer, 256);
    int i;
    int number = -1;
    ;
    for (i = 0; i < eventCount; ++i) {
        if (eventPointer[i].eventNumber == atoi(buffer)) {
            number = i;
            break;
        }
    }
    if (number == -1) {
        printf("Nem talalhato ilyen sorszamu esemeny\n");
        exit(1);
    }
    printf("\n Add meg a vendeg nevet \n");
    clearBuffer(buffer);

    read(1, buffer, 256);
    strcpy(eventPointer[number].guests[eventPointer[number].guestCount].name, buffer);

    printf("\n Add meg a vendeg email cimet \n");
    clearBuffer(buffer);

    read(1, buffer, 256);
    strcpy(eventPointer[number].guests[eventPointer[number].guestCount].email, buffer);
    eventPointer[number].guestCount++;

    eventPointer[number].guests[eventPointer[number].guestCount].date = time(0);
    printf("A vendeg sorszama: %d", eventPointer[number].guestCount);
}

void showRegisteredGuests() {
    char buffer[256];
    printf("\n Add meg az esemeny sorszamat \n");
    read(1, buffer, 256);
    int i;
    for (i = 0; i < eventCount; i++) {
        if (eventPointer[i].eventNumber == atoi(buffer)) {
            int j;
            for (j = 0; j < eventPointer[i].guestCount; j++) {
                printf("A vendeg neve: %semail cime: %s\n", eventPointer[i].guests[j].name, eventPointer[i].guests[j].email);
            }
        }
    }
}

void modifyGuest() {
    printf("\n Add meg a vendeg nevet, amit modositani szeretnel:\n");
    char buffer[256];
    read(1, buffer, 256);
    char nametofind[256];
    strcpy(nametofind, buffer);

    printf("\n Add meg a vendeg email cimet,amit modositani szeretnel \n");
    clearBuffer(buffer);
    read(1, buffer, 256);
    char email[256];
    strcpy(email, buffer);

    printf("\n Add meg a vendeg nevet, amire modositani szeretnel:\n");
    clearBuffer(buffer);
    read(1, buffer, 256);
    char nametoeditto[256];
    strcpy(nametoeditto, buffer);

    printf("\n Add meg a vendeg email cimet,amire modositani szeretnel \n");
    clearBuffer(buffer);
    read(1, buffer, 256);
    char emailtoeditto[256];
    strcpy(emailtoeditto, buffer);

    int i;
    for (i = 0; i < eventCount; i++) {
        int j;
        for (j = 0; j < eventPointer[i].guestCount; j++) {
            if (strcmp(eventPointer[i].guests[j].name, nametofind) == 0 && strcmp(eventPointer[i].guests[j].email, email) == 0) {
                strcpy(eventPointer[i].guests[j].name, nametoeditto);
                strcpy(eventPointer[i].guests[j].email, emailtoeditto);
            }
        }
    }
}

void createNewEvent() {
    printf("\n Add meg a rendezveny nevet:\n");
    char buffer[256];
    read(1, buffer, 256);
    strcpy(eventPointer[eventCount].eventName, buffer);
    printf("\n Add meg az esemeny sorszamat \n");
    clearBuffer(buffer);
    read(1, buffer, 256);
    eventPointer[eventCount].eventNumber = atoi(buffer);
    eventPointer[eventCount++].guestCount = 0;
    //showEvents();	

}

void deleteEvent() {
    printf("\n Add meg a rendezveny sorszamat, amelyiket torolni szeretned:\n");
    char buffer[256];
    read(1, buffer, 256);
    int i;
    int number = -1;
    for (i = 0; i < eventCount; i++) {
        if (eventPointer[i].eventNumber == atoi(buffer)) {
            number = i;
            int j;
            for (j = i; j < eventCount - 1; j++) {
                eventPointer[j] = eventPointer[j + 1];
            }
            eventCount--;
        }
    }
    if (number == -1) {
        printf("\n Nem talalhato a megadott sorszamu esemeny");
        exit(1);
    }
}

void showMenuMessage() {
    printf("\n 1-Vendeg jelentkezese \n 2-Jelentkezettek listaja \n 3-Vendeg adatainak modositasa\n 4-Vendeg torlese \n 5-Uj rendezveny letrehozasa \n 6-Rendezveny osszes adatanak torlese \n 7-Rendezveny lebonyolitasa \n");
}

void handler(int signumber){
  printf("Varom az adatokat\n");
}

void executeEvent() {
    int pipefd[2];
    pid_t pid;
    char m[256];
    struct guest guests[20];
    struct guest didntshowup[20];
    int didntshowupcount;
    signal(SIGTERM,handler);
    srand(time(NULL));
    printf("Add meg az esemeny sorszamat, amit el szeretnel inditani: ");    
    char buffer[256];
    read(1, buffer, 256);
    int i = 0;
    for (i = 0; i < eventCount; i++) {
        if (eventPointer[i].eventNumber == atoi(buffer)) {
            clearBuffer(buffer);
            printf("\n Add meg az esemeny helyszinet:");
            fflush( stdout );
            read(1, buffer, 256);
            if (pipe(pipefd) == -1) {
                perror("Hiba a pipe nyitaskor!");
                exit(EXIT_FAILURE);
            }
            pid = fork();
            if (pid == -1) {
                perror("Fork hiba");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) { // child process                
                read(pipefd[0], m, sizeof (m)); 
                sleep(5);
                printf("Itt vagyok: %s", m);
                kill(getppid(),SIGTERM); 
                printf("\n");
                sleep(1);
                read(pipefd[0],guests, eventPointer[i].guestCount*sizeof(struct guest));
                int j;
                for(j = 0;j<eventPointer[i].guestCount;j++){
                    printf("vendeg: %s",guests[j].name);
                    int generated = (rand() % 100 + 1);                    
                    if(generated>90){
                        didntshowup[didntshowupcount] = guests[j];
                        didntshowupcount++;
                    }
                }
                write(pipefd[1],&didntshowupcount,sizeof(didntshowupcount));
                write(pipefd[1],&didntshowup,didntshowupcount*sizeof(struct guest));
                kill(getppid(),SIGTERM); 
                
                close(pipefd[0]); // finally we close the used read end
            } else { // szulo process 
             
                write(pipefd[1], buffer, 256);
                
                fflush(NULL); // flushes all write buffers (not necessary)
                pause();
                
                write(pipefd[1], &eventPointer[i].guests, eventPointer[i].guestCount*sizeof(struct guest));
                fflush(NULL); // flushes all write buffers (not necessary)
                pause();
                read(pipefd[0],didntshowup,sizeof(didntshowupcount));
                read(pipefd[0],didntshowup,didntshowupcount*sizeof(struct guest));
                int j;        
                printf("\n Akik nem erkeztek meg:\n");                
                for(j = 0;j<didntshowupcount;j++){
                    printf("%s",didntshowup[j].name);
                }                
                showMenuMessage();
            }            
        }
    }
}

void showmenu() {
    int typedChar = -1;
    showMenuMessage();
    do {
        scanf("%d", &typedChar);
        switch (typedChar) {
            case 0: handleExit();
                break;
            case 1: registerNewGuest();
                typedChar = -1;
                showMenuMessage();
                break;
            case 2: showRegisteredGuests();
                typedChar = -1;
                showMenuMessage();
                break;
            case 3: modifyGuest();
                typedChar = -1;
                showMenuMessage();
                break;
            case 4: deleteGuest();
                typedChar = -1;
                showMenuMessage();
                break;
            case 5: createNewEvent();
                typedChar = -1;
                showMenuMessage();
                break;
            case 6: deleteEvent();
                typedChar = -1;
                showMenuMessage();
                break;
            case 7: executeEvent();
                typedChar = -1;
                
                break;
            default: printf("Nem ertem, hogy mit akarsz ");
                typedChar = -1;
        }

    } while (typedChar == -1);
}

void init() {
    if (readEventsFromFile() == 1) {
        eventCount = 0;
    }
}

int main() {
    setbuf(stdout, NULL);
    init();
    showmenu();

    return 0;
}
