/*
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
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 1024

void new();
void modify();
void delete();
void list_names();
void list_names_by_route();
void copy_file(const char * from, const char * to);

int main(int argc, char *argv[])
{
    printf("Mit szeretnel csinalni?  (kilepes - CTRL+C)\n");
    printf("u - uj jelentkezo adatainak megadasa\n");
    printf("m - meglevo jelentkezo adatainak modositasa\n");
    printf("t - meglevo jelentkezo adatainak torlese\n");
    printf("n - teljes nevsor kilistazasa\n");
    printf("l - egyes utvonalakra jelentkezettek kilistazasa\n");

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

    return 0;
}

void new()
{
    FILE * data;
    data = fopen("data.txt", "a");

    char name[MAX_SIZE];
    char email[MAX_SIZE];
    char phone[MAX_SIZE];
    char route[MAX_SIZE];
    int applicant_number;
    char application_date[MAX_SIZE];

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
        char name[MAX_SIZE];
        char email[MAX_SIZE];
        char phone[MAX_SIZE];
        char route[MAX_SIZE];
        int applicant_number;
        int year;
        int month;
        int day;
        int hour;
        int min;
        int sec;
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);

        char new_name[MAX_SIZE];
        char new_email[MAX_SIZE];
        char new_phone_who_dis[MAX_SIZE];
        char new_route[MAX_SIZE];
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
        char name[MAX_SIZE];
        char email[MAX_SIZE];
        char phone[MAX_SIZE];
        char route[MAX_SIZE];
        int applicant_number;
        int year;
        int month;
        int day;
        int hour;
        int min;
        int sec;
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

    char line[MAX_SIZE];
    char name[MAX_SIZE];
    char email[MAX_SIZE];
    char phone[MAX_SIZE];
    char route[MAX_SIZE];
    int applicant_number;
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;

    printf("Jelentkezettek nevei:\n");

    while(fgets(line, 100, data) != NULL)
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

    char input[MAX_SIZE];
    char line[MAX_SIZE];
    char name[MAX_SIZE];
    char email[MAX_SIZE];
    char phone[MAX_SIZE];
    char route[MAX_SIZE];
    int applicant_number;
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;

    printf("Melyik utvonalra jelentkezetteik neveit szeretned listazni (Parlament/Hosok/Var)? ");
    scanf("%s", input);
    while (strcmp(input, "Parlament") != 0 && strcmp(input, "Hosok") != 0 && strcmp(input, "Var") != 0)
    {
        printf("Ervenytelen utvonal. Add meg ujra: ");
        scanf("%s", input);
    }

    printf("%s utvonalra jelentkezettek nevei:\n", input);
    while(fgets(line, 100, data) != NULL)
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

    char line[MAX_SIZE];
    char name[MAX_SIZE];
    char email[MAX_SIZE];
    char phone[MAX_SIZE];
    char route[MAX_SIZE];
    int applicant_number;
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;

    while (fgets(line, sizeof(line), fr) != NULL)
    {
        sscanf(line, "%s %s %s %s %d %d-%d-%d %d:%d:%d", name, email, phone, route, &applicant_number, &year, &month, &day, &hour, &min, &sec);
        fprintf(t, "%s %s %s %s %d %d-%d-%d %d:%d:%d\n", name, email, phone, route, applicant_number, year, month, day, hour, min, sec);
    }

    fclose(fr);
    fclose(t);

    remove("temp.txt");
}