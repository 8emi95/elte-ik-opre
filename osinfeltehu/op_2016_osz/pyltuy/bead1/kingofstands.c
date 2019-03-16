#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>   //stat
#include <time.h> //ctime
#include <string.h>

#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#define FREEMTR(m, b) do {int i; for(i = 0; i < b; ++i) { free(m[i]);}free(m);} while (0)

struct person {
    char* name;
    char* email;
    int number;
    char* joinTime;
    char* eventID;
};

struct person_vector {
    int size;
    int max;
    struct person* people;
};

struct event_vector {
    int size;
    char** eventIDs;
};


void initialize(struct person_vector* p, struct event_vector* e)
{
    e->size   = 0;
    e->eventIDs = 0;
    p->size   = 0;
    p->max = 0;
    p->people = 0;
}

const char * menu_options[7] =
{
    "vendeg jelentkezes",
    "vendegek listazasa",
    "vendeg modositasa",
    "vendeg torlese",
    "esemeny letrehozasa",
    "esemeny torlese",
    "kilepes (adatok mentese)"
};

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

int add_person(struct person_vector* p, struct event_vector* e, struct person per)
{
    struct person* new_p = (struct person*)malloc(sizeof(struct person)*(p->size + 1));

    int j = 0;

    while (j < e->size && strcmp(e->eventIDs[j], per.eventID))
    {
      ++j;
    }

    if(j == e->size)
    {
      return 1;
    }

    for(j = 0; j < p->size; ++j)
    {
        new_p[j] = p->people[j];
    }
    per.number = p->max;
    new_p[j] = per;
    SWAP( struct person*, new_p, p->people);
    free(new_p);

    ++(p->max);
    ++(p->size);
    return 0;
}


void delete_person(struct person_vector* p, char* name)
{
    printf("\t1\n");
    int i = 0, j;
    while (i < p->size && strcmp(name, p->people[i].name))
    {
      ++i;
    }
    if (i == p->size)
    {
        printf("Nincs ilyen nevu vendeg!\n");
        return;
    }
    printf("\t2\n");

    struct person* new_p = (struct person*)malloc(sizeof(struct person)*(p->size - 1));

    j = 0;

    while (j < i)
    {
        new_p[j] = p->people[j];
        ++j;
    }
    printf("\t3\n");
    while(j + 1 < p->size)
    {
      new_p[j] = p->people[j + 1];
      ++j;
    }
    printf("\t4\n");


    SWAP( struct person*, new_p, p->people);
    free(new_p);

    --(p->size);
    printf("%s nevu vendeg torolve!\n", name);
    return;
}

void modify_guest(struct person_vector* p, struct event_vector* e, char* name)
{
    int i = 0, j;
    while (i < p->size && strcmp(name, p->people[i].name))
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
        printf("email: %s\n szeretned modositani? y/n:\n", p->people[i].email);
        gets(buffer);
        if(!strcmp(buffer, "y"))
        {
          printf("uj ertek: ");
          gets(buffer);
          p->people[i].email = (char*)malloc(strlen(buffer) + 1);
          strcpy(p->people[i].email, buffer);
        }

        printf("esemeny nev: %s\n szeretned modositani? y/n:\n", p->people[i].eventID);
        gets(buffer);
        if(!strcmp(buffer, "y"))
        {
          printf("uj ertek: ");
          gets(buffer);

          j = 0;
          while (j < e->size && strcmp(e->eventIDs[j], buffer))
          {
            ++j;
          }
          if(j == e->size)
          {
            printf("Nincs ilyen nevu esemeny!\n");
            return;
          }
          p->people[i].eventID = (char*)malloc(strlen(buffer) + 1);
          strcpy(p->people[i].eventID, buffer);
        }
    }
    printf("%s nevu vendeg modositva!\n", name);
}

void list_guests(struct person_vector* p)
{
    int i;
    printf("Vendegek:\n");
    for(i = 0; i < p->size; ++i)
    {
        printf("\t%i. szamu vendeg:\n\t\tnev: %s\n\t\temail: %s\n\t\tcsatlakozasi ido: %s\n\t\tesemeny neve: %s\n\n",
          p->people[i].number + 1, p->people[i].name, p->people[i].email, p->people[i].joinTime, p->people[i].eventID);
    }
    printf("\n");
}

void add_event(struct event_vector* e, char* eventID)
{
    int i;

    char** new_IDs = (char**)malloc((e->size + 1)*sizeof(char*));

    for(i = 0; i < e->size; ++i)
    {
        new_IDs[i] = e->eventIDs[i];
    }

    new_IDs[i] = eventID;

    SWAP(char**, e->eventIDs, new_IDs);
    free(new_IDs);

    ++(e->size);
    printf("A(z) %s nevu esemeny hozzaadva!\n", eventID);
}

