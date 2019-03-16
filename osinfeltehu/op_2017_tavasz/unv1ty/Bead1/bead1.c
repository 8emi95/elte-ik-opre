#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //open
#include <errno.h>     //perror, errno
#include <time.h>      //time, asctime
#include <fcntl.h>     //open,creat
#include <sys/stat.h>  //stat
#include <string.h>    //strchr, strlen

typedef int bool;
#define true 1
#define false 0

struct Question
{
    char text[80];
    bool isFinished;
	int answNum;
    char answers[4][80];
    time_t createDate;
};

typedef struct ListNodes
{
    struct Question q;
    struct ListNodes *next;
} ListNode;

bool AddQuestion(char FileName[32])
{
    char text[80];
    int answNum = 0;

    struct Question q;
    struct Question *last = malloc(sizeof(struct Question));

    int i;

    printf("Kerdes: ");
    scanf(" %[^\n]s", &text);
    strcpy(q.text, text);

    while (answNum < 2 || answNum > 4)
    {
        printf("Mennyi valaszt kivan rogziteni?");
        scanf("%i", &answNum);
        if (answNum < 2 || answNum > 4) printf("Minimum 2 es maximum 4 valasz megadasa lehetseges!\n");
    }
    q.answNum = answNum;

    for (i = 0; i < answNum; ++i)
    {
        char answer[80];
        printf("%i. valasz: ", i+1);
        scanf(" %[^\n]s", &answer);
        strcpy(q.answers[i], answer);
    }

    printf("Veglegesiti a kerdest? (1-igen 0-nem): ");
    bool isFinished;
    scanf("%d", &isFinished);
    q.isFinished = isFinished;

    q.createDate = time(NULL);

    FILE *File = fopen(FileName, "ab+");
    if (File == NULL)
    {
        perror("Hiba!\n");
        return false;
    }

    if (fwrite(&q, sizeof(struct Question), 1, File) != 1)
    {
        perror("Sikertelen volt a fajliras!\n");
        fclose(File);
        return false;
    }

    free(last);
    fclose(File);
    return true;
}

bool CreateFile(char FileName[32])
{
    struct stat s;
    if (stat(FileName, &s) == 0)
    {
        printf("A fajl mar letre lett hozva.\n");
        return true;
    }

    FILE *file;
    file = fopen(FileName, "wb+");
    if (file == NULL)
    {
        perror("Sikertelen a beolvasas!\n");
        fclose(file);
        return false;
    }
    fclose(file);
    printf("Fajl sikeresen letrehozva.\n");
    return true;
}

bool WriteToFile(ListNode *head, char FileName[32])
{
    FILE *File = fopen(FileName, "wb");
    if (File == NULL)
    {
        perror("Sikertelen a beolvasas!\n");
        return false;
    }

    ListNode *akt = head;

    while (akt->next != NULL)
    {
        akt = akt->next;
        if (fwrite(&akt->q, sizeof(struct Question), 1, File) != 1)
        {
            perror("Nem sikerult a fajlba iras...\n");
            fclose(File);
            return false;
        }
    }

    fclose(File);
    return true;
}

bool ReadFile(ListNode *head, char *FileName)
{
    ListNode *akt = head;
    struct Question *q = malloc(sizeof(struct Question));

    FILE *File = fopen(FileName, "rb");
    if (File == NULL)
    {
        perror("Sikertelen a beolvasas!\n");
        return false;
    }

    fseek(File, 0, SEEK_SET);
    while (!feof(File))
    {
        fread(q, sizeof(struct Question), 1, File);
        if (!feof(File))
        {
            akt->next = malloc(sizeof(ListNode));
            akt = akt->next;
            akt->q = *q;
            akt->next = NULL;
        }
    }

    free(q);
    fclose(File);
    return true;
}

void DeleteList(ListNode **ln)
{
    if (!*ln) return;
    DeleteList(&((*ln)->next));
    free(*ln);
    *ln = NULL;
}


bool GetList(char FileName[32])
{
    struct Question *q = malloc(sizeof(struct Question));

    FILE *File = fopen(FileName, "rb");
    if (File == NULL)
    {
        perror("Nem sikerult megnyitni a fajlt!\n");
        return false;
    }

    int i = 0;
    while (!feof(File))
    {
        fread(q, sizeof(struct Question), 1, File);
        if (!feof(File))
        {
            int j;
            struct tm *timeInfo = localtime(&q->createDate);
            char *isFinal = q->isFinished ? "vegleges" : "nem vegleges";
            printf("%i.  %s (%s) - %s", i+1, q->text, isFinal, asctime(timeInfo));
            for (j = 0; j < q->answNum; ++j)
                printf("\t%i. %s\n", j+1, q->answers[j]);
            ++i;
        }
    }

    free(q);
    fclose(File);
    return true;
}

