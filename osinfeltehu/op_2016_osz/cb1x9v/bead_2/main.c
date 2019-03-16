
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define TRUE 1

struct Event
{
    int id;
    char name[1024];
    char localtion[1024];
    char date[1024];
};
struct Visitor
{
    int id;
    char name[1024];
    int age;
    int eventID;
    int brokenLeg;
};
struct Report
{
    long success;
    struct Visitor notAttended[1024];
};
static struct Event events[10];
static struct Visitor tempVisitors[100];
static struct Visitor visitors[100];
static char tempName[10];
static int eventCounter = 0;

char *generateRandomWord()
{
    memset(tempName, 0, 10);
    int i = 0;
    for(i = 0; i < 10; ++i)
    {
        tempName[i] =  'A' + (random() % 26);
    }

    return tempName;
}
void initArrays()
{
    int i = 0;
    for(i = 0; i < 10; ++i)
    {
        struct Event temp;
        temp.id = i;
        stpcpy(temp.name, generateRandomWord());
        stpcpy(temp.localtion, "ELTE");
        stpcpy(temp.date, "2016.11.12");

        events[i] = temp;
    }
    for(i = 0; i < 100; ++i)
    {
        struct Visitor temp;
        temp.id = rand() % 100;
        temp.age = 20;
        stpcpy(temp.name, generateRandomWord());
        temp.eventID = i % 10;
        temp.brokenLeg = rand() % 100;
        visitors[i] = temp;

    }
}
void getVisitorsByEventNumber(int number)
{
    memset(tempVisitors, 0, sizeof(tempVisitors));
    int i = 0;
    for(i = 0; i < 100; ++i)
    {
        if(visitors[i].eventID == number)
        {
            tempVisitors[i] = visitors[i];
        }
    }
}

void getNotAttendedVisitors(int number)
{

    int i;
    for(i = 0; i < 100; ++i)
    {
        if(visitors[i].id < 10 && visitors[i].eventID == number)
        {
            printf("%d\t%s\t%d\t%d\n", visitors[i].id, visitors[i].name, visitors[i].age, visitors[i].eventID);
        }
    }
}
void printVisitors(struct Visitor *visitors,int size)
{
    int i = 0;
    for(i = 0; i < size; ++i)
    {
        if(visitors[i].id != 0)
        {
            printf("%d\t%s\t%d\t%d\n", visitors[i].id, visitors[i].name, visitors[i].age, visitors[i].eventID);
        }
    }
}

void signalHandler(int signumber)
{
    printf("Signal with number %i has arrived\n",signumber);
}
void signalReport(int sigNumber)
{
    struct Report report;
    printf("Generating report....\n");
    srand(time(NULL));
    report.success = rand() % 20;
    printf("%ld\n", report.success);
    printf("Did not attend:\n");
    getNotAttendedVisitors(eventCounter);

}
void signalQuit(int sigNumber)
{
    printf("Signal with number %i has arrived\n", sigNumber);
    _exit(0);
}

int main()
{
    initArrays();
    int locationPipe[2];
    int visitorPipe[2];
    char locationBuffer[1024];
    struct Visitor visitorBuffer[1024];
    pid_t cpid;

    signal(SIGTERM,signalHandler);
    signal(SIGINT, signalReport);
    signal(SIGQUIT, signalQuit);

    while(eventCounter < 10)
    {

        printf("Processing event: %d\n", eventCounter);
        if (pipe(locationPipe) == -1)
        {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }
        if(pipe(visitorPipe) == -1)
        {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }

        cpid = fork();
        if (cpid == -1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

        if(cpid == 0)
        {
            close(locationPipe[1]);
            read(locationPipe[0], locationBuffer, sizeof(locationBuffer));
            close(locationPipe[0]);
            printf("Travel to: %s\n", locationBuffer);
            sleep(10);
            printf("Arrived\n");

            kill(getpid(), SIGTERM);

            close(visitorPipe[1]);
            read(visitorPipe[0], visitorBuffer, sizeof(visitorBuffer));
            close(visitorPipe[0]);
            printf("Attending: \n");
            printVisitors(visitorBuffer, 100);
            printf("Event in progress\n");
            sleep(10);
            printf("Finished\n");

            kill(getpid(), SIGINT);
        }
        else
        {

            close(locationPipe[0]);
            write(locationPipe[1], events[eventCounter].localtion, sizeof(events[eventCounter].localtion));
            close(locationPipe[1]);

            getVisitorsByEventNumber(events[eventCounter].id);

            close(visitorPipe[0]);
            write(visitorPipe[1], tempVisitors, sizeof(tempVisitors));
            close(visitorPipe[1]);

            int status;
            pause();
            wait(&status);
        }

        ++eventCounter;
    }


    printf("Exiting program...\n");
    kill(0, SIGKILL);
    _exit(0);

}

