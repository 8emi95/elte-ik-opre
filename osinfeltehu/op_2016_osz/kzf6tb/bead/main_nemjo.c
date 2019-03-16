#include <stdio.h>
#include <stdlib.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h>  //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand
#include <signal.h>
#include <string.h>

#define RATE_MAX 10

#define EVENTLIMIT 100
#define GUESTLIMIT 50

struct Guest
{
    char name[100];
    char email[100];
    time_t time;
};

struct Event
{
    int guestnum;
    struct Guest *guests;
};

int eventnum = 0;
struct Event events[EVENTLIMIT];

void loadData()
{
    FILE *f;
    f = fopen("out.txt", "r");

    if (f == NULL)
    {
        printf("##Betoltes sikertelen");
        return;
    }

    fread(&eventnum, sizeof(int), 1, f);
    int i;
    for (i = 0; i < eventnum; i++)
    {
        fread(&events[i].guestnum, sizeof(int), 1, f);
        events[i].guests = (struct Guest *)malloc(GUESTLIMIT * sizeof(struct Guest));
        fread(events[i].guests, sizeof(*events[i].guests), events[i].guestnum, f);
    }
}

int count = 0;
struct itimerval timer;

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}
void handlersetitimer(int signalnumber)
{
        printf("[Szulo] timer\n");
        timer.it_interval.tv_sec = 0; /* it will be stopped */
        timer.it_interval.tv_usec = 0;

}

void printNames(char *ptrNames, int bytes)
{
    int charIndex = 0;
    char currentGuestName[100];
    int i;
    for (i = 0; i < bytes; ++i)
    {
        currentGuestName[charIndex++] = *ptrNames;
        if (*(ptrNames++) == '\0')
        {
            charIndex = 0;

            printf("[Szulo] Megjelent: %s \n", currentGuestName);
        }
    }
}

int main()
{
    // event loading
    loadData();
    if (eventnum == 0)
    {
        printf("nincs esemeny");
        exit(2);
    }

    //  signal
    signal(SIGTERM, handler);

    // pipe
    int fPipe = mkfifo("mypipe", 0600);
    if (fPipe < 0)
    {
        perror("error");
        exit(1);
    }
    fPipe = open("mypipe", O_RDWR);
    struct pollfd poll_fds[2];
    poll_fds[0].fd = fPipe;
    poll_fds[0].events = POLLIN;

    // string buffer
    char strBuff[100];
            struct Event myEvent = events[0];

pid_t childPid = fork();;
    int eventind;

   // for (eventind = 0; eventind < eventnum; ++eventind)
   // {
        //fork();
                printf("_________________ %s, %i\n", childPid, eventnum);


        printf("_________________ %s\n", childPid);
        // start child

        // parent
        if (childPid > 0)
        {
            write(fPipe, "Budapest", 9);
            printf("[Szulo] Cel elkuldve\n");

            pause();
            printf("[Szulo] Gyerek megerkezett\n");

            // send guests
            int i;
            for (i = 0; i < myEvent.guestnum; ++i)
            {
                write(fPipe, myEvent.guests[i].name, strlen(myEvent.guests[i].name) + 1);
                //printf("[Szulo] Nev elkuldve: %s\n", myEvent.guests[i].name);
            }

            // timeout for pause
            struct sigaction sigact;
            sigemptyset(&sigact.sa_mask);
            sigact.sa_handler = handlersetitimer;
            sigact.sa_flags = 0;
            sigaction(SIGALRM, &sigact, NULL);

            timer.it_interval.tv_sec = 0;
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_sec = 15;
            timer.it_value.tv_usec = 0;

            setitimer(ITIMER_REAL, &timer, NULL);

            // wait for rating and names (or timeout)
                        printf("[Szulo] Var\n");

            pause();
                        printf("[Szulo] Var vege\n");

            struct itimerval timer2;
            timer2.it_interval.tv_sec = 0;
            timer2.it_interval.tv_usec = 0;
            timer2.it_value.tv_sec = 0;
            timer2.it_value.tv_usec = 0;

            setitimer(ITIMER_REAL, &timer2, &timer);



            int result = poll(poll_fds, 1, 3000); //
            if (result > 0)
            {
                int bytes = read(fPipe, strBuff, sizeof(strBuff));
                printf("[Szulo] Az esemeny befejezodott. Ertekeles: %s\n", strBuff);

                char *buffPointer = strBuff;
                buffPointer += strlen(strBuff) + 1;
                bytes -= strlen(strBuff) + 1;

                printNames(buffPointer, bytes);
                while ((result = poll(poll_fds, 1, 3000)) > 0)
                {
                    int bytes = read(fPipe, strBuff, sizeof(strBuff));
                    printNames(strBuff, bytes);
                }
            }
            else
            {
                printf("[Szulo] nem jott hir a rendezvenyrol, valami baj torenhetett...\n");
                kill(childPid, SIGKILL);
            }
            wait();
            printf("[Szulo] Vege\n");
        }
        // child
        else
        {
            int result = poll(poll_fds, 1, 3000);
            if (result > 0)
            {
                read(fPipe, strBuff, sizeof(strBuff));

                // travel to destination
                sleep(2);
                printf("[Gyerek] Megerkeztem ide: %s \n", strBuff);

                // notify parent
                kill(getppid(), SIGTERM);
            }
            else
            {
                perror("nincs uticel");
            }

            // recieve guests
            int guestnum = 0;
            char guestList[GUESTLIMIT][100];
            int charIndex = 0;

            while ((result = poll(poll_fds, 1, 3000)) > 0)
            {
                if (poll_fds[0].revents & POLLIN)
                {
                    int bytes = read(fPipe, strBuff, sizeof(strBuff));

                    int i;
                    for (i = 0; i < bytes; ++i)
                    {
                        guestList[guestnum][charIndex++] = strBuff[i];
                        if (strBuff[i] == '\0')
                        {
                            ++guestnum;
                            charIndex = 0;

                            printf("[Gyerek] Nevet kaptam: %s \n", guestList[guestnum - 1]);
                        }
                    }
                }
            }

            printf("[Gyerek] Megvannak a nevek. Esemeny hamarosan kezodik\n");

            // wait for event to end
            sleep(3);
            printf("[Gyerek] Vege az esemenyek. Jelentes kuldese...\n");

            // send rating
            char strRating[10];
            sprintf(strRating, "%i/%i", rand() % RATE_MAX + 1, RATE_MAX);
            write(fPipe, strRating, strlen(strRating) + 1);

            // send attended guests
            int j;
            for (j = 0; j < guestnum; ++j)
            {
                if (rand() % 101 >= 10)
                {
                    write(fPipe, guestList[j], strlen(guestList[j]) + 1);
                }
            }

            // notify parent
            kill(getppid(), SIGTERM);
            printf("[Gyerek] Vege\n");
        }
   // }
    unlink("mypipe");
    return 1;
}
