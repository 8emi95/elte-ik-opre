#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <errno.h>     //perror, errno
#include <string.h>    //strchr, strlen
#include <time.h>      //time, asctime
#include <sys/stat.h>  //stat

typedef int bool;
#define true 1
#define false 0

struct Question
{
    char Text[128];
    int AnswerNumber;
    char Answers[4][128];
    time_t Created;
    bool IsFinal;
};

typedef struct node
{
    struct Question val;
    struct node *next;
} Question_node_t;

bool readFileToQuestionList(Question_node_t *head, char *FileName)
{
    Question_node_t *current = head;
    struct Question *Question = malloc(sizeof(struct Question));

    FILE *File = fopen(FileName, "rb");
    if (File == NULL)
    {
        perror("Nem sikerult megnyitni a kerdeslistat!");
        return false;
    }

    fseek(File, 0, SEEK_SET);
    while (!feof(File))
    {
        fread(Question, sizeof(struct Question), 1, File);
        if (!feof(File))
        {
            current->next = malloc(sizeof(Question_node_t));
            current = current->next;
            current->val = *Question;
            current->next = NULL;
        }
    }

    free(Question);
    fclose(File);
    return true;
}

void deallocateQuestionList(Question_node_t **n)
{
    if (!*n)
        return;
    deallocateQuestionList(&((*n)->next));
    free(*n);
    *n = NULL;
}

bool WriteFileFromQuestionList(Question_node_t *head, char FileName[32])
{
    FILE *File = fopen(FileName, "wb");
    if (File == NULL)
    {
        perror("Nem sikerult megnyitni a kerdeslistat!");
        return false;
    }

    Question_node_t *current = head;

    while (current->next != NULL)
    {
        current = current->next;
        if (fwrite(&current->val, sizeof(struct Question), 1, File) != 1)
        {
            perror("Sikertelen fajliras!");
            fclose(File);
            return false;
        }
    }

    fclose(File);
    return true;
}

bool CreateFile(char FileName[32])
{
    struct stat buffer;
    if (stat(FileName, &buffer) == 0)
    {
        printf("A fajl mar letezik.");
        return true;
    }

    FILE *file;
    file = fopen(FileName, "wb+");
    if (file == NULL)
    {
        perror("Hiba tortent a fajl megnyitasa kozben!\n");
        fclose(file);
        return false;
    }
    fclose(file);
    printf("Fajl letrehozva.");
    return true;
}

bool NewQuestion(char FileName[32])
{
    char Text[128];
    int AnswerNumber = 0;

    struct Question Question;
    struct Question *lastQuestion = malloc(sizeof(struct Question));

    int i;

    printf("Kerdes: ");
    scanf(" %[^\n]s", &Text);
    strcpy(Question.Text, Text);

    while (AnswerNumber < 2 || AnswerNumber > 4)
    {
        printf("Valaszok szama: ");
        scanf("%i", &AnswerNumber);
        if (AnswerNumber < 2 || AnswerNumber > 4)
            printf("2 es 4 kozott kell lennie!\n");
    }
    Question.AnswerNumber = AnswerNumber;

    for (i = 0; i < AnswerNumber; ++i)
    {
        char Answer[128];
        printf("%i. valasz: ", i+1);
        scanf(" %[^\n]s", &Answer);
        strcpy(Question.Answers[i], Answer);
    }

    printf("Vegleges? (igen: 1, nem: 0): ");
    bool IsFinal;
    scanf("%d", &IsFinal);
    Question.IsFinal = IsFinal;

    Question.Created = time(NULL);

    FILE *File = fopen(FileName, "ab+");
    if (File == NULL)
    {
        perror("Nincs ilyen esemeny!\n");
        return false;
    }

    if (fwrite(&Question, sizeof(struct Question), 1, File) != 1)
    {
        perror("Sikertelen fajliras!\n");
        fclose(File);
        return false;
    }

    free(lastQuestion);
    fclose(File);
    return true;
}

bool ListQuestions(char FileName[32])
{
    struct Question *question = malloc(sizeof(struct Question));

    FILE *File = fopen(FileName, "rb");
    if (File == NULL)
    {
        perror("Nem sikerult megnyitni a falt!\n");
        return false;
    }

    int i = 0;
    while (!feof(File))
    {
        fread(question, sizeof(struct Question), 1, File);
        if (!feof(File))
        {
            int j;
            struct tm *timeinfo = localtime(&question->Created);
            char *IsFinalText = question->IsFinal ? "vegleges" : "nem vegleges";
            printf("%i)  %s (%s) - %s", i+1, question->Text, IsFinalText, asctime(timeinfo));
            for (j = 0; j < question->AnswerNumber; ++j)
                printf("\t(%i) %s\n", j+1, question->Answers[j]);
            ++i;
        }
    }

    free(question);
    fclose(File);
    return true;
}