bool Edition(char FileName[32])
{
    if (!GetList(FileName))
    {
        perror("Nem sikerult megnyitni a fajlt");
        return false;
    }

    ListNode *head = NULL;
    head = malloc(sizeof(ListNode));
    head->next = NULL;

    if (ReadFile(head, FileName))
    {
        printf("Modositani kivant kerdes szama: ");
        int questionNum;
        scanf("%d", &questionNum);
        ListNode *akt = head;
        if (akt == NULL)
        {
            printf("Ures a fajl!\n");
            DeleteList(&head);
            free(head);
            return false;
        }
        int i = 0;
        while (i < questionNum && akt->next != NULL)
        {
            akt = akt->next;
            ++i;
        }
        if (i != questionNum || questionNum < 1)
        {
            printf("Ilyen sorszamu kerdes nem talalhato!\n");
            DeleteList(&head);
            free(head);
            return false;
        }
        int selected;
        char text[80];
        printf("Mit szeretne modositani: 1) Kerdes, 2) Valasz, 3) Vegleges ");
        scanf("%i", &selected);
        int answer = 0;
        switch (selected)
        {
        case 1:
            printf("Adja meg az uj kerdest: ");
            scanf(" %[^\n]s", &text);
            strcpy(akt->q.text, text);
            break;
        case 2:
            while (answer < 1 || answer > akt->q.answNum)
            {
                printf("Melyik valaszt kivanja felulirni?");
                scanf("%i", &answer);
                if (answer < 1 || answer > akt->q.answNum)
                    printf("Nem talalhato ilyen szamu valasz!\n");
            }

            printf("Adja meg az uj valaszt: ");
            scanf(" %[^\n]s", &text);
            strcpy(akt->q.answers[answer-1], text);
            break;
        case 3:
            printf("Vegleges? (1-igen 0-nem): ");
            bool isFinished;
            scanf("%i", &isFinished);
            akt->q.isFinished = isFinished;
            break;
        default:
            printf("Nincs ilyen opcio!\n");
        }
        if (!WriteToFile(head, FileName))
        {
            printf("Fajliras sikertelen!\n");
            DeleteList(&head);
            free(head);
            return false;
        }
        printf("Fajliras sikeres!\n");
    }
    else 
    {
        perror("Nem sikerult beolvasni a fajlt!");
        DeleteList(&head);
        free(head);
        return false;
    }

    DeleteList(&head);
    free(head);
    return true;
}

bool DeleteQuestion(char FileName[32])
{
    if (!GetList(FileName))
    {
        perror("Fajl megniytasa sikertelen!");
        return false;
    }

    ListNode *head = NULL;
    head = malloc(sizeof(ListNode));
    head->next = NULL;

    if (ReadFile(head, FileName))
    {
        ListNode *akt = head;
        if (akt == NULL)
        {
            printf("Ures a fajl!\n");
            DeleteList(&head);
            free(head);
            return false;
        }
        printf("Hanyas kerdest kivanja torolni? ");
        int questionNum;
        scanf("%i", &questionNum);
        int i = 1;
        while (i < questionNum && akt->next != NULL)
        {
            akt = akt->next;
            ++i;
        }
        if (akt->next == NULL || questionNum < 1)
        {
            printf("Nincs ilyen sorszamu kerdes.\n");
            DeleteList(&head);
            free(head);
            return false;
        }
        
        ListNode *ptr = akt->next;
        akt->next = ptr->next;
        free(ptr);
		
        if (!WriteToFile(head, FileName))
        {
            printf("Fajliras sikertelen!\n");
            DeleteList(&head);
            free(head);
            return false;
        }
    }
    else
    {
        perror("Nem sikerult beolvasni a fajlt!");
        DeleteList(&head);
        free(head);
        return false;
    }

    return true;
}

void menu()
{
	printf("\n\n   MLSZ - Nemzeti Konzultacio\n\n");
    int select;
    char FileName[32] = "Consultation";
    CreateFile(FileName);

    do
	{
        printf("\nKerem valasszon az alabbi menupontok kozul\n\n 1. Uj Kerdes hozzaadasa\n 2. Kerdes modositasa\n 3. Kerdes(ek) listazasa\n 4. Kerdes torlese\n 0. Kilepes\n");
        scanf("%d", &select);
        switch (select)
        {
        case 1:
            if (AddQuestion(FileName))
                printf("Kerdes hozzaadasa sikeres.\n");
            else
                printf("Kerdes hozzaadasa sikertelen.\n");
            break;
		case 2:
            if (Edition(FileName))
                printf("Modositas sikeres.\n");
            else
                printf("Modositas sikertelen.\n");
            break;
        case 3:
            if (!GetList(FileName))
                printf("Nem sikerult listazni :(\n");
            break;
        case 4:
            if (DeleteQuestion(FileName))
                printf("Torles megtortent.\n");
            else
                printf("Torles sikertelen.\n");
            break;
        case 0:
            break;
        default:
            printf("Nincs ilyen opcio!\n");
        }
	}while (select != 0);
}

int main()
{
   menu();
   return 0;
}