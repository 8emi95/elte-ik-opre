#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <string.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h> //rand

#define CSERE(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#define FREEMTR(m, b) do {int i; for(i = 0; i < b; ++i) { free(m[i]);}free(m);} while (0)

struct participant {
    char* name;
    char* email;
    int number;
    char* joinTime;
    char* partyID;
};

struct participant_array {
    int size;
    int max;
    struct participant* participants;
};

struct party_array {
    int size;
    char** partyIDs;
};


void init_function(struct participant_array* p, struct party_array* e)
{
    e->size   = 0;
    e->partyIDs = 0;
    p->size   = 0;
    p->max = 0;
    p->participants = 0;
}

const char * menu_options[8] =
{
    "Resztvevo jelentkezes",
    "Resztvevok listazasa",
    "Resztvevo modositasa",
    "Resztvevo torlese",
    "Esemeny letrehozasa",
    "Esemeny torlese",
	"Esemeny inditasa",
    "Kilepes (adatok mentese)"
};

int input()
{
    printf("Lehetosegek:\n");
    int i;
    for(i = 0; i < sizeof(menu_options) / sizeof(menu_options[0]); ++i)
    {
        printf("\t%i. %s\n", i + 1, menu_options[i]);
    }
    char string[256];
    gets(string);
    return atoi(string);
}

int add_participant(struct participant_array* p, struct party_array* e, struct participant per)
{
    struct participant* new_p = (struct participant*)malloc(sizeof(struct participant)*(p->size + 1));

    int j = 0;

    while (j < e->size && strcmp(e->partyIDs[j], per.partyID))
    {
      ++j;
    }

    if(j == e->size)
    {
      return 1;
    }

    for(j = 0; j < p->size; ++j)
    {
        new_p[j] = p->participants[j];
    }
    per.number = p->max;
    new_p[j] = per;
    CSERE( struct participant*, new_p, p->participants);
    free(new_p);

    ++(p->max);
    ++(p->size);
    return 0;
}


void delete_participant(struct participant_array* p, char* name)
{
    printf("\t1\n");
    int i = 0, j;
    while (i < p->size && strcmp(name, p->participants[i].name))
    {
      ++i;
    }
    if (i == p->size)
    {
        printf("Nincs ilyen nevu vendeg!\n");
        return;
    }
    printf("\t2\n");

    struct participant* new_p = (struct participant*)malloc(sizeof(struct participant)*(p->size - 1));

    j = 0;

    while (j < i)
    {
        new_p[j] = p->participants[j];
        ++j;
    }
    printf("\t3\n");
    while(j + 1 < p->size)
    {
      new_p[j] = p->participants[j + 1];
      ++j;
    }
    printf("\t4\n");


    CSERE( struct participant*, new_p, p->participants);
    free(new_p);

    --(p->size);
    printf("%s nevu vendeg torolve!\n", name);
    return;
}

void modify_guest(struct participant_array* p, struct party_array* e, char* name)
{
    int i = 0, j;
    while (i < p->size && strcmp(name, p->participants[i].name))
    {
      ++i;
    }
    if (i == p->size)
    {
        printf("Nincs ilyen nevu vendeg!\n");
        return;
    }
    else
    {
        char buffer[256];
        printf("E-mail: ");
        gets(buffer);
        p->participants[i].email = (char*)malloc(strlen(buffer) + 1);
        strcpy(p->participants[i].email, buffer);

        printf("Esemeny azonosito: %s\n szeretned modositani? y/n:\n", p->participants[i].partyID);
        gets(buffer);
        j = 0;
        while (j < e->size && strcmp(e->partyIDs[j], buffer))
        {
          ++j;
        }
        if(j == e->size)
        {
          printf("Nincs ilyen azonositoju esemeny!\n");
          return;
        }
        p->participants[i].partyID = (char*)malloc(strlen(buffer) + 1);
        strcpy(p->participants[i].partyID, buffer);
    }
    printf("%s nevu vendeg modositva!\n", name);
}

void list_guests(struct participant_array* p)
{
    int i;
    printf("Vendegek:\n");
    for(i = 0; i < p->size; ++i)
    {
        printf("\t%i. szamu vendeg:\n\t\tnev: %s\n\t\temail: %s\n\t\tcsatlakozasi ido: %s\n\t\tesemeny azonositoja: %s\n\n",
          p->participants[i].number + 1, p->participants[i].name, p->participants[i].email, p->participants[i].joinTime, p->participants[i].partyID);
    }
    printf("\n");
}

