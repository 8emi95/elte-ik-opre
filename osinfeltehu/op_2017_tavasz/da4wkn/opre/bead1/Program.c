#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <string.h> //strchr, strlen
#include <time.h> //time, asctime

typedef int bool;
#define true 1
#define false 0

struct Record
{
    int OrderNumber;
    char FirstName[16];
    char SecondName[16];
    char Mail[32];
    time_t Time;
};

typedef struct node
{
    struct Record val;
    struct node *next;
} Record_node_t;

bool readFileToRecordList(Record_node_t *head, char *EventID)
{
    Record_node_t *current = head;
    struct Record *record = malloc(sizeof(struct Record));

    FILE* File = fopen(EventID, "rb");
    if (File == NULL)
    {
        perror("Nincs ilyen esemeny!");
        return false;
    }

    fseek(File, 0, SEEK_SET);
    while (!feof(File))
    {
        fread(record, sizeof(struct Record), 1, File);
        if (!feof(File))
        {
            current->next = malloc(sizeof(Record_node_t));
            current = current->next;
            current->val = *record;
            printf("%s %s\t%s\n", current->val.SecondName, current->val.FirstName, current->val.Mail);
            current->next = NULL;
        }
    }

    free(record);
    fclose(File);
    return true;
}

void deallocateRecordList(Record_node_t **n)
{  
    if(!*n) return;
    deallocateRecordList(&((*n)->next));
    free(*n);
    *n = NULL; 
}

bool WriteFileFromRecordList(Record_node_t *head, char EventID[32])
{
    FILE* File = fopen(EventID, "wb");
    if (File == NULL)
    {
        perror("Nincs ilyen esemeny!");
        return false;
    }

    Record_node_t * current = head;

    while (current->next != NULL) {
        current = current->next;
        
        // printf("%s %s\t%s\n", current->val.SecondName, current->val.FirstName, current->val.Mail);
        if (fwrite(&current->val, sizeof(struct Record), 1, File) != 1)
        {
            perror("Sikertelen fajliras!");
            fclose(File);
            return false;
        }
    }

    fclose(File);
    return true;
}

