/*
nem tudtam még befefejezni úgy, hogy helyesen működjön, de elküldöm akkor azt, ami jelenleg van
(Bocsi, csak kitartóan próbáltam kijavítani... arra jutottam, hogy kurva C)
ffuuuj.c
entries.txt
írogattam kommenteket a start_trip() nevű függvénybe, az a lényeges
meg az a 2 struct
azok mindenhová kellenek
illetve a handler
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// <bead2>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// </bead2>

#define MAX_SIZE 20000
#define ENTRY_SIZE 250
#define TAB "\t\t"
// <bead2>
#define PRICE 3000
#define BIKE_SEATS 6

struct trip_start_message
{
    char participants[BIKE_SEATS * ENTRY_SIZE];
    int price;
    char direction[11];
};

struct trip_start_ack
{
    int gained_money;
    int amount_of_beer;
};
// </bead2>

void parse_file(char** content);
void display_startup();
void read_data(char fields[5][60]);
char* create_entry();
char* add_entry(const char* entries);
// <bead2>
int count_by_direction(char* entries, char* direction);
void start_trip(char* direction, char* entries);
void handler(int signumber);
// </bead2>
char* concat(char* dest, const char* src);
char* search(char* entries, char* keyword);
void list_by_direction(char* entries, char* direction);
void delete_entry(char* entries, char* keyword);
void modify_entry(char** entries, char* keyword);
void write_to_file(char* entries);

int main()
{
    char* content = NULL;
    parse_file(&content);

    display_startup();
    char chosen_action[2];
    scanf("%s", chosen_action);
    int action_number = atoi(chosen_action);

    while (action_number != 0)
    {
        switch(action_number)
        {
            case 1:
                puts("---ADDING NEW PARTICIPANT---");
                puts("Enter data: ");
                content = add_entry(content);
                puts("\nAdding entry...\n");
                puts(content);
                break;

            case 2:
                printf("\n---LIST OF PARTICIPANTS--- \n%s\n", content);
                break;

            case 3:
                puts("---LIST BY DIRECTION---");
                char direction[15];
                do
                {   puts("Enter direction (Parlament, Budai_Var, Hosok_Tere): ");
                    scanf("\n%[^\n]", direction);
                    if ((strcmp(direction, "Parlament") != 0) &&
                        (strcmp(direction, "Budai_Var") != 0) &&
                        (strcmp(direction, "Hosok_Tere") != 0))
                    {
                        puts("Not a direction.");
                    }
                }
                while((strcmp(direction, "Parlament") != 0) &&
                    (strcmp(direction, "Budai_Var") != 0) &&
                    (strcmp(direction, "Hosok_Tere") != 0));

                list_by_direction(content, direction);
                break;

            case 4:
                puts("---MODIFY ENTRY---");
                printf("\nParticipants: \n\n%s", content);
                char mod_keyword[60];
                puts("\nEnter keyword to modify entry: ");
                scanf("\n%[^\n]", mod_keyword);
                modify_entry(&content, mod_keyword);
                printf("\nParticipants after modification: \n\n%s", content);
                break;

            case 5:
                puts("---DELETE ENTRY---");
                char del_keyword[60];
                printf("\nParticipants: \n\n%s", content);
                puts("\nEnter keyword to delete containing entry: ");
                scanf("\n%[^\n]", del_keyword);
                delete_entry(content, del_keyword);
                break;

            default:
                puts("INVALID KEY");
                break;
        }
        puts("\nChoose action... (1, 2, 3, 4, 5 or 0)");
        scanf("%s", chosen_action);
        action_number = atoi(chosen_action);
    }
    write_to_file(content);
    free(content);
    return 0;
}

void display_startup()
{
    puts("_______________________");
    puts("|_____BEER BIKES_______|");

    puts("    o   ");
    puts("   (<=U ");
    puts("_(O)˘(O)_______________\n");

    puts("1:________________Apply");
    puts("2:_________List Entries");
    puts("3:____List By Direction");
    puts("4:_________Modify Entry");
    puts("5:_________Delete Entry");
    puts("0:_________________Exit");

    puts("\nPress key (1, 2, 3, 4, 5 or 0) to continue:");
}


void parse_file(char** content)
{
    FILE *fp = fopen("entries.txt", "r");
    if (fp != NULL)
    {
        if (fseek(fp, 0L, SEEK_END) == 0)
        {
            long bufsize = ftell(fp);
            if (bufsize == -1)
            {
                puts("ERROR");
                exit(1);
            }

            *content = (char*)malloc(sizeof(char) * (bufsize + 1));
            if (fseek(fp, 0L, SEEK_SET) != 0)
            {
                puts("ERROR 2");
                exit(2);
            }

            size_t new_len = fread(*content, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 )
            {
                fputs("Error reading file", stderr);
            }
            else
            {
                (*content)[new_len++] = '\0';
            }
        }
    }
    fclose(fp);
}

void read_data(char fields[5][60])
{
    printf("Name: ");
    scanf("\n%[^\n]", fields[0]);

    printf("Email: ");
    scanf("\n%[^\n]", fields[1]);

    printf("Phone number: ");
    scanf("\n%[^\n]", fields[2]);

    do
    {
        printf("Number of participants (between 1 and 9): ");
        scanf("\n%[^\n]", fields[3]);
        if ((strlen(fields[3]) != 1) || (atoi(fields[3]) == 0))
        {
            puts("Number of participants must be between 1 and 9.");
        }
    }
    while((strlen(fields[3]) != 1) || (atoi(fields[3]) == 0));

    do
    {
        printf("Direction (Parlament/Budai_Var/Hosok_Tere): ");
        scanf("\n%[^\n]", fields[4]);
        if ((strcmp(fields[4], "Parlament") != 0) &&
            (strcmp(fields[4], "Budai_Var") != 0) &&
            (strcmp(fields[4], "Hosok_Tere") != 0))
            {
                puts("Wrong direction.");
            }
    }
    while((strcmp(fields[4], "Parlament") != 0) &&
        (strcmp(fields[4], "Budai_Var") != 0) &&
        (strcmp(fields[4], "Hosok_Tere") != 0));
}

char* create_entry()
{
    char fields[5][60];
    read_data(fields);

    char* new_entry = malloc(ENTRY_SIZE);
    char* p = new_entry;
    new_entry[0] = '\0';

    int field_number;
    for (field_number = 0; field_number < 5; ++field_number)
    {
        p = concat(p, fields[field_number]);
        p = concat(p, TAB);
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    p = concat(p, asctime(tm));
    return new_entry;
}

char* add_entry(const char* entries)
{
    char* new_entry = create_entry();
    const size_t entries_len = strlen(entries);
    const size_t new_entry_len = strlen(new_entry);
    char *result = malloc(entries_len + new_entry_len + 1);
    memcpy(result, entries, entries_len);
    memcpy(result + entries_len, new_entry, new_entry_len + 1);
    free(new_entry);

    // <bead2>
    int participants_Hosok_Tere = count_by_direction(result, "Hosok_Tere");
    int participants_Budai_Var = count_by_direction(result, "Budai_Var");
    int participants_Parlament = count_by_direction(result, "Parlament");

    if (participants_Hosok_Tere >= BIKE_SEATS)
    {
        start_trip("Hosok_Tere", result);
    }
    else if (participants_Parlament >= BIKE_SEATS)
    {
        start_trip("Parlament", result);
    }
    else if (participants_Budai_Var >= BIKE_SEATS)
    {
        start_trip("Budai_Var", result);
    }
    // </bead2>

    return result;
}

// <bead2>
struct message
{
    long sender_id;
    char msg[1024];
};

void start_trip(char* direction, char* entries)
{
    int pipefd[2];
    pid_t pid;
    signal(SIGUSR1, handler);
    int message_queue;
    message_queue = msgget(ftok("SKB", 1), 0644 | IPC_CREAT);

    if (message_queue < 0)
    {
        perror("error: nem sikerult letrehozni az uzenetsort\n");
        return;
    }

    if (pipe(pipefd) == -1)
    {
        perror("hiba a pipe nyitasakor\n");
        return;
    }
    pid = fork();

    if (pid == -1)
    {
        perror("error: fork hiba\n");
        return;
    }
    // szülő

    if (pid > 0) {

        pause(); //waits till signal arrives
        printf("[SKB] Utas adatok atadasa csovezeteken...\n");
        close(pipefd[0]); // read-end bezárása

        int number_of_participants = 0;
        char entries_copy[MAX_SIZE];
        strcpy(entries_copy, entries);
        char* line = NULL;
        char* found = NULL;
        line = strtok(entries_copy, "\n");

        number_of_participants = 0;

        //a nekunk kello utasok: ugyelni arra, hogy ki hany fot hozott magaval! (BIKE_SEATS: max)
        char* needed_participants = "asdasdasdasd";

        
        //kigyujteni megfelelo szamu utast az eredeti utaslitabol,
        //azokat onnan torolni
        
        struct trip_start_message tsm;
        strcpy(tsm.participants, needed_participants);
        strcpy(tsm.direction, direction);
        tsm.price = PRICE;
        //osszeallitottuk az uzenetet a gyerek szamara
        //pipeid[0] --> olvasható vég, 1-->írható
        write(pipefd[1], &tsm, sizeof(tsm));
        //az adott memóriacímtől kezdve struct méretnyit fogunk írni
        //a pipefile-ba írunk

        close(pipefd[1]);
        printf("[SKB] Utas adatok atkuldve, csovezetek bezarva.\n");

        printf("[SKB] Varakozas a beszedett dijra es tankolt sormennyisegre...\n");
        struct trip_start_ack tsa;
        read(pipefd[0], &tsa, sizeof(tsa));
        //beolvassuk a gyerektol az adatokat
        //amig a gyerek nem küldi (elvileg), addig nem megy tovább a szülő
        close(pipefd[0]);
        //megnyitas: maga a pipe meghivasa, lezárni manuálisan!
        wait();
        //megvarja a gyerekfolyamat befejződését
        printf("[SKB] SBD - befejezodott az ut, back to the menü!\n");
        sleep(2);
    }

    // gyerek

    else
    {

        //kuld egy signalt a szulonek, a szulo addig pause-olva van
        kill(pid, SIGUSR1);

        printf("[SBD] Elek es virulok, varom az utasokat...\n");
        struct trip_start_message tsm;
        read(pipefd[0], &tsm, sizeof(tsm));
        close(pipefd[0]); //bezárjuk az olvasási végét
        printf("[SBD] utasok: %s\n", tsm.participants);

        /*
            pénzbeszedés
            sörmennyiség kiszámítása
            BIKE_SEATS*5liter
            kedvezmény

        */
        int budget = 0/*beszedés eredménye*/;
        struct trip_start_ack tsa;
        tsa.gained_money = budget;
        tsa.amount_of_beer = BIKE_SEATS*5;
        //lehet, hoyg nem jó a write???
        printf("write elott \n");
        write(pipefd[1], &tsa, sizeof(tsa));
        printf("write utan \n");
        //elküldjük a szülőnek, hogy mennyit tankoltunk és mennyit kerestünk
        close(pipefd[1]); // write-end bezárása

        //random: másodperc sleep
        srand(time(NULL));
        int trip_time = (rand() % 5) + 2;
        //ennyi másodpercet alszik ^
        printf("[SBD] Indul a TRIP...\n");
        sleep(trip_time);

        printf("[SBD] Befejezodott az ut.\n");
        exit(0); // a gyerekfolyamat kilép, a szülő pedig visszatér a résztvevő hozzáadása ponthoz
    }

}
// </bead2>

