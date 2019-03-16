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
#include <ctype.h>
#include <time.h>

typedef int bool;
enum { false, true };
int eventCount = 0;

int menu()
{
	printf("King of Stands\n");
	printf("1 - uj latogato hozzaadasa\n");
	printf("2 - latogato modositasa\n");
	printf("3 - latogato torlese\n");
	printf("4 - uj rendezveny hozzaadasa\n");
	printf("5 - rendezveny torlese\n");
	printf("6 - rendezveny resztvevoinek listazasa\n");
	printf("7 - rendezveny lebonyolitasa\n");
	printf("8 - programbol valo kilepes\n");

	char numchar[1];
	int number;
	bool valid = false;
	do
	{
		printf("A te valasztasod: ");
		scanf("%s%*c", &numchar);
		number = atoi(numchar);
		if ((number == 0 && *numchar != '0') || (number<= 0 || number > 8))
		{
			printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot. Probald ujra!\n");
			printf("\n");
		}
		else valid = true;

	} while (!valid);
	return number;
}
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

int eventMenu()
 {
     if(eventCount==0)
     {
         printf("Meg nincs letrehozva egy esemeny sem! Nyomj egy gombot a folytatashoz!\n");
         return -1;
     }
     printf("Az osszes esemeny listaja: ");
     int i;
     printf("azonosito\tesemeny neve\n");
     printf("\n");
     for(i=0;i<eventCount;i++)
     {
         printf("%d\t\t%s\n",i+1,myEvents[i].eventName);
     }
     printf("\n");

    char numchar[1];
    int number;
    bool valid = false;
	do
	{
		printf("A te valasztasod: ");
		scanf("%s%*c", &numchar);
		number = atoi(numchar);
		if ((number == 0 && *numchar != '0') || (number<= 0 || number > eventCount))
		{
			printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot. Probald ujra! ");
			printf("\n");
		}
		else valid = true;

	} while (!valid);
	return number;
 }

void addEvent() {
	if (eventCount >= 10) {
		printf("Mar nem adhato hozza tobb esemeny! Torolj egy mar meglevot!\n");
		return;
	}
	eventCount++;
	char eventName[100];
	printf("Esemeny neve: ");
	scanf("%s%*c",eventName);
    	printf("\n");
	myEvents[eventCount-1].myGuests = (struct Guests*)malloc(10 * sizeof(struct Guests));
	strcpy(myEvents[eventCount-1].eventName, eventName);
	printf("Esemeny letrehozva!");
	printf("Azonosito: %d\n", eventCount);
	printf("Nyomj meg egy gombot a fomenube valo visszatereshez\n");
}

void removeEvent() {
	int id = eventMenu();
	if (id != -1)
    {
        myEvents[id-1].guestCount = 0;
        int i;
        for (i = id-1 ; i < eventCount-1; i++ )
        {
            myEvents[i] = myEvents[i+1];
        }
        eventCount--;
        printf("A torles sikeres!");
        return;
    }
	else
    {
        printf("A torles sikertelen! Nyomjon egy gombot a folytatashoz!\n");
    }
}
int getListOfGuests()
 {
	 int id = eventMenu();
	 if(id==-1)
     {
         return -1;
     }
     if(myEvents[id].guestCount==0)
     {
         printf("Az esemenyhez meg nincs resztvevo!\n");
     }
     id--;
	 if (id != -1 && myEvents[id].guestCount>0)
	 {
		 printf("Jelentkezettek a %d. azonositoju esemenyhez:\n\n",id+1);
		 int i;
		 for (i = 0; i < myEvents[id].guestCount; i++)
		 {
			 printf("%d. jelentkezo neve: %s\n", i + 1, myEvents[id].myGuests[i].name);
			 printf("%d. jelentkezo e-mail cime: %s\n", i + 1, myEvents[id].myGuests[i].email);
			 printf("%d. jelentkezo jelentkezesenek ideje: %s\n", i + 1, ctime(&myEvents[id].myGuests[i].now));
			 printf("\n");
		 }
	 }
	 return id;
 }