void add_party(struct party_array* e, char* partyID)
{
    int i;

    char** new_IDs = (char**)malloc((e->size + 1)*sizeof(char*));

    for(i = 0; i < e->size; ++i)
    {
        new_IDs[i] = e->partyIDs[i];
    }

    new_IDs[i] = partyID;

    CSERE(char**, e->partyIDs, new_IDs);
    free(new_IDs);

    ++(e->size);
    printf("A(z) %s nevu esemeny hozzaadva!\n", partyID);
}

int delete_party(struct participant_array* p, struct party_array* e, char* partyID)
{
    int i = 0, j = 0;
    char* temp;

    char** new_IDs = (char**)malloc((e->size - 1)*sizeof(char*));

    while(i < e->size && strcmp(e->partyIDs[i], partyID))
    {
        i++;
    }
    if(i == e->size)
    {
        printf("Nincs ilyen azonositoju esemeny!\n");
        return 1;
    }
    else
    {
        for(j = i; j < e->size - 1; ++j)
        {
            e->partyIDs[j] = e->partyIDs[j + 1];
        }
        for(i = 0; i < e->size - 1; ++i)
        {
            new_IDs[i] = e->partyIDs[i];
        }

        CSERE(char**, e->partyIDs, new_IDs);
        free(new_IDs);

        --(e->size);

        for(j = 0; j < p->size; ++j)
        {
            if(!strcmp(p->participants[j].partyID, partyID))
            {
                temp = p->participants[j].name;
                delete_participant(p, temp);
                --j;
            }
        }

        printf("A(z) %s nevu esemeny torolve!\n", partyID);

        return 0;
    }
}

void save_to_file(struct participant_array* p, struct party_array* e)
{
    FILE* f = fopen("kog_data", "w");
    int i;
    fprintf(f, "%i\n%i\n%i\n", p->size, p->max, e->size);
    for(i = 0; i < p->size; ++i)
    {
        fprintf(f, "%s\n%s\n%i\n%s\n%s\n", p->participants[i].name, p->participants[i].email, p->participants[i].number, p->participants[i].joinTime, p->participants[i].partyID);
    }
    for(i = 0; i < e->size; ++i)
    {
        fprintf(f, "%s\n", e->partyIDs[i]);
    }
    fclose(f);
}

