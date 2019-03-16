#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <memory.h>

struct Event
{
    int id;
    char name[100];
    char date[100];
    char location[100];
};
struct Visitor
{
    int id;
    int eventID;
    char name[100];
    char emailAddress[100];
    char regTime[1024];
};
struct EventDao
{
    FILE *file;
    char buff[2048];
}eventDao;
struct VisitorDao
{
    FILE *file;
    char buff[2048];
}visitorDao;

int readInteger()
{
    int result = -1;
    printf("Kerem a szamot: ");
    scanf("%d", &result);
    return result;
}

static struct Event events[100];
static struct Visitor visitors[100];
int registeredVisitors;

struct Event *readEvents()
{
    eventDao.file = fopen("eventsList.txt", "r");

    int eventCounter = 0;
    if(eventDao.file != 0)
    {
        while(fgets(eventDao.buff, 2048 ,eventDao.file) != NULL)
        {
            char *line = eventDao.buff;
            char *token;
            struct Event temp;
            int counter = 0;
            while((token = strsep(&line,"\t")) != NULL)
            {
                //printf("%s\n", token);
                switch (counter)
                {
                    case 0:
                        temp.id = atoi(token);
                        break;
                    case 1:
                        strcpy(temp.name, token);
                        break;
                    case 2:
                        strcpy(temp.date, token);
                        break;
                    case 3:
                        strcpy(temp.location, token);
                        break;
                }
                ++counter;
            }

            events[eventCounter] = temp;
            ++eventCounter;
        }

        fclose(eventDao.file);

        //eventNumber = eventCounter;

        struct Event *p = events;
        return p;

    }
    else
    {
        printf("Nem sikerült megnyitni a fájlt!");
    }
}
void writeEvent(struct Event *event)
{
    eventDao.file = fopen("eventsList.txt", "a");

    if(eventDao.file != 0)
    {
        fprintf(eventDao.file, "%d\t%s\t%s\t%s\n", event->id, event->name, event->date, event->location);

        fclose(eventDao.file);
    }
    else
    {
        printf("Nem sikerült megnyitni a fájlt!");
    }
}

struct Event *getEvents()
{
    struct Event *p = events;
    return p;
}
int generateEventID()
{
    srand(time(NULL));
    return rand();
}
void dropEventData()
{
    eventDao.file = fopen("eventsList.txt", "w");
    fclose(eventDao.file);
}
void resetEventArray()
{
    memset(events, 0, 100);
}
void getAllEvents()
{
    struct Event *temp = readEvents();

    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
        {
            printf("%d\t%s\t%s\t%s\n", temp[i].id, temp[i].name, temp[i].location, temp[i].date);
        }
    }

}
void addNewEvent()
{
    struct Event temp;
    char name[100];
    char date[100];
    char location[100];

    temp.id = generateEventID();

    printf("Adja meg a nevet!\n");
    scanf("%s", name);
    stpcpy(temp.name, name);

    printf("Adja meg a helyet!\n");
    scanf("%s", location);
    stpcpy(temp.location, location);

    printf("Adja meg az idopontot!\n");
    scanf("%s", date);
    stpcpy(temp.date, date);

    writeEvent(&temp);

    printf("Rendezveny sikeresen hozzaadva!\n");
}
struct Visitor* readVisitors()
{
    registeredVisitors = 0;
    visitorDao.file = fopen("visitorsList.txt", "r");

    if(visitorDao.file != 0)
    {
        int visitorCounter = 0;
        while(fgets(visitorDao.buff, 2048 ,visitorDao.file) != NULL)
        {
            char *line = visitorDao.buff;

            if(line[0] == '\n')
                continue;

            char *token;
            struct Visitor temp;
            int counter = 0;
            while((token = strsep(&line,"\t")) != NULL)
            {
                //printf("%s\n", token);
                switch (counter)
                {
                    case 0:
                        temp.id = atoi(token);
                        break;
                    case 1:
                        temp.eventID = atoi(token);
                        break;
                    case 2:
                        strcpy(temp.name, token);
                        break;
                    case 3:
                        strcpy(temp.emailAddress, token);
                        break;
                    case 4:
                        stpcpy(temp.regTime, token);
                        break;
                }
                ++counter;
            }

            visitors[visitorCounter] = temp;
            ++visitorCounter;


        }

        registeredVisitors = visitorCounter;

        fclose(visitorDao.file);

        return visitors;
    }
    else
    {
        printf("Nem sikerült megnyitni a fájlt!\n");
    }
}


void writeVisitor(struct Visitor *visitor, int status)
{

    visitorDao.file = fopen("visitorsList.txt", "a");

    if(visitorDao.file != 0)
    {
        fprintf(visitorDao.file, "%d\t%d\t%s\t%s\t%s\n", visitor->id, visitor->eventID, visitor->name, visitor->emailAddress, visitor->regTime);

        fclose(visitorDao.file);
        ++registeredVisitors;


        if(status == 1)
            readVisitors();
    }
    else
    {
        printf("Nem sikerült megnyitni a fájlt!");
    }
}
void resetVisitorArray()
{
    memset(visitors, 0, 100);
}
void dropVisitorData()
{
    visitorDao.file = fopen("visitorsList.txt", "w");
    fclose(visitorDao.file);
    registeredVisitors = 0;
}
void deleteEvent()
{
    struct Event *temp = readEvents();
    dropEventData();

    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
        {
            printf("%d %s %s %s\n", temp[i].id, temp[i].name, temp[i].location, temp[i].date);
        }
    }

    int id = -1;
    printf("Adja meg a torlendo esemeny azonositojat:\n ");
    scanf("%d", &id);
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != id && temp[i].id != 0)
        {
            writeEvent(&temp[i]);
        }
    }

    struct Visitor* structVisitor = readVisitors();
    dropVisitorData();
    for(i = 0; i < 100; ++i)
    {
        if(structVisitor[i].eventID != id && structVisitor[i].id != 0)
        {
            writeVisitor(&structVisitor[i], 0);
        }
    }

    resetEventArray();
    resetVisitorArray();
}


