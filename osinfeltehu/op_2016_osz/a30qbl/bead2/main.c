#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>

#define RATE_MAX 10

#define GUESTLIMIT 50

typedef int bool;
enum { false, true };
int eventCount = 0;

struct Guests
{
	char name[100];
	char email[100];
	time_t now;
};

struct Events
{
    char eventName[100];
	int guestCount;
	struct Guests* myGuests;
};
struct Events myEvents[10];

 void fromFile()
 {
	 FILE* file=fopen("savepoint.txt", "r");
	 if (file == NULL)
	 {
		 puts("Hiba tortent a betoltesnel!");
		 return;
	 }
	 fread(&eventCount, sizeof(int), 1, file);
	 int i;
	 for (i = 0; i < eventCount; i++)
	 {
		 fread(&myEvents[i].guestCount, sizeof(int), 1, file);
		 fread(&myEvents[i].eventName, 100,1,file);
		 myEvents[i].myGuests = (struct Guests*)malloc(10 * sizeof(struct Guests));
		 fread(myEvents[i].myGuests, sizeof(*myEvents[i].myGuests), myEvents[i].guestCount, file);
		 fscanf(file,"\n");
	 }
	 fclose(file);
 }

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
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

int handleEvent(int eventid, int fPipe, struct pollfd *poll_fds)
{
    struct Events myEvent = myEvents[eventid];
    char strBuff[100];

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
        for (i = 0; i < myEvent.guestCount; ++i)
        {
            write(fPipe, myEvent.myGuests[i].name, strlen(myEvent.myGuests[i].name) + 1);
            //printf("[Szulo] Nev elkuldve: %s\n", myEvent.guests[i].name);
        }

        // wait for event to end
        pause();

        int result = poll(poll_fds, 1, 3000); //
        if (result > 0)
        {
            int bytes = read(fPipe, strBuff, sizeof(strBuff));
            printf("[Szulo] Az esemeny befejezodott. Ertekeles: %s\n", strBuff);

            // read all the names
            char *buffPointer = strBuff;
            buffPointer += strlen(strBuff) + 1;
            bytes -= strlen(strBuff) + 1;

            printNames(buffPointer, bytes);
            while ((result = poll(poll_fds, 1, 3000)) > 0)
            {
                if (poll_fds[0].revents & POLLIN)
                {
                    int bytes = read(fPipe, strBuff, sizeof(strBuff));
                    printNames(strBuff, bytes);
                }
                else
                    break;
            }
        }
        else
        {
            printf("[Szulo] nem jott hir a rendezvenyrol, valami baj torenhetett...\n");
            kill(childPid, SIGKILL);
        }
        wait();
        printf("[Szulo] Vege\n");

        return 1;
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
        int guestCount = 0;
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
                    guestList[guestCount][charIndex++] = strBuff[i];
                    if (strBuff[i] == '\0')
                    {
                        guestCount++;
                        charIndex = 0;

                        printf("[Gyerek] Nevet kaptam: %s \n", guestList[guestnum - 1]);
                    }
                }
            } else
                break;

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
        for (j = 0; j < guestCount; ++j)
        {
            if (rand() % 101 >= 10)
            {
                write(fPipe, guestList[j], strlen(guestList[j]) + 1);
            }
        }

        // notify parent
        kill(getppid(), SIGTERM);
        printf("[Gyerek] Vege\n");

        return 0;
    }
}

int main()
{
    // event loading
    fromFile();
    if (eventCount == 0)
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

    int eventid = 0;
    int r = 1;
    do
    {
        r = handleEvent(eventid, fPipe, poll_fds);

        if (r == 1)
        {
            ++eventid;
            poll_fds[0].fd = fPipe;
            poll_fds[0].events = POLLIN;
        }
    } while (r == 1 && eventid < eventCount);


    unlink("mypipe");
    return 1;
}
