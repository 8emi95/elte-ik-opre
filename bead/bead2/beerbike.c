/*
1.
Egy sörbicikli túrához szervezzük a jelentkezőket!

új jelentkező adatai
+ név
+ email
+ telefonszám
+ útvonal (3: Parlament, Hősök tere, Vár)
+ hány fő nevében jelentkezik
+ auto felvételi dátum (év, hó, nap, óra, perc, másodperc):
- rögzítés
- módosítás
- törlés

listázni:
- teljes névsort
- egyes útvonalakra jelentkezetteket

- Az adatokat fájlban tároljuk.
- Minden hozott vendég 15% kedvezményt kap!

2. Sör-Kocsi Budapest társaság (SKB) = szülő; Sör-Bike-Driver (SBD) = gyerek
ha egy sörtúra nyereségesen indítható (van elég jelentkező):
- SKB: munkába állít egy SBD-t
- SBD: megfelelő mennyiségű sört kezd vételezni
- SBD->SKB: jelzés h (SBD) kész a vételezésre
- SKB->SBD: utaslista, részvételi díj / fő, túraútvonal (pipe)
- SBD: kiírja a képernyőre az adatokat, résztvevőnként 5 liter sört vételez, beszedi a jelentkezési díjakat
- SBD->SKB: vételezett sör mennyisége, a beszedett díj összege (pipe)
*túra véget ér (kis idő múlva, véletlen szám, sleep, usleep)*
- SBD->SKB: a feladatot elvégezte, kész a következő túrára
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SIZE 1024
#define MIN_APPLICANTS 6
#define ENTRY_SIZE 250
#define PRICE_PER_LITER 3000

struct trip_start_message
{
    char applicants[MIN_APPLICANTS * ENTRY_SIZE];
    int all_applicants;
    int price;
    char direction[11];
};

struct trip_start_ack
{
    int gained_money;
    int amount_of_beer;
};

void new();
void modify();
void delete();
void list_names();
void list_names_by_route();
void copy_file(const char * from, const char * to);
void start_trip(char * direction);
void handler(int signumber);

int main(int argc, char *argv[])
{
    printf("Mit szeretnel csinalni?  (kilepes - CTRL+C)\n");
    printf("u - uj jelentkezo adatainak megadasa\n");
    printf("m - meglevo jelentkezo adatainak modositasa\n");
    printf("t - meglevo jelentkezo adatainak torlese\n");
    printf("n - teljes nevsor kilistazasa\n");
    printf("l - egyes utvonalakra jelentkezettek kilistazasa\n");
    printf("i - tura inditasa\n");

    char input[MAX_SIZE];
    scanf("%s", input);

    if (strcmp(input, "u") == 0)
    {
        new();
    }
    else if (strcmp(input, "m") == 0)
    {
        modify();
    }
    else if (strcmp(input, "t") == 0)
    {
        delete();
    }
    else if (strcmp(input, "n") == 0)
    {
        list_names();
    }
    else if (strcmp(input, "l") == 0)
    {
        list_names_by_route();
    }
    else if (strcmp(input, "i") == 0)
    {
        FILE * data;
        data = fopen("data.txt", "r");

        char line[MAX_SIZE], name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
        int applicant_number, year, month, day, hour, min, sec;

        int count_parlament = 0;
        int count_hosok = 0;
        int count_var = 0;
        while(fgets(line, sizeof(line), data) != NULL)
        {
            sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
            if (strcmp("Parlament", route) == 0)
            {
                count_parlament += applicant_number;
            }
            else if (strcmp("Hosok", route) == 0)
            {
                count_hosok += applicant_number;
            }
            else if (strcmp("Var", route) == 0)
            {
                count_var += applicant_number;
            }
        }
        // printf("parlament: %d\nhosok: %d\nvar: %d\n", count_parlament, count_hosok, count_var);

        if (count_parlament >= MIN_APPLICANTS)
        {
            start_trip("Parlament");
        }
        else if (count_hosok >= MIN_APPLICANTS)
        {
            start_trip("Hosok");
        }
        if (count_var >= MIN_APPLICANTS)
        {
            start_trip("Var");
        }
    }

    return 0;
}

void new()
{
    FILE * data;
    data = fopen("data.txt", "a");

    char name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE], application_date[MAX_SIZE];
    int applicant_number;

    printf("Add meg a jelentkezo adatait! (kilepes - CTRL+C)\n");
    printf("nev (pl. PalInka): ");
    scanf("%s", name);
    printf("e-mail (pl. palinka@gmail.com): ");
    scanf("%s", email);
    printf("telfonszam (pl. +36-20-123-4567): ");
    scanf("%s", phone);

    printf("utvonal (Parlament/Hosok/Var): ");
    scanf("%s", route);
    while (strcmp(route, "Parlament") != 0 && strcmp(route, "Hosok") != 0 && strcmp(route, "Var") != 0)
    {
        printf("Ervenytelen utvonal. Add meg ujra: ");
        scanf("%s", route);
    }

    printf("jelentkezok szama (pl. 1): ");
    scanf("%d", &applicant_number);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(data, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, phone, route, applicant_number, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(data);
}

void modify()
{
    char input[MAX_SIZE];
    printf("Add meg annak jelentkezonek a nevet, akinek az adatait modositani szeretned: ");
    scanf("%s", input);

    FILE * data;
    FILE * temp;
    data = fopen("data.txt", "r");
    temp = fopen("temp.txt", "w");
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), data) != NULL)
    {
        char name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
        int applicant_number, year, month, day, hour, min, sec;
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);

        char new_name[MAX_SIZE], new_email[MAX_SIZE], new_phone_who_dis[MAX_SIZE], new_route[MAX_SIZE];
        int new_applicant_number;

        if (strcmp(name, input) == 0)
        {
            char mod[MAX_SIZE];
            printf("Mit szeretnel modositani?\n");
            printf("n - nev\n");
            printf("e - email\n");
            printf("t - telefonszam\n");
            printf("u - utvonal\n");
            printf("j - jelentkezok szama\n");
            scanf("%s", mod);
            
            if (strcmp(mod, "n") == 0)
            {
                printf("Add meg az uj nevet: ");
                scanf("%s", new_name);
                fprintf(temp, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", new_name, email, phone, route, applicant_number, year, month, day, hour, min, sec);
            }
            else if (strcmp(mod, "e") == 0)
            {
                printf("Add meg az uj e-mail cimet: ");
                scanf("%s", new_email);
                fprintf(temp, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, new_email, phone, route, applicant_number, year, month, day, hour, min, sec);
            }
            else if (strcmp(mod, "t") == 0)
            {
                printf("\nAdd meg az uj telefonszamot: ");
                scanf("%s", new_phone_who_dis);
                fprintf(temp, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, new_phone_who_dis, route, applicant_number, year, month, day, hour, min, sec);
            }
            else if (strcmp(mod, "u") == 0)
            {
                printf("Add meg az uj utvonalat: ");
                scanf("%s", new_route);

                while (strcmp(new_route, "Parlament") != 0 && strcmp(new_route, "Hosok") != 0 && strcmp(new_route, "Var") != 0)
                {
                    printf("Ervenytelen utvonal. Add meg ujra az uj utvonalat: ");
                    scanf("%s", new_route);
                }

                fprintf(temp, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, phone, new_route, applicant_number, year, month, day, hour, min, sec);
            }
            else if (strcmp(mod, "j") == 0)
            {
                printf("Add meg a jelentkezok szamat: ");
                scanf("%d", &new_applicant_number);
                fprintf(temp, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, phone, route, new_applicant_number, year, month, day, hour, min, sec);
            }
            else
            {
                modify();
            }
        }
        else
        {
            fputs(line, temp);
        }
    }

    fclose(data);
    fclose(temp);

    copy_file("temp.txt", "data.txt");
}

void delete()
{
    char input[MAX_SIZE];
    printf("Add meg annak jelentkezonek a nevet, akinek az adatait modositani szeretned: ");
    scanf("%s", input);

    FILE * data;
    FILE * temp;
    data = fopen("data.txt", "r");
    temp = fopen("temp.txt", "w");
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), data) != NULL)
    {
        char name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
        int applicant_number, year, month, day, hour, min, sec;
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);

        if (strcmp(name, input) != 0)
        {
            fputs(line, temp);
        }
    }

    fclose(data);
    fclose(temp);

    copy_file("temp.txt", "data.txt");
}

void list_names()
{
    FILE * data;
    data = fopen("data.txt", "r");

    char line[MAX_SIZE], name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
    int applicant_number, year, month, day, hour, min, sec;

    printf("Jelentkezettek nevei:\n");

    while(fgets(line, sizeof(line), data) != NULL)
    {
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
        printf("%s\n", name);
    }

    fclose(data);
}

void list_names_by_route()
{
    FILE * data;
    data = fopen("data.txt", "r");

    char input[MAX_SIZE], line[MAX_SIZE], name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
    int applicant_number, year, month, day, hour, min, sec;

    printf("Melyik utvonalra jelentkezetteik neveit szeretned listazni (Parlament/Hosok/Var)? ");
    scanf("%s", input);
    while (strcmp(input, "Parlament") != 0 && strcmp(input, "Hosok") != 0 && strcmp(input, "Var") != 0)
    {
        printf("Ervenytelen utvonal. Add meg ujra: ");
        scanf("%s", input);
    }

    printf("%s utvonalra jelentkezettek nevei:\n", input);
    while(fgets(line, sizeof(line), data) != NULL)
    {
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
        if (strcmp(input, route) == 0)
        {
            printf("%s\n", name);
        }
    }

    fclose(data);
}

void copy_file(const char * from, const char * to)
{
    FILE * fr;
    FILE * t;
    fr = fopen(from, "r");
    t = fopen(to, "w");

    char line[MAX_SIZE], name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
    int applicant_number, year, month, day, hour, min, sec;

    while (fgets(line, sizeof(line), fr) != NULL)
    {
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
        fprintf(t, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, phone, route, applicant_number, year, month, day, hour, min, sec);
    }

    fclose(fr);
    fclose(t);

    remove("temp.txt");
}

void start_trip(char * direction)
{
    FILE * data;
    data = fopen("data.txt", "r");

    char line[MAX_SIZE], name[MAX_SIZE], email[MAX_SIZE], phone[MAX_SIZE], route[MAX_SIZE];
    int applicant_number, year, month, day, hour, min, sec;

    int pipefd[2];
    pid_t pid;
    signal(SIGUSR1, handler);
    int message_queue;
    message_queue = msgget(ftok("SKB", 1), IPC_CREAT);

    if (message_queue < 0)
    {
        perror("Hiba: Nem sikerult letrehozni az uzenetsort.\n");
        return;
    }
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitasakor.\n");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Hiba: Fork hiba.\n");
        return;
    }

    // szulo
    if (pid > 0) {
        pause(); // var a signal erkezesere
        printf("[SKB] Utas adatok atadasa csovezeteken...\n");

        struct trip_start_message tsm;

        FILE * temp;
        temp = fopen("temp.txt", "w");

        char needed_applicants[MIN_APPLICANTS * ENTRY_SIZE];
        int discounts = 0;
        int i = 0; // nem kedvezmenyes utasok szama is egyben
        while(fgets(line, sizeof(line), data) != NULL)
        {
            ++i;
            sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
            if (strcmp(direction, route) == 0)
            {
                strcat(tsm.applicants, line);
                if (applicant_number > 1)
                {
                    discounts += applicant_number - 1;
                }
                fputs(line, temp);
            }
        }
        int cost = i * PRICE_PER_LITER + discounts * PRICE_PER_LITER * 0.85;

        fclose(temp);
        copy_file("temp.txt", "data.txt");

        strcpy(tsm.direction, direction);
        tsm.all_applicants = i + discounts;
        tsm.price = cost;
        // osszeallitottuk az uzenetet a gyerek szamara
        write(pipefd[1], &tsm, sizeof(tsm));
        // pipeid[0]: olvashato veg
        // pipeid[1]: irhato veg
        // az adott memoriacimtol kezdve struct meretnyit fogunk irni a pipefile-ba

        close(pipefd[1]);
        printf("[SKB] Utas adatok atkuldve, csovezetek bezarva.\n");

        printf("[SKB] Varakozas a beszedett dijra es tankolt sormennyisegre...\n");
        struct trip_start_ack tsa;
        pause();
        read(pipefd[0], &tsa, sizeof(tsa)); // beolvassuk a gyerektol az adatokat, amig a gyerek nem kuldi (elvileg), addig nem megy tovabb a szulo
        close(pipefd[0]); // read-end bezarasa
        // megnyitas: maga a pipe meghivasa, lezarni manualisan!
        wait(); // megvarja a gyerekfolyamat befejzodeset
        printf("[SKB] SBD - befejezodott az ut!\n");
        sleep(2);
    }

    // gyerek
    else
    {
        kill(pid, SIGUSR1); // kuld egy signalt a szulonek, a szulo addig pause-olva van

        printf("[SBD] Varakozas az utasokra.\n");
        struct trip_start_message tsm;
        read(pipefd[0], &tsm, sizeof(tsm));
        close(pipefd[0]); // bezarjuk az olvasasi veget
        printf("[SBD] utasok:\n%s\n", tsm.applicants);

        int budget = tsm.price * 5;
        struct trip_start_ack tsa;
        tsa.gained_money = budget;
        tsa.amount_of_beer = tsm.all_applicants * 5;
        write(pipefd[1], &tsa, sizeof(tsa));
        kill(pid, SIGUSR1);
        // elkuldjuk a szulonek h mennyit tankoltunk es mennyit kerestunk
        close(pipefd[1]); // write-end bezarasa

        srand(time(NULL)); // random: masodperc sleep
        int trip_time = (rand() % 5) + 2; // ennyi masodpercet alszik
        printf("[SBD] Indul a TRIP...\n");
        sleep(trip_time);

        printf("[SBD] Befejezodott az ut.\n");
        // exit(0); // a gyerekfolyamat kilep, a szulo pedig visszater a resztvevo hozzaadasa ponthoz
    }

    fclose(data);
}

void handler(int signumber) {};