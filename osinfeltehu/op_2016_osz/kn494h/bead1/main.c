#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //read
#include <time.h> //time
#include <string.h> //strncpy
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define TIMESIZE 20

int fd[2], nbytes;
pid_t childpid;
pid_t parentpid;
char readbuffer[80];
int status;
key_t key;
int messageQ;

typedef struct Guest
{
    char* name;
    char* mail;
    int id;
    char* regTime;
}Guest;

typedef struct Event
{
    char* name;
    int id;
	char* location;
}Event;

typedef struct KingOfStands
{
    Guest* guests;
    size_t gSize;
    size_t gUsed;
    Event* events;
    size_t eSize;
    size_t eUsed;
    int eIDCount;
}KingOfStands;

void insertGuest(KingOfStands* kos, Guest* g)
{
    if (kos->gUsed == kos->gSize)
    {
        kos->gSize++;
        kos->guests = realloc(kos->guests, kos->gSize * sizeof(*(kos->guests)));
    }
    kos->guests[kos->gUsed++] = *g;
}

void insertEvent(KingOfStands *kos, Event* e)
{
    if (kos->eUsed == kos->eSize)
    {
        kos->eSize++;
        kos->events = realloc(kos->events, kos->eSize * sizeof(*(kos->events)));
    }
    kos->events[kos->eUsed++] = *e;
}

void freeKos(KingOfStands *kos)
{
    free(kos->guests);
    kos->guests = NULL;
    kos->gUsed = kos->gSize = 0;
    free(kos->events);
    kos->events = NULL;
    kos->eUsed = kos->eSize = 0;
}