bool EditQuestion(char FileName[32])
{
    if (!ListQuestions(FileName))
    {
        perror("Nem sikerult megnyitni a kerdeslistat");
        return false;
    }

    Question_node_t *head = NULL;
    head = malloc(sizeof(Question_node_t));
    head->next = NULL;

    if (readFileToQuestionList(head, FileName))
    {
        printf("Adja meg a modositani kivant kerdes sorszamat: ");
        int OrderNumber;
        scanf("%d", &OrderNumber);
        Question_node_t *current = head;
        if (current == NULL)
        {
            printf("Ures a fajl!\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
        int i = 0;
        while (i < OrderNumber && current->next != NULL)
        {
            current = current->next;
            ++i;
        }
        if (i != OrderNumber || OrderNumber < 1)
        {
            printf("Nincs ilyen sorszamu kerdes.\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
        int choice;
        char Text[128];
        printf("Mely adatot akarja modositani: 1) Kerdes, 2) Valasz, 3) Veglegesseg: ");
        scanf("%i", &choice);
        int Answer = 0;
        switch (choice)
        {
        case 1:
            printf("Uj kerdes: ");
            scanf(" %[^\n]s", &Text);
            strcpy(current->val.Text, Text);
            break;
        case 2:
            while (Answer < current->val.AnswerNumber || Answer > current->val.AnswerNumber)
            {
                printf("Hanyas sorszamu kerdest akarja modositani: ");
                scanf("%i", &Answer);
                if (Answer < current->val.AnswerNumber || Answer > current->val.AnswerNumber)
                    printf("Nincs ilyen sorszamu valasz!\n");
            }

            printf("Uj valasz: ");
            scanf(" %[^\n]s", &Text);
            strcpy(current->val.Answers[Answer-1], Text);
            break;
        case 3:
            printf("Vegleges? (igen: 1, nem: 0): ");
            bool IsFinal;
            scanf("%i", &IsFinal);
            current->val.IsFinal = IsFinal;
            break;
        default:
            printf("Nincs ilyen menu-elem!\n");
        }
        if (!WriteFileFromQuestionList(head, FileName))
        {
            printf("Fajliras sikertelen!\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
        printf("Fajliras sikeres!\n");
    }
    else // if (readFileToQuestionList(head, FileName))
    {
        perror("Nem sikerult beolvasni a fajlt!");
        deallocateQuestionList(&head);
        free(head);
        return false;
    }

    deallocateQuestionList(&head);
    free(head);
    return true;
}

bool DeleteQuestion(char FileName[32])
{
    if (!ListQuestions(FileName))
    {
        perror("Nem sikerult megnyitni a kerdeslistat");
        return false;
    }

    Question_node_t *head = NULL;
    head = malloc(sizeof(Question_node_t));
    head->next = NULL;

    if (readFileToQuestionList(head, FileName))
    {
        Question_node_t *current = head;
        if (current == NULL)
        {
            printf("Ures a fajl!\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
        printf("Adja meg a torolni kivant kerdes sorszamat: ");
        int OrderNumber;
        scanf("%i", &OrderNumber);
        int i = 1;
        while (i < OrderNumber && current->next != NULL)
        {
            current = current->next;
            ++i;
        }
        if (current->next == NULL || OrderNumber < 1)
        {
            printf("Nincs ilyen sorszamu kerdes.\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
        // Deleting node:
        Question_node_t *newNext = current->next;
        current->next = newNext->next;
        free(newNext);

        // Write list to file:
        if (!WriteFileFromQuestionList(head, FileName))
        {
            printf("Fajliras sikertelen!\n");
            deallocateQuestionList(&head);
            free(head);
            return false;
        }
    }
    else // if (readFileToQuestionList(head, FileName))
    {
        perror("Nem sikerult beolvasni a fajlt!");
        deallocateQuestionList(&head);
        free(head);
        return false;
    }

    return true;
}

int main()
{
    printf("\n\n== Magyar Labdajátékok Szervezete (MLSZ) ==\n\tNemzeti Konzultáció\n");
    int selectedItem;
    char FileName[32] = "QuestionList";
    CreateFile(FileName);

    do
    {
        printf("\n--Menu--\n 0) Kilepes\n 1) Kerdes felvetele\n 2) Kerdes modositasa\n 3) Kerdes torlese\n 4) Kerdessor listazasa\n");
        scanf("%d", &selectedItem);
        switch (selectedItem)
        {
        case 1:
            if (NewQuestion(FileName))
                printf("Kerdes felvetele sikeres.\n");
            else
                printf("Kerdes felvetele sikertelen.\n");
            break;
        case 4:
            if (!ListQuestions(FileName))
                printf("Listazas sikertelen.\n");
            break;
        case 2:
            if (EditQuestion(FileName))
                printf("Modositas sikeres.\n");
            else
                printf("Modositas sikertelen.\n");
            break;
        case 3:
            if (DeleteQuestion(FileName))
                printf("Torles sikeres.\n");
            else
                printf("Torles sikertelen.\n");
            break;
        case 0:
            break;
        default:
            printf("Nincs ilyen menu-elem!\n");
        }
    } while (selectedItem != 0);

    return 0;
}