void open_from_file(struct participant_array* p, struct party_array* e)
{
    FILE* f = fopen("kog_data", "r");
    if(!f || feof(f))
    {
        init_function(p, e);
        return;
    }
    char temp[256];
    int i, j, n;

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    p->size = n;
    p->participants = (struct participant*)malloc(n * sizeof(struct participant));

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    p->max = n;

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    e->size = n;
    e->partyIDs = (char**)malloc(n * sizeof(char*));

    for(i = 0; i < p->size ; ++i)
    {
        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->participants[i].name = (char *)malloc(strlen(temp) + 1);
        strcpy(p->participants[i].name, temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->participants[i].email = (char *)malloc(strlen(temp) + 1);
        strcpy(p->participants[i].email, temp);

        fgets(temp, sizeof(temp), f);
        p->participants[i].number = atoi(temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->participants[i].joinTime = (char *)malloc(strlen(temp) + 1);
        strcpy(p->participants[i].joinTime, temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->participants[i].partyID = (char *)malloc(strlen(temp) + 1);
        strcpy(p->participants[i].partyID, temp);
    }

    for(i = 0; i < e->size; ++i)
    {
        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        e->partyIDs[i] = (char *)malloc(strlen(temp) + 1);
        strcpy(e->partyIDs[i], temp);
    }
    fclose(f);
}

void handler(int snumber) {};

void send_participants(struct participant_array* p, struct party_array* e, char* party)
{
    signal(SIGTERM, handler);
    char* mqname="/s8h1db";
    int pipeid = mkfifo("S8H1DB_cso",0600);
    pipeid = open("S8H1DB_cso",O_RDWR);
    struct mq_attr attr;
    mqd_t mq1;
    attr.mq_maxmsg=10; //MAXMSGS
    attr.mq_msgsize=sizeof(int); //MSGSIZE
    mq_unlink(mqname); // delete if exist
    mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    if ( mq1 < 0 ) { 
        printf("badmqueue: %s",strerror(errno)); 
        return;
    }  
    if (pipeid < 0)
    {
        printf("badpipe\n");
        return;
    }
    pid_t pid = fork();
    if (pid < 0 )
    {
        printf("badfork\n");
        return;
    }
    else if (pid == 0) //child
    {
        srand(time(NULL));
        int num, i;
        int score = 0;
        char buffer[500];
        char hely[500];
        read(pipeid,&hely,sizeof(hely));
        kill(getppid(), SIGTERM);
        read(pipeid,&num,sizeof(num));
        for(i = 0; i < num; ++i)
        {
            read(pipeid,buffer,sizeof(buffer));
            printf(buffer);
            printf("\n");
            int temp = rand()%10 + 1;
            mq_send(mq1,(char *)&temp,sizeof(temp),1);
        }
        while(1)
        {
            sleep(1);
        }
        exit(1);
    }
    else //parent
    {
        char buffer[500];
        int index = 0;
        while (index < e->size && strcmp(e->partyIDs[index], party))
        {
            printf(".");
            ++index;
        }
        printf("\n");
        if(index == e->size)
        {
            printf("Nincs ilyen azonositoju esemeny.\n");
            kill(pid, SIGKILL);
            return;
        }
        int i;
        char * helyszin = "helyszin";
        write(pipeid, helyszin, sizeof(helyszin));
        pause();
        int allparticipants = 0;
        int index2 = 0;
        while(index2 < p->size) {
            if (!strcmp(p->participants[index2].partyID, party)) {
                ++allparticipants;
            }
            ++index2;
        }
        write(pipeid,&allparticipants,sizeof(allparticipants));
        index2 = 0;
        while(index2 < allparticipants) {
            if (!strcmp(p->participants[index2].partyID, party)) {
                strcpy(buffer,p->participants[index2].name);
                write(pipeid,buffer, sizeof(buffer));       
            }
            ++index2;
        }
        int participants = 0;
        for(i = 0; i < allparticipants; ++i)
        {
            int k;
            mq_receive(mq1,(char *)&k,sizeof(int),0);
            if (k <= 9) {++participants;}
        }
        unlink("S8H1DB_cso");
        mq_close(mq1);
        mq_unlink(mqname);
        printf("Reszveteli arany: %i / %i ", participants, allparticipants);
        delete_party(p, e, party);
        int status = 0;
        char evbuffer [500];
        index2 = 0;
        while(index2 < e->size && status == 0) {
            if (strcmp(e->partyIDs[index2], party)) {
                printf("A kovetkezo esemeny: ");
                printf(e->partyIDs[index2]);
                printf("\n");
                status = 1;
            }
            ++index2;
        }
        if (status == 0) {
            printf("Nincs tobb party\n");
        }
    }
}

void menu(struct participant_array* p, struct party_array* e)
{
    char temp[256], temp2[256];
    char *p1;
    struct participant pp;
    time_t t;
    while(1)
    {
        switch (input())
        {
        case 1: //uj vendeg jelentkezese
            t = time(NULL);
            printf("Nev: ");
            gets(temp);
            pp.name = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.name, temp);

            printf("E-mail: ");
            gets(temp);
            pp.email = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.email, temp);

            pp.joinTime = (char*)malloc(strlen(ctime(&t)) + 1);
            printf("(%s)\n", ctime(&t));
            strcpy(pp.joinTime, ctime(&t));
            pp.joinTime[strcspn(pp.joinTime, "\n")] = 0;

            printf("Esemeny azonosito: ");
            gets(temp);
            pp.partyID = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.partyID, temp);

            if(add_participant(p, e, pp))
            {
              printf("Nincs ilyen azonositoju esemeny.\n");
            }
            else
            {
                printf("%s nevu vendeg feljelentkezett a(z) %s azonositoju esemenyre!\n", pp.name, temp);
            }
            break;
        case 2: //vendegek listazasa
            list_guests(p);
            break;
        case 3: //vendeg modositasa
            printf("Nev: ");
            gets(temp);

            modify_guest(p, e, temp);
            break;
        case 4: //vendeg torlese
            printf("Nev: ");
            gets(temp);
            delete_participant(p, temp);
            break;
        case 5: //esemeny letrehozasa
            printf("Esemeny azonosito: ");
            gets(temp);
            p1 = (char*)malloc(strlen(temp) + 1);
            strcpy(p1, temp);

            add_party(e, p1);
            break;
        case 6: //esemeny torlese
            printf("Esemeny azonosito: ");
            gets(temp);

            delete_party(p, e, temp);
            break;
		case 7:
            printf("Esemeny azonosito: ");
            gets(temp);
			
            send_participants(p, e, temp);
            break;
        case 8: //kilepes
            printf("Kilepes...\n");
            save_to_file(p, e);
            exit(0);
        default:
            printf("Ismeretlen parancs");
        }
        printf("\n");
    }
}
int main()
{
    struct participant_array p;
    struct party_array e;
    open_from_file(&p, &e);
    menu(&p, &e);
    return 0;
}
