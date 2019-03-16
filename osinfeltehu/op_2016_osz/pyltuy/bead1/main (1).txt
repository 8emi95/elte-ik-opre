#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
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

#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#define FREEMTR(m, b) do {int i; for(i = 0; i < b; ++i) { free(m[i]);}free(m);} while (0)

struct person {
    char* name;
    char* date;
    char* number;
};

struct stat_vector {
    int size;
    struct person** data;
    char** stations;
    char** depart;
    int* travelers;
};

void initialize(struct stat_vector* s)
{
    s->size = 0;
    s->data = 0;
    s->stations = 0;
    s->travelers = 0;
}

void data_write(struct stat_vector* s)
{
    int i, j;
    printf("%i allomas:\n", s->size);
    for(i = 0; i < s->size; ++i)
    {
        printf("%s allomas (indulas: %s, %i fovel):\n", s->stations[i], s->depart[i], s->travelers[i]);
        for(j = 0; j < s->travelers[i]; ++j)
        {
            printf("\t%s %s %s\n", s->data[i][j].name, s->data[i][j].date, s->data[i][j].number);
        }
    }
    printf("\n--Allomasok vege--\n");
}

void stat_write(struct stat_vector* s, char* station)
{
    int i, j;
    for(i = 0; i < s->size; ++i)
    {
        if(!strcmp(s->stations[i], station))
        {
            printf("indulas: %s, %i fovel\n", s->depart[i], s->travelers[i]);
            for(j = 0; j < s->travelers[i]; ++j)
            {
                printf("\t%s %s %s\n", s->data[i][j].name, s->data[i][j].date, s->data[i][j].number);
            }
            return;
        }
    }
    printf("Nem talalhato allomas!\n");
}

void add_station(struct stat_vector* s, char* name, char* date)
{
    int i;
    for(i = 0; i < s->size; ++i)
    {
        if(!strcmp(s->stations[i], name))
        {
            return;
        }
    }

    struct person** data = (struct person**)malloc((s->size + 1)*sizeof(char*));
    char** stations = (char**)malloc((s->size + 1) *sizeof(char*));
    char** depart = (char**)malloc((s->size + 1) *sizeof(char*));
    int* travelers = (int*)malloc((s->size + 1)*sizeof(int));

    for(i = 0; i < s->size; ++i)
    {
        stations[i] = s->stations[i];
        travelers[i] = s->travelers[i];
        depart[i] = s->depart[i];
        data[i] = s->data[i];
    }
    stations[i] = name;
    travelers[i] = 0;
    depart[i] = date;
    data[i] = 0;

    SWAP(char**, stations, s->stations);
    SWAP(struct person **, data, s->data);
    SWAP(int*,travelers, s->travelers);
    SWAP(char**, depart, s->depart);
    free(data);
    free(stations);
    ++(s->size);
    free(travelers);
    free(depart);
}

void remove_station(struct stat_vector* s, char* name)
{
    int i = 0;
    while (i < s->size && strcmp(s->stations[i], name))
    {
        ++i;
    }
    if(i == s->size)
    {
        return;
    }
    int to_delete = i;
    struct person** data = (struct person**)malloc((s->size - 1)*sizeof(char*));
    char** stations = (char**)malloc((s->size - 1) *sizeof(char*));
    char** depart = (char**)malloc((s->size - 1) *sizeof(char*));
    int* travelers = (int*)malloc((s->size - 1)*sizeof(int));
    i = 0;
    while(i < to_delete)
    {
        stations[i] = s->stations[i];
        travelers[i] = s->travelers[i];
        depart[i] = s->depart[i];
        data[i] = s->data[i];
        ++i;
    }
    free(s->stations[i]);
    free(s->depart[i]);
    free(s->data[i]);
    ++i;
    while( i < s->size )
    {
        stations[i - 1] = s->stations[i];
        travelers[i - 1] = s->travelers[i];
        depart[i - 1] = s->depart[i];
        data[i - 1] = s->data[i];
        ++i;
    }
    SWAP(char**, stations, s->stations);
    SWAP(struct person **, data, s->data);
    SWAP(int*,travelers, s->travelers);
    SWAP(char**, depart, s->depart);
    free(data);
    free(stations);
    free(depart);
    --(s->size);
    free(travelers);
}