char * get_line(void)
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;)
    {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0)
        {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);
            if(linen == NULL)
            {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

void printMenu()
{
    printf("\nAdd new guest:\t (1)\n");
    printf("List all guests: (2)\n");
    printf("Edit guest:\t (3)\n");
    printf("Delete guest:\t (4)\n");
    printf("Add new event:\t (5)\n");
    printf("Delete event:\t (6)\n");
	printf("Launch event:\t (7)\n");
    printf("Exit:\t\t (q)\n");
}

bool kosHasEventID(KingOfStands* kos, int id)
{
    int i;
    for(i=0; i<kos->eSize; ++i)
    {
        if(kos->events[i].id == id)
            return true;
    }
    return false;
}

void addEvent(KingOfStands* kos)
{
    Event* e = &(struct Event) {.name = "", .id = -1, .location=""};
    printf("Name:\n");
    e->name = get_line();
    kos->eIDCount++;
    e->id = kos->eIDCount;
	printf("Location:\n");
	e->location = get_line();
    insertEvent(kos, e);
    printf("Event with ID of %i has been created!\n", e->id);
}

int getIDCount(KingOfStands* kos, int id)
{
    int c = 0;
    int i;
    for(i=0; i<kos->gSize; ++i)
    {
        if(kos->guests[i].id == id)
            c++;
    }
    return c;
}

void addGuest(KingOfStands* kos)
{
    bool goodID = false;
    while(goodID == false)
    {
        Guest* g = &(struct Guest) {.name = "", .mail = "", .id = -1};
        printf("Name:\n");
        g->name = get_line();
        printf("Email:\n");
        g->mail = get_line();
        printf("EventID:\n");
        g->id = atoi(get_line());
        g->regTime = malloc(TIMESIZE);
        char buff[TIMESIZE];
        time_t now = time(NULL);
        strftime(buff, TIMESIZE, "%Y-%m-%d %H:%M:%S", localtime(&now));
        int i;
        for(i=0; i<TIMESIZE; ++i)
        {
            g->regTime[i] = buff[i];
        }
        if(kosHasEventID(kos, g->id))
        {
            insertGuest(kos, g);
            printf("Registration complete! You are the %i. guest on this event!\n", getIDCount(kos, g->id));
            goodID = true;
        }
        else
        {
            printf("There is no EventID of %i!\n", g->id);
        }
    }
}

void listGuests(KingOfStands* kos)
{
    int i;
    for(i=0; i<kos->gSize; ++i)
    {
        printf("EventID:%i\nName:%s\nMail:%s\n", kos->guests[i].id, kos->guests[i].name, kos->guests[i].mail);
        printf("Register time: %s\n", kos->guests[i].regTime);
    }
    printf("\n");
}

void deleteGuest(KingOfStands* kos, char* name, char* mail)
{
    Guest* temp;
    if(kos->gSize > 1)
        temp = malloc((kos->gSize - 1) * sizeof(*(kos->guests)));
    else
        temp = malloc(sizeof(*(kos->guests)));
    bool found = false;
    int i = 0;
    while(i < kos->gSize)
    {
        if(strcmp(kos->guests[i].name, name) == 0 && strcmp(kos->guests[i].mail, mail) == 0)
        {
            found = true;
            i++;
        }
        if(found)
        {
            temp[i-1].name = kos->guests[i].name;
            temp[i-1].mail = kos->guests[i].mail;
            temp[i-1].id = kos->guests[i].id;
            temp[i-1].regTime = kos->guests[i].regTime;
        }
        else
        {
            temp[i].name = kos->guests[i].name;
            temp[i].mail = kos->guests[i].mail;
            temp[i].id = kos->guests[i].id;
            temp[i].regTime = kos->guests[i].regTime;
        }
        i++;
    }
    if(found)
    {
        free(kos->guests);
        kos->guests = NULL;
        kos->guests = temp;
        kos->gSize--;
        kos->gUsed--;
        printf("The guest of name %s has been removed!", name);
    }
    else
    {
        printf("There is no guest with the name %s and mail of %s", name, mail);
    }
}

void deleteGuests(KingOfStands* kos, int eID) // delete all guests with eventID of eID
{
    int i = 0;
    while(i <= kos->gSize)
    {
        if(kos->guests[i].id == eID)
        {
            deleteGuest(kos, kos->guests[i].name, kos->guests[i].mail);
            i--;
        }
        i++;
    }
}

void deleteEvent(KingOfStands* kos)
{
    printf("Enter the ID of the Event:");
    int eID = atoi(get_line());


    if(kosHasEventID(kos, eID))
    {
        Event* temp;
        if(kos->eSize > 1)
            temp = malloc((kos->eSize - 1) * sizeof(*(kos->events)));
        else
            temp = malloc(sizeof(*(kos->events)));
        bool found = false;
        int i = 0;
        while(i <= kos->eSize)
        {
            if(kos->events[i].id == eID)//only once, because id's are unique
            {
                found = true;
                i++;
            }
            if(found)
            {
                temp[i-1].name = kos->events[i].name;
                temp[i-1].id = kos->events[i].id;
				temp[i-1].location = kos->events[i].location;
            }
            else
            {
                temp[i].name = kos->events[i].name;
                temp[i].id = kos->events[i].id;
				temp[i].location = kos->events[i].location;
            }
            i++;
        }

        free(kos->events);
        kos->events = NULL;
        kos->events = temp;
        kos->eSize--;
        kos->eUsed--;
        deleteGuests(kos, eID);
        printf("The event of %i ID has been removed with all guests!", eID);
    }
    else
        printf("There is no event with ID of %i!", eID);
}

void editGuest(KingOfStands* kos, char* name, char* mail, int eID)
{
    bool goodID = false;
    printf("New name:");
    char* nname = get_line();
    printf("New mail:");
    char* nmail = get_line();
    printf("New eventID:");
    int neID = atoi(get_line());
    do
    {
        printf("New eventID:");
        neID = atoi(get_line());
        int i;
        for(i=0; i<kos->eSize; ++i)
        {
            if(kos->events[i].id == neID)
                goodID = true;
            else
                printf("There is no event with the ID of %i!", neID);
        }

    }while(goodID == false);

    int i;
    for(i=0; i<kos->gSize; ++i)
    {
        if(kos->guests[i].id == eID && strcmp(kos->guests[i].name, name) == 0 && strcmp(kos->guests[i].mail, mail) == 0)
        {
            kos->guests[i].id = neID;
            kos->guests[i].name = nname;
            kos->guests[i].mail = nmail;
        }
    }

}

void loadKos(KingOfStands* kos, char* filename)
{
    FILE* fp = fopen(filename, "r");
    char* line = malloc(10);
    fgets(line, 34, fp);
    kos->gSize = atoi(line);
    fgets(line, 34, fp);
    kos->gUsed = atoi(line);
    fgets(line, 34, fp);
    kos->eSize = atoi(line);
    fgets(line, 34, fp);
    kos->eUsed = atoi(line);


    int i;
    kos->events = NULL;
    kos->events = malloc(kos->eSize * sizeof(*(kos->events)));
    for(i=0; i<kos->eSize; ++i)
    {
        fgets(line, 34, fp);
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(10);
            fgets(line, 34, fp);
        }
        kos->events[i].name = line;
        line = malloc(10);
        fgets(line, 34, fp);
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(10);
            fgets(line, 34, fp);
        }
        kos->events[i].id = atoi(line);
		line = malloc(100);
        fgets(line, 100, fp);
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(100);
            fgets(line, 100, fp);
        }
		kos->events[i].location = line;
		line = malloc(100);
    }

    kos->guests = NULL;
    kos->guests = malloc(kos->gSize * sizeof(*(kos->guests)));
    line = malloc(34);
    fgets(line, 34, fp);
    for(i=0; i<kos->gSize; ++i)
    {
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(34);
            fgets(line, 34, fp);
        }
        kos->guests[i].name = line;

        line = malloc(34);
        fgets(line, 34, fp);
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(34);
            fgets(line, 34, fp);
        }
        kos->guests[i].mail = line;

        line = malloc(34);
        fgets(line, 34, fp);
        while(line[0] == '\0' || line[0] == '\n')
        {
            line = malloc(34);
            fgets(line, 34, fp);
        }
        kos->guests[i].id = atoi(line);

        line = malloc(34);
        fgets(line, 34, fp);
        while(line[0] == '\0' || line[0] == '\n' || (line[0] < 48 && line[0] > 56))
        {
            line = malloc(34);
            fgets(line, 34, fp);
        }
        kos->guests[i].regTime = line;
        line = malloc(34);
        fgets(line, 34, fp);
    }
    fclose(fp);
}

