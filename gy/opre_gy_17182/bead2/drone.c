/*
1. Ne-Sas-Old Kft drónfogó sasok bérbeadásával foglalkozik.

bejelentést intéző alkalmazás:
- bejelentés idejét (rendszeridő)
- az ügyfél nevét
- címét
- e-mail címét
- telefonszámát
- sürgősséggel kéri-e a beavatkozást (feláras)

funkciók:
- adatokat bevinni
- módosítani
- törölni
- listázni
    - teljes
    - szűrt listák
        - ügyfélre
        - sürgősségre

Az adatokat fájlban tároljuk.


2. 
szülő: Ne-Sas-Old Kft (NSO)
gyerek: idomár (IDO)
gyerek: madár (SAS)
képernyő (SCR)
fájl (FIL)

IDO->NSO: jelzés (madár körüli teendőket elvégezte, munkára kész)
NSO->IDO: cső (következő cím, sürgőst előre veszik)
     IDO: csőből kiolvas
IDO->SCR: nyugtázza, kiírja a képernyőre h megkapta a címet
IDO  SAS: helyszínre megy, drón befogása hosszabb-rövidebb ideig tarthat (1-5s, véletlen)
IDO->NSO: cső (mennyi idő alatt fogta be, kéri a következő címet)
NSO->FIL: elkészíti a számlát (fájlba)
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
// #include <sys/msg.h>

#define MAX_SIZE 2048

#define PRICE_NORMAL 100000
#define PRICE_URGENT 120000
#define PRICE_SECOND 1000

struct celebrity
{
    char application_date[MAX_SIZE];
    char name[MAX_SIZE];
    char address[MAX_SIZE];
    char email[MAX_SIZE];
    char phone[MAX_SIZE];
    char urgency[MAX_SIZE];
};

void new();
void modify();
void delete();
void list_all();
void view_info();
void list_by_urgency();
void copy_file(const char * from, const char * to);

void handler(int signumber);
void fulfill();
void organize();

int main(int argc, char *argv[])
{
    printf("What do you want to do?  (exit - CTRL+C)\n");
    printf("a) add new celebrity\n");
    printf("b) modify a celebrity's data\n");
    printf("c) delete a celebrity's data\n");
    printf("d) list all celebrities\n");
    printf("e) view a celebrity's data\n");
    printf("f) list requests by urgency\n");
    printf("g) fulfill request\n");

    char input[MAX_SIZE];
    scanf("%s", input);

    if (strcmp(input, "a") == 0)
    {
        new();
    }
    else if (strcmp(input, "b") == 0)
    {
        modify();
    }
    else if (strcmp(input, "c") == 0)
    {
        delete();
    }
    else if (strcmp(input, "d") == 0)
    {
        list_all();
    }
    else if (strcmp(input, "e") == 0)
    {
        view_info();
    }
    else if (strcmp(input, "f") == 0)
    {
        list_by_urgency();
    }
    else if (strcmp(input, "g") == 0)
    {
        organize();
    }

    return 0;
}

void new()
{
    FILE * data;
    data = fopen("data.txt", "a");

    struct celebrity c;

    printf("Please enter your information! (exit - CTRL+C)\n");
    printf("name (pl. Jack_Hoff): ");
    scanf("%s", c.name);
    printf("address (pl. 1234_Fake_Street_Chicago_IL_12345): ");
    scanf("%s", c.address);
    printf("e-mail (pl. jackhoff@gmail.com): ");
    scanf("%s", c.email);
    printf("phone number: (pl. 360-111-1111): ");
    scanf("%s", c.phone);

    printf("urgency (Y - yes, N - no): ");
    scanf("%s", c.urgency);
    while (strcmp(c.urgency, "Y") != 0 && strcmp(c.urgency, "N") != 0)
    {
        printf("Invalid data. Please enter urgency again: ");
        scanf("%s", c.urgency);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(data, "%d%d%d_%d%d%d %s %s %s %s %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, c.name, c.address, c.email, c.phone, c.urgency);

    fclose(data);
}

void modify()
{
    char to_modify[MAX_SIZE];
    printf("Enter the name of the celebrity whose data you'd like to modify: ");
    scanf("%s", to_modify);

    FILE * data;
    FILE * temp;
    data = fopen("data.txt", "r");
    temp = fopen("temp.txt", "w");
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), data) != NULL)
    {
        struct celebrity c;
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);

        char new_name[MAX_SIZE];
        char new_address[MAX_SIZE];
        char new_email[MAX_SIZE];
        char new_phone_who_dis[MAX_SIZE];
        char new_urgency[MAX_SIZE];

        if (strcmp(c.name, to_modify) == 0)
        {
            char mod[MAX_SIZE];
            printf("Which piece of information would you like to modify?\n");
            printf("a) name\n");
            printf("b) address\n");
            printf("c) email\n");
            printf("d) phone number\n");
            printf("e) urgency\n");
            scanf("%s", mod);
            
            if (strcmp(mod, "a") == 0)
            {
                printf("Please enter the new name: ");
                scanf("%s", new_name);
                fprintf(temp, "%s %s %s %s %s %s\n", c.application_date, new_name, c.address, c.email, c.phone, c.urgency);
            }
            else if (strcmp(mod, "b") == 0)
            {
                printf("Please enter the new address: ");
                scanf("%s", new_address);
                fprintf(temp, "%s %s %s %s %s %s\n", c.application_date, c.name, new_address, c.email, c.phone, c.urgency);
            }
            else if (strcmp(mod, "c") == 0)
            {
                printf("Please enter the new e-mail address: ");
                scanf("%s", new_email);
                fprintf(temp, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, new_email, c.phone, c.urgency);
            }
            else if (strcmp(mod, "d") == 0)
            {
                printf("Please enter the new phone number: ");
                scanf("%s", new_phone_who_dis);
                fprintf(temp, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, new_phone_who_dis, c.urgency);
            }
            else if (strcmp(mod, "e") == 0)
            {
                printf("Please enter the new urgency request: ");
                scanf("%s", new_urgency);

                while (strcmp(c.urgency, "Y") != 0 && strcmp(c.urgency, "N") != 0)
                {
                    printf("Invalid data. Please enter the new urgency request again: ");
                    scanf("%s", new_urgency);
                }

                fprintf(temp, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, new_urgency);
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
    char to_delete[MAX_SIZE];
    printf("Enter the name of the celebrity whose data you'd like to delete: ");
    scanf("%s", to_delete);

    FILE * data;
    FILE * temp;
    data = fopen("data.txt", "r");
    temp = fopen("temp.txt", "w");
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), data) != NULL)
    {
        struct celebrity c;
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);

        if (strcmp(c.name, to_delete) != 0)
        {
            fputs(line, temp);
        }
    }

    fclose(data);
    fclose(temp);

    copy_file("temp.txt", "data.txt");
}

void list_all()
{
    FILE * data;
    data = fopen("data.txt", "r");

    char line[MAX_SIZE];
    struct celebrity c;

    printf("Celebrities' names:\n");

    while(fgets(line, 1000, data) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        printf("%s\n", c.name);
    }

    fclose(data);
}

void view_info()
{
    FILE * data;
    data = fopen("data.txt", "r");

    char to_view[MAX_SIZE];
    char line[MAX_SIZE];
    struct celebrity c;

    printf("Which celebrities' information would you like to view? ");
    scanf("%s", to_view);

    printf("Information of %s:\n", to_view);
    while(fgets(line, 1000, data) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        if (strcmp(to_view, c.name) == 0)
        {
            printf("%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        }
    }

    fclose(data);
}

void list_by_urgency()
{
    FILE * data;
    data = fopen("data.txt", "r");

    char to_list[MAX_SIZE];
    char line[MAX_SIZE];
    struct celebrity c;

    printf("Do you want to view urgent or not urgent requests? (Y - urgent, N - not urgent) ");
    scanf("%s", to_list);

    while(fgets(line, 1000, data) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        if (strcmp(to_list, c.urgency) == 0)
        {
            printf("%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
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

    char line[MAX_SIZE];
    struct celebrity c;

    while (fgets(line, sizeof(line), fr) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        fprintf(t, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
    }

    fclose(fr);
    fclose(t);

    remove("temp.txt");
}

void handler(int signumber) {}

void fulfill()
{
    char line[MAX_SIZE];
    struct celebrity c;

    FILE * bill;
    bill = fopen("bill.txt", "w");
    int price = 0;
    int capture_time = 0;

    int parent_to_child[2];
    int child_to_parent[2];
    pid_t pid;
    signal(SIGUSR1, handler);

    if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0)
    {
        perror("Error while opening pipe.\n");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Fork error.\n");
        return;
    }

   if (pid != 0) // headquarters (parent)
    {
        pause();
        printf("Headquarters: Sending the first address...\n");

        // NSO->IDO: cső (következő cím, sürgőst előre veszik)
        FILE * organized_parent;
        organized_parent = fopen("organized.txt", "r");
        while(fgets(line, 1000, organized_parent) != NULL)
        {
            sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);

            write(parent_to_child[1], &c, sizeof(c));
            printf("Headquarters: Address sent. Waiting for trainer...\n");
            pause();

            // NSO->FIL: elkészíti a számlát (fájlba)
            read(child_to_parent[0], &capture_time, sizeof(int));
            price = capture_time * PRICE_SECOND;
            if (strcmp(c.urgency, "Y") == 0)
            {
                price += PRICE_URGENT;
            }
            else
            {
                price += PRICE_NORMAL;
            }
            fprintf(bill, "%s %i %s\n", c.urgency, price, c.name);
            printf("Headquarters: Bill created.\n");
        }

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        printf("Headquarters: No more requests.\n");
        fclose(organized_parent);

    }
    else // trainer (child)
    {
        // IDO->NSO: jelzés (madár körüli teendőket elvégezte, munkára kész)
        kill(pid, SIGUSR1);
        printf("Trainer: Eagle ready. Waiting for the first address...\n");

        FILE * organized_child;
        organized_child = fopen("organized.txt", "r");
        while(fgets(line, 1000, organized_child) != NULL)
        {
            sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);

            // IDO: csőből kiolvas
            // IDO->SCR: nyugtázza, kiírja a képernyőre h megkapta a címet
            read(parent_to_child[0], &c, sizeof(c));
            printf("Trainer: Address is %s\n", c.address);

            // IDO  SAS: helyszínre megy, drón befogása hosszabb-rövidebb ideig tarthat (1-5s, véletlen)
            // IDO->NSO: cső (mennyi idő alatt fogta be, kéri a következő címet)
            srand(time(NULL));
            capture_time = rand() % 5 + 1;
            sleep(capture_time);
            printf("Trainer: Capturing took %d second(s).\n", capture_time);
            write(child_to_parent[1], &capture_time, sizeof(int));

            printf("Trainer: Waiting for the next request...\n");
            kill(getppid(), SIGUSR1);
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        fclose(organized_child);
    }
}

void organize()
{
    char line[MAX_SIZE];
    struct celebrity c;
    FILE * organized;
    organized = fopen("organized.txt", "w");

    FILE * data1;
    data1 = fopen("data.txt", "r");
    while (fgets(line, sizeof(line), data1) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        if (strcmp(c.urgency, "Y") == 0)
        {
            fprintf(organized, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        }
    }
    fclose(data1);

    FILE * data2;
    data2 = fopen("data.txt", "r");
    while (fgets(line, sizeof(line), data2) != NULL)
    {
        sscanf(line, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        if (strcmp(c.urgency, "N") == 0)
        {
            fprintf(organized, "%s %s %s %s %s %s\n", c.application_date, c.name, c.address, c.email, c.phone, c.urgency);
        }
    }
    fclose(data2);

    fclose(organized);

    fulfill();
}