void removeGuest() {
    int id=getListOfGuests();
    if(id==-1)
    {
        return;
    }
    if(myEvents[id].guestCount!=0)
    {
        printf("Az esemenyhez tartozo vendegek\n");
        char numchar[1];
        int number;
        printf("Torolni kivant vendeg sorszama: ");
        scanf("%s%*c", &numchar);
        number = atoi(numchar);
        if ((number == 0 && *numchar != '0')|| (number > myEvents[id].guestCount)) {
            printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot.\n");
            printf("Nyomj meg egy gombot a fomenube valo visszatereshez.\n");
            return;
        }
        if (myEvents[id].guestCount > 1)
        {
            int i;
            for (i = number-1 ; i < myEvents[id].guestCount-1; i++ )
            myEvents[id].myGuests[i] = myEvents[id].myGuests[i+1];

        }
        printf("A torles sikeres! Nyomjon egy gombot a folytatashoz!\n");
        myEvents[id].guestCount--;
    }
    else
    {
        printf("Az esemenyhez meg nincs resztvevo!\n");
        return;
    }
}

void addGuest() {
	int id = eventMenu();
	if (id == -1) {
		return;
	}
	else
    {
        char name[100];
        char email[100];

        printf("Neve: ");
        scanf("%s",name);

        printf("E-mail cime: ");
        scanf("%s",email);

        id--;
        int allGuests = myEvents[id].guestCount++;

        strcpy(myEvents[id].myGuests[allGuests].name, name);
        strcpy(myEvents[id].myGuests[allGuests].email, email);
        myEvents[id].myGuests[allGuests].now = time(0);
        printf("A jelentezes megtortent a %d. helyen\n",allGuests+1);
        printf("Nyomj meg egy gombot a fomenube valo visszatereshez!\n");
    }
}
void editGuest() {
    int id=getListOfGuests();
    if(myEvents[id].guestCount!=0)
    {
        printf("Az esemenyhez tartozo vendegek\n");
        char numchar[1];
        int number;
        printf("Modositani kivant vendeg sorszama: ");
        scanf("%s%*c", &numchar);
        number = atoi(numchar);
        if (number == 0) {
            printf("Nem szamot irtal be!\n");
            return;
        }
        if (myEvents[id].guestCount > 0)
        {
            number--;
            char name[100];
            char email[100];

            printf("Neve: ");
            scanf("%s",name);

            printf("E-mail cime: ");
            scanf("%s",email);

            strcpy(myEvents[id].myGuests[number].name, name);
            strcpy(myEvents[id].myGuests[number].email, email);

            printf("Modositas sikeres! Nyomjon egy gombot a folytatashoz!\n");
        }
    }
}

 void fromFile()
 {
	 FILE* file=fopen("savepoint.txt", "r");
	 if (file == NULL)
	 {
		 printf("Hiba tortent a betoltesnel!\n");
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
 void toFile()
 {
	 FILE* file = fopen("savepoint.txt", "w");
	 if (file == NULL)
	 {
		 printf("Hiba tortent a mentesnel!\n");
		 return;
	 }
	 fwrite(&eventCount, sizeof(int), 1, file);
	 int i;
	 for (i=0; i < eventCount; i++)
	 {
		 fwrite(&myEvents[i].guestCount, sizeof(int), 1, file);
		 fwrite(&myEvents[i].eventName, 100,1,file);
		 fwrite(myEvents[i].myGuests, sizeof(*myEvents[i].myGuests), myEvents[i].guestCount, file);
		 fprintf(file, "\n");
	 }
	 fclose(file);
 }

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

void printNames(char *ptrNames, int length)
{
    int index = 0;
    char cur[100];
    int i;
    int rnd=0;
    for (i = 0; i < length; i++)
    {
	rnd=rand() % 101;
	if(rnd<90)
	{
		cur[index] = *ptrNames;
		index++;
        	if (*(ptrNames++) == '\0')
        	{
            		index = 0;
            		printf("[Szulo] Aki megjelent a rendezvenyen: %s \n", cur);
        	}
	}
        
    }
}

int execute(int id, int fPipe, struct pollfd *poll_fds)
{
    struct Events myEvent = myEvents[id];
    char buffer[100];

    pid_t childPid = fork();
    if (childPid > 0)
    {
        write(fPipe, "[Cel]", 9);
        printf("[Szulo] Cel elkuldve\n");

        pause();
        printf("[Szulo] Gyerek megerkezett\n");

        int i;
        for (i = 0; i < myEvent.guestCount; i++)
        {
            write(fPipe, myEvent.myGuests[i].name, strlen(myEvent.myGuests[i].name) + 1);
        }
        pause();

        int result = poll(poll_fds, 1, 3000);
        if (result > 0)
        {
            int length = read(fPipe, buffer, sizeof(buffer));
            printf("[Szulo] Az esemeny ertekelese: %s\n", buffer);

            char *pBuff = buffer;
            pBuff += strlen(buffer) + 1;
            length -= strlen(buffer) + 1;

            printNames(pBuff, length);
            while ((result = poll(poll_fds, 1, 3000)) > 0)
            {
                if (poll_fds[0].revents & POLLIN)
                {
                    int length = read(fPipe, buffer, sizeof(buffer));
                    printNames(buffer, length);
                }
                else
		{
			break;
		}
                    
         }
    }
        else
        {
            printf("[Szulo] nincs hir a rendezvenyrol\n");
            kill(childPid, SIGKILL);
        }
        wait();
        printf("[Szulo] Lebonyolitas vege\n");

        return 0;
    }

    else
    {
        int child = poll(poll_fds, 1, 3000);
        if (child > 0)
        {
            read(fPipe, buffer, sizeof(buffer));
            sleep(2);
            printf("[Gyerek] Megerkeztem ide: %s \n", buffer);
            kill(getppid(), SIGTERM);
        }
        else
        {
            perror("nincs uticel");
        }

        int guestCount = 0;
        char guestList[10][10];
        int cindex = 0;

        while ((child = poll(poll_fds, 1, 3000)) > 0)
        {
            if (poll_fds[0].revents & POLLIN)
            {
                int length = read(fPipe, buffer, sizeof(buffer));

                int i;
		int c=1;
                for (i = 0; i < length; i++)
                {
                    guestList[guestCount][cindex] = buffer[i];
		    cindex++;
                    if (buffer[i] == '\0')
                    {
                        guestCount++;
                        cindex = 0;
                        printf("[Gyerek] %d. resztvevo: %s \n",c, guestList[guestCount - 1]);
			c++;
                    }
			
                }
            } else
                break;

        }

        printf("[Gyerek] Itt van a jelentkezettek listaja.\n");

        sleep(1);
        printf("[Gyerek] Vege az esemenynek.\n");

        char strRating[10];
	int rnd=rand()%11;
        sprintf(strRating, "%i/10", rnd);
        write(fPipe, strRating, strlen(strRating) + 1);

        int i;
        for (i = 0; i < guestCount; i++)
        {
            if (rand() % 101 >= 10)
            {
                write(fPipe, guestList[i], strlen(guestList[i]) + 1);
            }
        }

        kill(getppid(), SIGTERM);
        printf("[Gyerek] Gyerek folyamat vege\n");

        return 0;
    }
}
void playEvent()
{
	if(eventCount==0)
	{
		return;
	}
	signal(SIGTERM, handler);

    	int fPipe = mkfifo("pipe", 0600);
    	if (fPipe < 0)
    	{
        	perror("error");
        	exit(1);
    	}
    	fPipe = open("pipe", O_RDWR);

    	struct pollfd poll_fds[2];
    	poll_fds[0].fd = fPipe;
    	poll_fds[0].events = POLLIN;

    	int eventid = 0;
    	int r = 1;
    	do
    	{
        	r = execute(eventid, fPipe, poll_fds);
        	if (r == 1)
        	{
            		eventid++;
            		poll_fds[0].fd = fPipe;
            		poll_fds[0].events = POLLIN;
        	}
    	} while (r == 1 && eventid < eventCount);
    unlink("pipe");
}

int main()
{
	int option;
	do
	{	
		option = menu();
		fromFile();
		switch (option)
		{
		case 1:
			printf("Látogato felvetele\n");
			addGuest();
			break;
		case 2:
			printf("Latogato modositasa\n");
            		editGuest();
			break;
		case 3:
			printf("Latogato torlese\n");
            		removeGuest();
			break;
		case 4:
			printf("Esemeny letrehozasa\n");
			addEvent();
			break;
		case 5:
			printf("Esemeny torlese\n");
			removeEvent();
			break;
		case 6:
			printf("Esemeny latogatoinak listazasa\n");
			getListOfGuests();
			printf("A folytatashoz nyomj meg egy gombot!\n");
			break;
		case 7:
			playEvent();
			break;

		case 8:
			exit(0);
		}
		toFile();
	} while (option!=7);
    return 0;
}