void saveKos(KingOfStands* kos, char* filename)
{
    FILE* f = fopen(filename, "wt");
    fprintf(f, "%i", kos->gSize);
    fputc('\n', f);
    fprintf(f, "%i", kos->gUsed);
    fputc('\n', f);
    fprintf(f, "%i", kos->eSize);
    fputc('\n', f);
    fprintf(f, "%i", kos->eUsed);
    int i;
    for(i=0; i<kos->eSize; ++i)
    {
        fputc('\n', f);
        fputs(kos->events[i].name, f);
        fputc('\n', f);
        fprintf(f, "%i", kos->events[i].id);
		fputc('\n', f);
		fputs(kos->events[i].location, f);
    }
    for(i=0; i<kos->gSize; ++i)
    {
        fputc('\n', f);
        fputs(kos->guests[i].name, f);
        fputc('\n', f);
        fputs(kos->guests[i].mail, f);
        fputc('\n', f);
        fprintf(f, "%i", kos->guests[i].id);
        fputc('\n', f);
        fputs(kos->guests[i].regTime, f);
    }
    fclose(f);

}

void handler(int signo) {
    if(signo == SIGUSR1)
	{
		printf("Szulo signalt kapott, most kuldi vissza a neveket!\n");
	}
}

struct message { 
     long mtype;
     char mtext [2];
};

struct message2 {
	long mtype;
	char mtext [1024];
};

void sendmessage2(int msg, char* guestname)
{
    struct message m = { 2, "" };
	strcpy(m.mtext, guestname);
    int status;
    status = msgsnd( msg, &m, strlen ( m.mtext ) + 1 , 0 );
    if ( status < 0 )
        perror("msgsnd");
}

void sendmessage( int msg, int rg )
{
	struct message2 m = { 5, "" };
	if(rg == -1)
	{
		srand(time(NULL));
		int r = rand() % 100 + 1;
		char ra[2];
		sprintf(ra,"%ld", r);
		strcpy(m.mtext, ra);
	}
	else
	{
		char ra[2];
		sprintf(ra,"%ld", rg);
		strcpy(m.mtext, ra);
	}
    int status;
    status = msgsnd( msg, &m, strlen ( m.mtext ) + 1 , 0 );
    if ( status < 0 )
        perror("msgsnd");
} 
     
void receivemessage( int msg, char* ret) 
{ 
     struct message m;
     int status; 
     status = msgrcv(msg, &m, 1024, 5, 0 ); 
     if ( status < 0 ) 
          perror("msgsnd");
	  ret[0] = m.mtext[0];
	  ret[1] = m.mtext[1];
}

void receivemessage2(int msg)
{
	struct message2 m;
    int status; 
    status = msgrcv(msg, &m, 1024, 2, 0 ); 
    if ( status < 0 )
        perror("msgsnd");
	 printf("\n%s\n", m.mtext);
}

