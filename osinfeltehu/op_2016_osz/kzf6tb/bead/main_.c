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
    count++;
    //printf("The program was alarmed by setitimer the %i. time\n", count);
    if (count == 5)
    {
        timer.it_interval.tv_sec = 0; /* it will be stopped */
        timer.it_interval.tv_usec = 0;

        //raise(SIGTERM); //it will stop process
        // raise a signal and send it to the process itself
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
    struct Event myEvent = events[0];

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

    // start child
    pid_t childPid = fork();

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
            write(fPipe, myEvent.guests[i].name, strlen(myEvent.guests[i].name)+1);
            printf("[Szulo] Nev elkuldve: %s\n", myEvent.guests[i].name);
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
        pause();
        //int result = poll(poll_fds, 1, 3000); //
        printf("[Szulo] Var1\n");
        sleep(6);

        read(fPipe, strBuff, sizeof(strBuff));
        printf("[Szulo] Az esemeny befejezodott. Ertekeles: %s\n", strBuff);

        while (read(fPipe, strBuff, sizeof(strBuff)) > 0) {
            read(fPipe, strBuff, sizeof(strBuff));
            printf("[Szulo] Vendeg: %s \n", strBuff);
            sleep(1);
        }

        wait();
        printf("[Szulo] Vege\n");
    }
    // child
    else
    {
        //int result = poll(poll_fds, 1, 3000); //

        printf("[Gyerek] Var1\n");

        sleep(2);

        read(fPipe, strBuff, sizeof(strBuff));

        // travel to destination
        //sleep(1);
        printf("[Gyerek] Megerkeztem ide: %s \n", strBuff);

        // notify parent
        kill(getppid(), SIGTERM);

        printf("[Gyerek] Var2\n");

        sleep(2);
        // recieve guests
        int guestnum = 0;
        char guestList[GUESTLIMIT][100];
        while (read(fPipe, strBuff, 100) > 0) {
            printf("[Gyerek] Nevet kaptam: %s \n", strBuff);
            strcpy(guestList[guestnum++], strBuff);
            //sleep(1);
        }

        printf("[Gyerek] Megvannak a nevek. Esemeny hamarosan kezodik\n");

        // wait for event to end
        //sleep(1);
        printf("[Gyerek] Vege az esemenyek. Jelentes kuldese...\n");

        // send rating
        char strRating[10];
        sprintf(strRating, "%i/%i", rand() % RATE_MAX + 1, RATE_MAX);
        write(fPipe, strRating, strlen(strRating)+1);
    
        // send attended guests
        int j;
        for (j = 0; j < guestnum; ++j) {
            if (rand()%101  > 10){
                int wr = write(fPipe, guestList[j], strlen(guestList[j])+1);
                printf("[Gyerek] Megjelent: %s, w: %i\n", guestList[j], wr);
            }
        }

        // notify parent
        kill(getppid(), SIGTERM);

        //sleep(10);
        printf("[Gyerek] Vege\n");
    }

    unlink("mypipe");
    return 1;
}
