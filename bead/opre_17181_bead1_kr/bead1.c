#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 20000
#define ENTRY_SIZE 250
#define TAB "\t\t"

void parse_file(char** content);
void display_startup();
void read_data(char fields[5][60]);
char* create_entry();
char* add_entry(const char* entries);
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
    return result;
}

char* concat(char* dest, const char* src)
{
    while (*dest) dest++;
    while (*dest++ = *src++);
    return --dest;
}

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
