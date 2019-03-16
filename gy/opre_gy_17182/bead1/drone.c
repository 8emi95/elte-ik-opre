/*
Ne-Sas-Old Kft drónfogó sasok bérbeadásával foglalkozik.

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
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 2048

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

int main(int argc, char *argv[])
{
    printf("What do you want to do?  (exit - CTRL+C)\n");
    printf("a) add new celebrity\n");
    printf("b) modify a celebrity's data\n");
    printf("c) delete a celebrity's data\n");
    printf("d) list all celebrities\n");
    printf("e) view a celebrity's data\n");
    printf("f) list requests by urgency\n");

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