int delete_event(struct person_vector* p, struct event_vector* e, char* eventID)
{
    int i = 0, j = 0;
    char* temp;

    char** new_IDs = (char**)malloc((e->size - 1)*sizeof(char*));

    while(i < e->size && strcmp(e->eventIDs[i], eventID))
    {
        i++;
    }
    if(i == e->size)
    {
        printf("Nincs ilyen nevu esemeny!\n");
        return 1;
    }
    else
    {
        for(j = i; j < e->size - 1; ++j)
        {
            e->eventIDs[j] = e->eventIDs[j + 1];
        }
        for(i = 0; i < e->size - 1; ++i)
        {
            new_IDs[i] = e->eventIDs[i];
        }

        SWAP(char**, e->eventIDs, new_IDs);
        free(new_IDs);

        --(e->size);

        for(j = 0; j < p->size; ++j)
        {
            if(!strcmp(p->people[j].eventID, eventID))
            {
                temp = p->people[j].name;
                delete_person(p, temp);
                --j;
            }
        }

        printf("A(z) %s nevu esemeny torolve!\n", eventID);

        return 0;
    }
}

void save_data(struct person_vector* p, struct event_vector* e)
{
    FILE* f = fopen("kings_data", "w");
    int i;
    fprintf(f, "%i\n%i\n%i\n", p->size, p->max, e->size);
    for(i = 0; i < p->size; ++i)
    {
        fprintf(f, "%s\n%s\n%i\n%s\n%s\n", p->people[i].name, p->people[i].email, p->people[i].number, p->people[i].joinTime, p->people[i].eventID);
    }
    for(i = 0; i < e->size; ++i)
    {
        fprintf(f, "%s\n", e->eventIDs[i]);
    }
    fclose(f);
}

void open_data(struct person_vector* p, struct event_vector* e)
{
    FILE* f = fopen("kings_data", "r");
    if(!f || feof(f))
    {
        initialize(p, e);
        return;
    }
    char temp[256];
    int i, j, n;

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    p->size = n;
    p->people = (struct person*)malloc(n * sizeof(struct person));

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    p->max = n;

    fgets(temp, sizeof(temp), f);
    n = atoi(temp);
    e->size = n;
    e->eventIDs = (char**)malloc(n * sizeof(char*));

    for(i = 0; i < p->size ; ++i)
    {
        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->people[i].name = (char *)malloc(strlen(temp) + 1);
        strcpy(p->people[i].name, temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->people[i].email = (char *)malloc(strlen(temp) + 1);
        strcpy(p->people[i].email, temp);

        fgets(temp, sizeof(temp), f);
        p->people[i].number = atoi(temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->people[i].joinTime = (char *)malloc(strlen(temp) + 1);
        strcpy(p->people[i].joinTime, temp);

        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        p->people[i].eventID = (char *)malloc(strlen(temp) + 1);
        strcpy(p->people[i].eventID, temp);
    }

    for(i = 0; i < e->size; ++i)
    {
        fgets(temp, sizeof(temp),f);
        temp[strcspn(temp, "\n")] = 0;
        e->eventIDs[i] = (char *)malloc(strlen(temp) + 1);
        strcpy(e->eventIDs[i], temp);
    }
    fclose(f);
}
void loop(struct person_vector* p, struct event_vector* e)
{
    char temp[256], temp2[256];
    char *p1;
    struct person pp;
    time_t t;
    while(1)
    {
        switch (get_input())
        {
        case 1: //uj vendeg jelentkezese
            t = time(NULL);
            printf("nev: ");
            gets(temp);
            pp.name = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.name, temp);

            printf("email: ");
            gets(temp);
            pp.email = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.email, temp);

            pp.joinTime = (char*)malloc(strlen(ctime(&t)) + 1);
            printf("(%s)\n", ctime(&t));
            strcpy(pp.joinTime, ctime(&t));
            pp.joinTime[strcspn(pp.joinTime, "\n")] = 0;

            printf("esemeny neve: ");
            gets(temp);
            pp.eventID = (char*)malloc(strlen(temp) + 1);
            strcpy(pp.eventID, temp);

            if(add_person(p, e, pp))
            {
              printf("Nincs ilyen nevu esemeny.\n");
            }
            else
            {
                printf("%s nevu vendeg feljelentkezett a(z) %s nevu esemenyre!\n", pp.name, temp);
            }
            break;
        case 2: //vendegek listazasa
            list_guests(p);
            break;
        case 3: //vendeg modositasa
            printf("nev: ");
            gets(temp);

            modify_guest(p, e, temp);
            break;
        case 4: //vendeg torlese
            printf("nev: ");
            gets(temp);
            delete_person(p, temp);
            break;
        case 5: //esemeny letrehozasa
            printf("esemeny neve: ");
            gets(temp);
            p1 = (char*)malloc(strlen(temp) + 1);
            strcpy(p1, temp);

            add_event(e, p1);
            break;
        case 6: //esemeny torlese
            printf("esemeny neve: ");
            gets(temp);

            delete_event(p, e, temp);
            break;
        case 7: //kilepes
            printf("Kilepes...");
            save_data(p, e);
            exit(0);
        default:
            printf("Ismeretlen parancs");
        }
        printf("\n");
    }
}
int main()
{
    struct person_vector p;
    struct event_vector e;
    open_data(&p, &e);
    loop(&p, &e);
    return 0;
}