void makeChild(KingOfStands* kos, int index)
{
	if((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	if(childpid == 0) // child
	{
		close(fd[1]);
		sleep(1);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		char* eventLocation = readbuffer;
		printf("Gyerek megkapta a locationt: %s\n", eventLocation);
		sleep(1);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		char* eventName = readbuffer;
		printf("Gyerek megkapta az esemeny nevet: %s\n", readbuffer);
		int eventid;
		sleep(1);
		nbytes = read(fd[0], &eventid, sizeof(eventid));
		printf("Gyerek megkapta az eventidt is: %i\n", eventid);
		sleep(1);
		kill(parentpid, SIGUSR1);
		int nsize;
		sleep(1);
		nbytes = read(fd[0], &nsize, sizeof(nsize));
		printf("Gyerek megkapta a nevek meretet is: %i\n", nsize);
		char names[nsize][100];
		sleep(1);
		nbytes = read(fd[0], names, sizeof(names));
		int i;
		printf("Gyerek megkapta a neveket");
		for(i=0; i<nsize; ++i)
			printf(", %s ", names[i]);
		printf("\n");
		sleep(1);
		sendmessage(messageQ, -1);
		sleep(1);
		int r;
		int ng = 0;
		for(i=0; i<nsize; ++i)
		{
			r = rand() % 100 + 1;
			sleep(1);
			if(r <= 10)
			{
				ng++;
				sendmessage2(messageQ, names[i]);
			}
		}
		sleep(1);
		sendmessage(messageQ, ng);
		
		close(fd[0]);
		exit(1);
	}
	else //parent
	{
		close(fd[0]);
		sleep(1);
		write(fd[1], kos->events[index].location, (strlen(kos->events[index].location)+1));
		sleep(1);
		write(fd[1], kos->events[index].name, (strlen(kos->events[index].name)+1));
		sleep(1);
		write(fd[1], &(kos->events[index].id), (sizeof(kos->events[index].id)));
		sleep(1);
		
		pause();
		
		int i, counter=0;
		for(i=0; i<kos->gSize; ++i)
		{
			if(kos->guests[i].id == kos->events[index].id)
				counter++;
		}
		char names[counter][100];
		for(i=0; i<kos->gSize; ++i)
		{
			if(kos->guests[i].id == kos->events[index].id)
			{
				strcpy(names[i], kos->guests[i].name);
			}
		}
		sleep(1);
		
		write(fd[1], &(counter), (sizeof(counter)));
		sleep(1);
		write(fd[1], names, (sizeof(names)+1));
		sleep(1);
		char* stand = malloc(2);
		receivemessage(messageQ, stand);
		printf( "A rendezveny levezenyelve! Ennyire volt sikeres a rendezveny:  %c%c%\n", stand[0], stand[1] );
		char* ng = malloc(2);
		receivemessage(messageQ, ng);
		printf("Ennyien nem vettek reszt az esemenyen:%c%c\n", ng[0], ng[1]);
		int guests = atoi(ng);
		printf("Aki nem erkezett meg:\n");
		for(i=0; i<guests; ++i)
		{
			receivemessage2(messageQ);
		}
		sleep(1);
		wait(&status);
		close(fd[1]);
	}
}

int main(int argc, char* argv[])
{
    KingOfStands* kos = &(struct KingOfStands) {.gSize = 0, .gUsed = 0, .eSize = 0, .eUsed = 0, .guests = NULL, .events = NULL, .eIDCount = 0};
    char* ifile;
    char* ofile;
    if(argc >= 2)
    {
        ifile = argv[1];
        ofile = argv[2];
        loadKos(kos, ifile);
    }
    else
    {
        ofile = "output.txt";
    }
	signal(SIGUSR1,handler);
	pid_t parentpid = getpid();
	key = ftok(argv[0],1);
	
    char* input;
    do
    {
        printMenu();
        input = get_line();
        if(*input == '1')
        {
            if(kos->eSize == 0)
                printf("You have to make an event first!\n");
            else
                addGuest(kos);
        }
        else if(*input == '2')
            listGuests(kos);
        else if(*input == '3')
        {
            printf("Enter the name of the guest:");
            char* name = get_line();
            printf("Enter the mail of the guest:");
            char* mail = get_line();
            printf("Enter the eventID of the guest:");
            int eID = atoi(get_line());
            editGuest(kos, name, mail, eID);
        }
        else if(*input == '4')
        {
            printf("Enter the name of the guest:");
            char* name = get_line();
            printf("Enter the mail of the guest:");
            char* mail = get_line();
            deleteGuest(kos, name, mail);
        }
        else if(*input == '5')
            addEvent(kos);
        else if(*input == '6')
            deleteEvent(kos);
		else if(*input == '7')
		{
			printf("Enter the ID of the event:");
			int eID = atoi(get_line());
			int eIDindex = -1;
			int i;
			for(i=0; i<kos->eSize; ++i)
				if(kos->events[i].id == eID)
					eIDindex = i;
			if(eIDindex != -1)
			{
				messageQ = msgget( key, 0600 | IPC_CREAT );
				pipe(fd);
				makeChild(kos, eIDindex);
			}
			else
				printf("There is no event with the ID of %i!\n", eID);
		}
    } while(*input != 'q');
    saveKos(kos, ofile);
    freeKos(kos);
    return 0;
}