bool CreateEvent()
{
    FILE *file;
    char EventID[32];
    printf("Adja meg a rendezveny azonositojat: ");
    scanf("%s", EventID);
    file = fopen(EventID, "wb+");
    if (file == NULL)
    {
        perror("Hiba tortent a fajl megnyitasa kozben!\n");
        return false;
    }
    struct Record nullRecord;
    nullRecord.OrderNumber = 0;
    if (fwrite(&nullRecord, sizeof(struct Record), 1, file) != 1)
    {
        perror("Hiba tortent a fajl irasa kozben!\n");
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

bool Login()
{
    char firstName[16];
    char secondName[16];
    char mail[32];
    char EventID[32];

    struct Record record;
    struct Record *lastRecord = malloc(sizeof(struct Record));

    printf("Vezeteknev: ");
    scanf("%s", secondName);
    strcpy(record.SecondName, secondName);
    printf("Keresztnev: ");
    scanf("%s", firstName);
    strcpy(record.FirstName, firstName);
    printf("Mail: ");
    scanf("%s", mail);
    strcpy(record.Mail, mail);
    printf("Rendezveny azonositoja: ");
    scanf("%s", EventID);
    record.Time = time(NULL);

    FILE* File = fopen(EventID, "ab+");
    if (File == NULL)
    {
        perror("Nincs ilyen esemeny!\n");
        return false;
    }

    
    fseek(File, -sizeof(struct Record), SEEK_END);
    fread(lastRecord, sizeof(struct Record), 1, File);
    record.OrderNumber = lastRecord->OrderNumber + 1;

    printf("Te vagy a %d. jelentkezo az esemenyre.\n", record.OrderNumber);
    if (fwrite(&record, sizeof(struct Record), 1, File) != 1)
    {
        perror("Sikertelen fajliras!\n");
        fclose(File);
        return false;
    }

    free(lastRecord);
    fclose(File);
    return true;
}

bool ListLogins()
{
    char EventID[32];
    struct Record *record = malloc(sizeof(struct Record));

    printf("Esemeny azonosito: ");
    scanf("%s", EventID);

    FILE* File = fopen(EventID, "rb");
    if (File == NULL)
    {
        perror("Nincs ilyen esemeny!\n");
        return false;
    }

    fseek(File, sizeof(struct Record), SEEK_SET); //skip nullRecord
    while(!feof(File))
    {
        fread(record, sizeof(struct Record), 1, File);
        if (!feof(File))
        {
            struct tm *timeinfo = localtime(&record->Time);
            printf("%s %s, %s, %s", record->SecondName, record->FirstName, record->Mail, asctime(timeinfo));
        }
    }

    free(record);
    fclose(File);
    return true;
}


bool ChangeGuestDetails()
{
    Record_node_t *head = NULL;
    head = malloc(sizeof(Record_node_t));
    head->next = NULL;
    char EventID[32];
    char SecondName[16];
    char FirstName[16];
    char Mail[32];

    printf("Adja meg az esemeny azonositojat: ");
    scanf("%s", &EventID);
    if (readFileToRecordList(head, EventID))
    {
        printf("Adja meg a modositani kivant jelentkezo vezeteknevet: ");
        scanf("%s", &SecondName);
        Record_node_t *current = head;
        if (current == NULL)
        {
            printf("Ures a fajl!\n");
            deallocateRecordList(&head);
            free(head);
            return false;
        }
        while (strcmp(current->val.SecondName, SecondName) && current->next != NULL)
        {
            current = current->next;
        }
        if (strcmp(current->val.SecondName, SecondName))
        {
            printf("Nincs ilyen nevu jelentkezo.\n");
            deallocateRecordList(&head);
            free(head);
            return false;
        }
        else
        {
            int choice;
            printf("Mely adatot akarja modositani: 1) Vezeteknev, 2) Keresztnev, 3) Mail: ");
            scanf("%i", &choice);
            switch (choice)
            {
                case 1:
                    printf("Uj vezeteknev: ");
                    scanf("%s", &SecondName);
                    strcpy(current->val.SecondName, SecondName);
                    break;
                case 2:
                    printf("Uj keresztnev: ");
                    scanf("%s", &FirstName);
                    strcpy(current->val.FirstName, FirstName);
                    break;
                case 3:
                    printf("Uj mail: ");
                    scanf("%s", &Mail);
                    strcpy(current->val.Mail, Mail);
                    break;
                default:
                    printf("Nincs ilyen menu-elem!\n");
            }
            if (!WriteFileFromRecordList(head, EventID))
            {
                printf("Fajliras sikertelen!\n");
                deallocateRecordList(&head);
                free(head);
                return false;
            }
            else printf("Fajliras sikeres!\n");
        }
    }
    else // if (readFileToRecordList(head, EventID))
    {
        perror("Nem sikerult beolvasni a fajlt!");
        deallocateRecordList(&head);
        free(head);
        return false;
    }

    deallocateRecordList(&head);
    free(head);
    return true;
}

bool DeleteGuest()
{
    Record_node_t *head = NULL;
    head = malloc(sizeof(Record_node_t));
    head->next = NULL;
    char EventID[32];
    char SecondName[16];

    printf("Adja meg az esemeny azonositojat: ");
    scanf("%s", &EventID);
    if (readFileToRecordList(head, EventID))
    {
        printf("Adja meg a modositani kivant jelentkezo vezeteknevet: ");
        scanf("%s", &SecondName);
        Record_node_t *current = head;
        if (current == NULL)
        {
            printf("Ures a fajl!\n");
            deallocateRecordList(&head);
            free(head);
            return false;
        }
        while (strcmp(current->next->val.SecondName, SecondName) && current->next != NULL)
        {
            current = current->next;
        }
        if (strcmp(current->next->val.SecondName, SecondName))
        {
            printf("Nincs ilyen nevu jelentkezo.\n");
            deallocateRecordList(&head);
            free(head);
            return false;
        }
        // Deleting node:
        Record_node_t *newNext = current->next;
        current->next = newNext->next;
        free(newNext);
        
        // Write list to file:
        if (!WriteFileFromRecordList(head, EventID))
        {
            printf("Fajliras sikertelen!\n");
            deallocateRecordList(&head);
            free(head);
            return false;
        }


    }
    else // if (readFileToRecordList(head, EventID))
    {
        perror("Nem sikerult beolvasni a fajlt!");
        deallocateRecordList(&head);
        free(head);
        return false;
    }

    return true;
}

bool DeleteEvent()
{
    char EventID[32];

    printf("Adja meg az esemeny azonositojat: ");
    scanf("%s", &EventID);

    if(!remove(EventID)) printf("Fajl torlese sikeres.\n");
    else 
    {
        perror("Fajl torlese sikertelen.");
        return false;
    }

    return true;
}

int main()
{
    printf("\n\n== King of Stands ==\nElektronikus latogatoi nyilvantartas\n");
    int selectedItem;
    size_t bufsize = 2;
    do
    {
        printf("\n--Menu--\n 0) Kilepes\n 1) Rendezveny inditasa\n 2) Jelentkezesek listazasa\n 3) Vendeg adatainak modositasa\n 4) Vendeg adatainak torlese\n 5) Bejelentkezes rendezvenyre\n 6) Rendezveny torlese\n");
        scanf("%d", &selectedItem);
        switch (selectedItem)
        {
            case 5:
                if (Login())
                    printf("Jelentkezes sikeres.\n");
                else
                    printf("Jelentkezes sikertelen.\n");
                break;
            case 2:
                if (!ListLogins())
                    printf("Listazas sikertelen.\n");
                break;
            case 3:
                if (ChangeGuestDetails())
                    printf("Modositas sikeres.\n");
                else
                    printf("Modositas sikertelen.\n");
                break;
            case 4:
                if (DeleteGuest())
                    printf("Torles sikeres.\n");
                else
                    printf("Torles sikertelen.\n");
                break;
            case 1:
                if (CreateEvent())
                    printf("Rendezveny letrehozva.\n");
                else
                    printf("Rendezveny letrehozasa sikertelen.\n");
                break;
            case 6:
                if (DeleteEvent())
                    printf("Rendezveny torolve.\n");
                else
                    printf("Rendezveny torlese sikertelen.\n");
                break;
            case 0:
                break;
            default:
                printf("Nincs ilyen menu-elem!\n");
        }
    } while (selectedItem != 0);

    return 0;
}