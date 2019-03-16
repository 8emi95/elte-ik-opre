#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int get_menu_choice(void);
int check_file(const char *fileName);
void new_visitor();
int visitor_counter();
void list_visitors();
void change_visitor();
void delete_visitor();
void new_event();
void delete_event();
void send_to_kid();

struct Event
{
    char name[50];
    char id[2];
};

struct Visitor
{
    char name[50];
    char email[25];
    const char *event_id;
};

void main(void)
{
    int p;
    struct Event defaultEvent;
    strcpy(defaultEvent.name, "King of Stands");
    strcpy(defaultEvent.id, "0");
    FILE *fptr = fopen(defaultEvent.id, "w");
    if (fptr != NULL)
    {
        fclose(fptr);
    }
    int choice;
    do
    {
        choice = get_menu_choice();

        switch (choice)
        {
        case 1:
            new_visitor();
            break;
        case 2:
            list_visitors();
            break;
        case 3:
            change_visitor();
            break;
        case 4:
            delete_visitor();
            break;
        case 5:
            new_event();
            break;
        case 6:
            delete_event();
            break;
        case 7:
            send_to_kid();
            break;
        }
    } while (choice != 0);
}

int get_menu_choice(void)
{
    int selection = 0;

    do
    {
        printf("1 - Új vendég hozzáadása");
        printf("\n2 - Vendégek listázása");
        printf("\n3 - Vendég adatainak módositása.");
        printf("\n4 - Vendég törlése.");
        printf("\n5 - Új rendezvény hozzáadása.");
        printf("\n6 - Meglévő rendezvény törlése.");
        printf("\n7 - Rendezvény inditása.(2.bead)");
        printf("\n0 - Kilépés.");
        printf("\nVálasszon egy menüpontot: ");
        scanf("%d", &selection);

    } while ((selection < 0) || (selection > 7));
    return selection;
}

int check_file(const char *fileName)
{
    FILE *fptr = fopen(fileName, "r");
    if (fptr != NULL)
    {
        fclose(fptr);
        return 1;
    }
    return 0;
}

void new_visitor()
{
    char visitor_name[50];
    printf("Adja meg a nevét: ");
    scanf("%s", &visitor_name);

    char email[25];
    printf("Adja meg az email cimét: ");
    scanf("%s", &email);

    char id[2];
    printf("Adja meg a rendezvény sorszámát: ");
    scanf("%s", &id);

    time_t current_time;
    char *c_time_string;

    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (check_file(id) == 1)
    {
        FILE *f = fopen(id, "a");
        fprintf(f, "%s\n", visitor_name);
        fprintf(f, "%s\n", email);
        fprintf(f, "%s", c_time_string);
        fclose(f);
    }
    else
    {
        printf("Nincs ilyen esemény. \n");
    };

    printf("Ön a %d. vendég. \n", visitor_counter(id));
}

int visitor_counter(const char *fname)
{
    FILE *f;
    int nr = 0;
    f = fopen(fname, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }
    char line[50];
    while (!feof(f))
    {
        fgets(line, sizeof(line), f);
        nr++;
    }
    fclose(f);
    return nr / 3;
}

void list_visitors()
{
    char id[2];
    printf("Adja meg a rendezvény sorszámát: ");
    scanf("%s", &id);
    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }
    char line[50];
    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }
    printf("\n");
    fclose(f);
}