char* concat(char* dest, const char* src)
{
    while (*dest) dest++;
    while (*dest++ = *src++);
    return --dest;
}

// <bead2>
void handler(int signumber){};
// </bead2>

char* search(char* entries, char* keyword)
{
    char entries_copy[MAX_SIZE];
    strcpy(entries_copy, entries);
    char* result = NULL;
    char* line = NULL;
    char* found = NULL;
    line = strtok(entries_copy, "\n");
    while ((line != NULL) && (!found))
    {
      found = strstr(line, keyword);
      if (found)
      {
          result = line;
      }
      line = strtok(NULL, "\n");
    }
    return result;
}

void list_by_direction(char* entries, char* direction)
{
    char entries_copy[MAX_SIZE];
    strcpy(entries_copy, entries);
    char* line = NULL;
    char* found = NULL;
    line = strtok(entries_copy, "\n");
    while (line != NULL)
    {
        found = strstr(line, direction);
        if (found)
        {
          puts(line);
        }
        line = strtok(NULL, "\n");
    }
}

// <bead2>
int count_by_direction(char* entries, char* direction)
{
    int number_of_participants = 0;
    char entries_copy[MAX_SIZE];
    strcpy(entries_copy, entries);
    char* line = NULL;
    char* found = NULL;
    line = strtok(entries_copy, "\n");
    while (line != NULL)
    {
        found = strstr(line, direction);
        if (found)
        {
          ++number_of_participants;
        }
        line = strtok(NULL, "\n");
    }
    return number_of_participants;
}
// </bead2>

void delete_entry(char* entries, char* keyword)
{
    char* to_delete = search(entries, keyword);
    if (to_delete)
    {
        entries = strstr(entries, to_delete);

        const size_t line_length = strlen(to_delete) + 1;
        char *copy_end;
        char *copy_from = entries + line_length;
        while (NULL != (copy_end = strstr(copy_from, to_delete)))
        {
            memmove(entries, copy_from, copy_end - copy_from);
            entries += copy_end - copy_from;
            copy_from = copy_end + line_length;
        }
        memmove(entries, copy_from, 1 + strlen(copy_from));
    }
    else
    {
        puts("Invalid key.");
    }
}

void modify_entry(char** entries, char* keyword)
{
    char* found_line = search(*entries, keyword);
    if(found_line)
    {
        printf("Result: \n%s\n", found_line);
        delete_entry(*entries, keyword);

        puts("Enter new values.\n");
        *entries = add_entry(*entries);
    }
    else
    {
        puts("Sorry, there is no such entry.");
    }
}

void write_to_file(char* entries)
{
    FILE *fp = fopen("entries.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "%s", entries);
    }
    fclose(fp);
}