int add_person(struct stat_vector* s, char* station, struct person per)
{
    int i = 0, place;
    while( i < s->size && strcmp(station, s->stations[i]))
    {
        ++i;
    }
    if( i == s->size)
    {
        return 1;
    }
    place = i;
    struct person* p = (struct person*)malloc(sizeof( struct person)*(s->travelers[place] + 1));
    for(i = 0; i < s->travelers[place]; ++i)
    {
        p[i] = s->data[place][i];
    }
    p[i] = per;
    SWAP( struct person*, p, s->data[place]);
    free(p);
    ++s->travelers[place];
    return 0;
}

int remove_person(struct stat_vector* s, char* station, char* name)
{
    int i = 0, place;
    while( i < s->size && strcmp(station, s->stations[i]))
    {
        ++i;
    }
    if( i == s->size)
    {
        return 1;
    }
    place = i;
    i = 0;
    int person_place;
    while(i < s->travelers[place] && strcmp(s->data[place][i].name, name))
    {
        ++i;
    }
    if( i == s->travelers[place])
    {
        return 2;
    }
    person_place = i;
    struct person* p = (struct person*)malloc(sizeof( struct person)*(s->travelers[place] - 1));
    i = 0;
    while(i < person_place)
    {
        p[i] = s->data[place][i];
        ++i;
    }
    ++i;
    while(i < s->travelers[place])
    {
        p[i - 1] = s->data[place][i];
    }
    SWAP( struct person*, p, s->data[place]);
    free(p);
    --s->travelers[place];
    return 0;

}

const char * menu_options[7] =
{
    "allomasok kiirasa",
    "jarat torlese",
    "jarat hozzaadasa",
    "jelentkezo hozzaadasa",
    "jelentkezo torlese",
    "kilepes (adatok mentese)",
    "jarat inditasa"
};

int initiate()
{
    return 0;
}

int get_input()
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

void save_data(struct stat_vector* s)
{    FILE* f = fopen("main_data", "w");
    int i;
    fprintf(f, "%i\n", s->size);
    for(i = 0; i < s->size; ++i)
    {
        fprintf(f, "%s\n%s\n%i\n", s->stations[i],s->depart[i], s->travelers[i]);
        int j;
        for(j = 0; j < s->travelers[i]; ++j)
        {
            fprintf(f, "%s\n%s\n%s\n", s->data[i][j].name, s->data[i][j].date, s->data[i][j].number);
        }
    }
    fclose(f);
}