int generateVisitorID()
{
    srand(time(NULL));
    return rand();
}
int getTotalRegisteredVisitorNumber()
{
    return registeredVisitors;
}
void getAllVisitors()
{
    struct Visitor *temp = readVisitors();
    resetVisitorArray();

    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
        {
            printf("%d\t%d\t%s\t%s\t%s\n", temp[i].id,  temp[i].eventID, temp[i].name, temp[i].emailAddress, temp[i].regTime);
        }
    }

}
void addNewVisitor()
{
    struct Visitor temp;
    char name[100];
    char email[100];
    int event;
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    stpcpy(temp.regTime, asctime(timeinfo) );

    temp.id = generateVisitorID();

    printf("Adja meg a nevet!\n");
    scanf("%s", name);
    stpcpy(temp.name, name);

    printf("Adja meg az email cimet!\n");
    scanf("%s", email);
    stpcpy(temp.emailAddress, email);


    printf("Adja meg a rendezvenyt!\n");
    event = readInteger();
    temp.eventID = event;




    writeVisitor(&temp,0);

    printf("Latogato sikeresen hozzaadva!\n");
    int counter = getTotalRegisteredVisitorNumber();
    printf("Regisztralt latogato szama: %d\n", counter);
}
void deleteVisitor()
{
    struct Visitor *temp = readVisitors();
    dropVisitorData();
    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
            printf("%d\t%d\t%s\t%s\t%s\n", temp[i].id,  temp[i].eventID, temp[i].name, temp[i].emailAddress, temp[i].regTime);
    }

    int id = -1;

    printf("Adja meg a torlendo szemely azonositojat: ");
    scanf("%d", &id);

    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != id && temp[i].id != 0)
            writeVisitor(&temp[i], 1);

    }
    resetVisitorArray();
}
void getVisitorsByEventID(int eventID)
{
    struct Visitor *temp = readVisitors();

    int i;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].eventID == eventID)
        {
            printf("%d %s %s %d\n", temp[i].id, temp[i].name, temp[i].emailAddress, temp[i].eventID);
        }
    }
}
void updateVisitor()
{
    struct Visitor *temp = readVisitors();

    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
        {
            printf("%d\t%d\t%s\t%s\t%s\n", temp[i].id,  temp[i].eventID, temp[i].name, temp[i].emailAddress, temp[i].regTime);
        }
    }

    int id = -1;
    int index = -1;
    printf("Kerem adja meg a latogato azonositojat: ");
    scanf("%d", &id);

    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id == id)
        {
            index = i;
            break;
        }
    }

    char name[100];
    char email[100];
    int event;

    printf("Adja meg a nevet!\n");
    scanf("%s", name);
    stpcpy(temp[index].name, name);

    printf("Adja meg az email cimet!\n");
    scanf("%s", email);
    stpcpy(temp[index].emailAddress, email);


    printf("Adja meg a rendezvenyt!\n");
    event = readInteger();
    temp[index].eventID = event;


    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    stpcpy(temp[index].regTime, asctime(timeinfo) );

    dropVisitorData();

    for(i = 0; i < 100; ++i)
    {
        if(temp[i].id != 0)
            writeVisitor(&temp[i], 1);

    }
    resetVisitorArray();

}
void addNewVisitorToEventController()
{
    addNewVisitor();
}
void deleteVisitorFromEventController()
{
    deleteVisitor();
}
void listVisitors()
{
    getAllVisitors();
}
void listEvents()
{
    getAllEvents();
}
void deleteEventController()
{
    deleteEvent();
}
void addNewEventController()
{
    addNewEvent();
}
void listVisitorsByEvent()
{
    listEvents();
    int id;
    printf("Kerem a rendezveny szamat:\n");
    scanf("%d", &id);
    getVisitorsByEventID(id);
}
void updateVisitorController()
{
    updateVisitor();
}
int main()
{
    int repeat = 0;
    int choice = -1;
    while(repeat == 0)
    {
        printf("King of Stands elektronikus nyilvantartas.\n");
        printf("Kerem valasszony:\n");
        printf("1) Rendezveny hozzaadasa, 2) Rendezveny torlese, 3) Rendezvenyek listazasa, \n4) Latogato hozzaadas, 5) Latogato torlese, 6) Latogatok listazasa, \n7) Latogato adatainak modositasa, 8) Latogatok listazasa esemenyenkent 9) Kilepes\n");
        choice = readInteger();

        switch (choice)
        {
            case 1:
                addNewEventController();
                break;
            case 2:
                deleteEventController();
                break;
            case 3:
                listEvents();
                break;
            case 4:
                addNewVisitorToEventController();
                break;
            case 5:
                deleteVisitorFromEventController();
                break;
            case 6:
                listVisitors();
                break;
            case 7:
                updateVisitorController();
                break;
            case 8:
                listVisitorsByEvent();
                break;
            case 9:
                repeat = 1;
                break;
        }

    }
    return 0;
}