void delete_visitor()
{
    char visitor_name[50];
    printf("Adja meg a nevét: ");
    scanf("%s", &visitor_name);
    strcat(visitor_name, "\n");

    char id[2];
    printf("Adja meg a rendezvény sorszámát: ");
    scanf("%s", &id);

    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    FILE *f_new;
    f_new = fopen("temp", "w");
    if (f_new == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    char line1[50];
    char line2[50];
    char line3[50];
    while (!feof(f))
    {
        fgets(line1, sizeof(line1), f);
        fgets(line2, sizeof(line2), f);
        fgets(line3, sizeof(line3), f);
        if (strcmp(line1, visitor_name) != 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s", line1);
            fprintf(f_new, "%s", line2);
            fprintf(f_new, "%s", line3);
        }
    }
    fclose(f);
    remove(id);
    fclose(f_new);
    rename("temp", id);
}

void change_visitor()
{
    char visitor_name[50];
    printf("Adja meg a módosítandó látogató nevét: ");
    scanf("%s", &visitor_name);
    strcat(visitor_name, "\n");

    char id[2];
    printf("Adja meg a rendezvény sorszámát: ");
    scanf("%s", &id);

    char new_visitor_name[50];
    printf("Adja meg az új nevet: ");
    scanf("%s", &new_visitor_name);

    char new_email[25];
    printf("Adja meg az új email cimet: ");
    scanf("%s", &new_email);

    time_t current_time;
    char *c_time_string;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    FILE *f_new;
    f_new = fopen("temp", "w");
    if (f_new == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    char line1[50];
    char line2[50];
    char line3[50];
    while (!feof(f))
    {
        fgets(line1, sizeof(line1), f);
        fgets(line2, sizeof(line2), f);
        fgets(line3, sizeof(line3), f);
        if (strcmp(line1, visitor_name) != 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s", line1);
            fprintf(f_new, "%s", line2);
            fprintf(f_new, "%s", line3);
        }
        if (strcmp(line1, visitor_name) == 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s\n", new_visitor_name);
            fprintf(f_new, "%s\n", new_email);
            fprintf(f_new, "%s", c_time_string);
        }
    }
    fclose(f);
    remove(id);
    fclose(f_new);
    rename("temp", id);
}

void new_event()
{
    printf("Mi legyen a rendezvény sorszáma? ");
    char id[2];
    scanf("%s", &id);
    FILE *f;
    f = fopen(id, "w");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }
}

void delete_event()
{
    printf("Melyik sorszámú eventet törli? ");
    char id[2];
    scanf("%s", &id);
    remove(id);
}

void handler(int x)
{
    x = x;
}
void send_to_kid()
{
    int option;
    char id[2];
    do{
        printf("Adjon meg egy rendezvényazonosítót: ");
        scanf("%s", &id);
        struct sigaction sigact;
        sigact.sa_handler = handler;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = 0;
        sigaction(SIGUSR1, &sigact, NULL);

        char names[10][50];
        char namesOriginal[10][50];
        int i = 0;
        FILE *f;
        int size = 0;
        int missed;
        char missedNames[10][50];

        int fd[2];
        char buff[300];

        f = fopen(id, "r");
        if (f == NULL)
        {
            perror("File opening error\n");
            exit(1);
        }
        char line[50];
        while (fgets(line, sizeof(line), f))
        {
            strcpy(namesOriginal[i], line);
            fgets(line, sizeof(line), f);
            fgets(line, sizeof(line), f);
            ++size;
            ++i;
        }

        int Pipe = pipe(fd);
        if (Pipe < 0)
        {
            perror("Pipe error!");
            exit(-1);
        }

        pid_t child = fork();
        if (child < 0)
        {
            perror("fork() error!");
            exit(-1);
        }

        /* parent process */
        if (0 != child)
        {
            sigset_t sigset;
            sigfillset(&sigset);
            sigdelset(&sigset, SIGUSR1);

            write(fd[1], id, strlen(id));

            if (sigsuspend(&sigset) == -1)
            {
                printf("A partner megérkezett a(z) %s. rendezvényre\n", id);
            };

            write(fd[1], namesOriginal, sizeof namesOriginal);
            sigsuspend(&sigset);

            wait(NULL);

            read(fd[0], missedNames, sizeof missedNames);
            int j;
            for (j = 0; j < size; ++j)
            {
                if (missedNames[j] != ".")
                {
                    printf("Nem volt itt: %s \n", missedNames[j]);
                }
            }
        }
        /* child process */
        else
        {
            memset(buff, 0, strlen(buff));
            read(fd[0], buff, strlen(id));
            kill(getppid(), SIGUSR1);
            printf("A partner megkapta arendezveny azonositojat: %s\n", buff);

            read(fd[0], names, sizeof names);
            kill(getppid(), SIGUSR1);
            printf("Partner: megkaptam a neveket\n");

            srand(time(NULL));
            missed = 0;
            int i;
            for (i = 0; i < size; i++)
            {
                if (rand() % 10 == 0)
                {
                    strcpy(missedNames[i], names[i]);
                }
                else
                {
                    strcpy(missedNames[i], ".");
                }
            }

            write(fd[1], missedNames, sizeof missedNames);
            exit(0);
            /*****************************************************/
        }
        printf("Ha ki akar lépni nyomja meg a 0-t. Vagy adjon meg másik rendezvény azonositot. ");
        scanf("%d", &option);
    }
    while(option != 0);
}