void open_data(struct stat_vector* s)
{
    FILE* f = fopen("main_data", "r");
    if(!f || feof(f))
    {
        initialize(s);
        return;
    }
    char temp[256];
    int i, j, n;

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    s->size = n;
    s->stations = (char**)malloc(n * sizeof(char*));
    s->data = (struct person**)malloc(n * sizeof(struct person*));
    s->depart = (char**)malloc(n * sizeof(char*));
    s->travelers = (int*)malloc(n * sizeof(int));

    for(i = 0; i < n ; ++i)
    {
        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        s->stations[i] = (char *)malloc(strlen(temp) + 1);
        strcpy(s->stations[i], temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        s->depart[i] = (char *)malloc(strlen(temp) + 1);
        strcpy(s->depart[i], temp);

        fgets(temp, sizeof(temp), f);
        s->travelers[i] = atoi(temp);

        s->data[i] = (struct person*)malloc(s->travelers[i]*sizeof(struct person));
        for(j = 0; j < s->travelers[i]; ++j)
        {
            fgets(temp, sizeof(temp), f);
            temp[strcspn(temp, "\n")] = 0;
            s->data[i][j].name = (char *)malloc(strlen(temp) + 1);
            strcpy(s->data[i][j].name, temp);

            fgets(temp, sizeof(temp), f);
            temp[strcspn(temp, "\n")] = 0;
            s->data[i][j].date = (char *)malloc(strlen(temp) + 1);
            strcpy(s->data[i][j].date, temp);

            fgets(temp, sizeof(temp), f);
            s->data[i][j].number = (char *)malloc(strlen(temp) + 1);
            temp[strcspn(temp, "\n")] = 0;
            strcpy(s->data[i][j].number, temp);
        }
    }
    fclose(f);
}

void send_people(struct stat_vector* s, char* station)
{
    char* mqname="/zbwdfag";
    int pipeid = mkfifo("ZBWDAG_pipe",0600);
    pipeid = open("ZBWDAG_pipe",O_RDWR);
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
        read(pipeid,&num,sizeof(num));
        for(i = 0; i < num; ++i)
        {
            read(pipeid,buffer,sizeof(buffer));
            int temp = rand()%5 + 1;
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
        int ind = 0;//select station index
        while (ind < s->size && strcmp(s->stations[ind], station))
        {
            printf(".");
            ++ind;
        }
        printf("\n");
        if(ind == s->size)
        {
            printf("not found\n");
            kill(pid, SIGKILL);
            return;
        }
        int i;
        write(pipeid,&s->travelers[ind],sizeof(s->travelers[ind]));
        for(i = 0; i < s->travelers[ind]; ++i)
        {
            strcpy(buffer,s->data[ind][i].name);
            write(pipeid,buffer, sizeof(buffer));
        }
        int score = 0;
        for(i = 0; i < s->travelers[ind]; ++i)
        {
            int k;
            mq_receive(mq1,(char *)&k,sizeof(int),0);
            score += k;
        }
        unlink("ZBWDAG_pipe");
        mq_close(mq1);
        mq_unlink(mqname);
        printf("atlagos pontszam: %f", (float)score / s->travelers[ind]);
    }
}

void loop(struct stat_vector* s)
{
    char temp[256], temp2[256];
    size_t size = 256;
    char *p1, *p2;
    struct person p;
    time_t t;
    while(1)
    {
        switch (get_input())
        {
        case 1:
            printf("allomas:");
            gets(temp);
            stat_write(s, temp);
            break;
        case 2:
            printf("allomas: ");
            gets(temp);
            remove_station(s, temp);
            break;
        case 3:
            printf("allomas:" );
            gets(temp);
            p1 = (char*)malloc(strlen(temp) + 1);
            strcpy(p1, temp);

            printf("indulas: ");
            gets(temp);
            p2 = (char*)malloc(strlen(temp) + 1);
            strcpy(p2, temp);

            add_station(s, p1, p2);
            p1 = 0;
            p2 = 0;
            break;
        case 4:
            t = time(NULL);
            printf("nev: ");
            gets(temp);
            p.name = (char*)malloc(strlen(temp) + 1);
            strcpy(p.name, temp);

            p.date = (char*)malloc(strlen(ctime(&t)) + 1);
            printf("(%s)\n", ctime(&t));
            strcpy(p.date, ctime(&t));
            p.date[strcspn(p.date, "\n")] = 0;

            printf("telefonszam: ");
            gets(temp);
            p.number = (char*)malloc(strlen(temp) + 1);
            strcpy(p.number, temp);
            printf("allomas: ");
            gets(temp);
            if(add_person(s,temp ,p))
            {
                printf("Nem talalhato az allomas.");
            }
            break;
        case 5:
            printf("nev: ");
            gets(temp);
            printf("allomas: ");
            gets(temp2);
            remove_person(s, temp2, temp);
            break;
        case 6:
            printf("Kilepes...\n");
            save_data(s);
            clean(s);
            exit(EXIT_SUCCESS);
        case 7:
            printf("jarat: ");
            gets(temp);
            send_people(s,temp);
            break;
        default:
            //data_write(s);
            printf("Ismeretlen parancs");
        }
        printf("\n");
    }
}

clean(struct stat_vector* s)
{
    FREEMTR(s->stations, s->size);
    FREEMTR(s->data, s->size);
    free(s->travelers);
}

int main()
{
    srand(time(NULL));
    struct stat_vector s;
    //initialize(&s);
    open_data(&s);
    printf("\n---\n");
    data_write(&s);
    printf("\n---\n");
    loop(&s);
    return 0;
}
