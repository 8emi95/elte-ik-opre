#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
typedef struct Kerdoiv {
    struct Kerdesek* 	head;
    size_t size;
} Kerdoiv;
 
typedef struct Kerdesek {
    int                 answnumber;
    char                question[100];
    char                answers[4][100];
    struct Kerdesek*  	next;
} Kerdesek;
   
int deleteQuestion(Kerdoiv* kerdo, unsigned int nodeNumber) {
 
    unsigned int lastnodeNumber = kerdo->size;
    if (nodeNumber > lastnodeNumber || nodeNumber == 0)
        return -1;
 
    if (nodeNumber == 1) {
        Kerdesek* p = kerdo->head->next;
        free(kerdo->head);
        kerdo->head = p;
    }
    else if (nodeNumber == lastnodeNumber) {
        Kerdesek** secondToLast = &(kerdo->head);
 
        while (((*secondToLast)->next)->next != NULL) {
            secondToLast = &((*secondToLast)->next);
        }
        free((*secondToLast)->next);
        (*secondToLast)->next = NULL;
    }
    else {
        Kerdesek* nodeBeforeDelete = kerdo->head;
        unsigned int i;
        for (i = 2; i < nodeNumber; i++) {
            nodeBeforeDelete = nodeBeforeDelete->next;
        }
        Kerdesek* nodeAfterDelete = nodeBeforeDelete->next->next;
        free(nodeBeforeDelete->next);
        nodeBeforeDelete->next = nodeAfterDelete;
    }
    kerdo->size--;
    return 0;
}
 
int modifyQuestion(Kerdoiv* kerdo, unsigned int nodeNumber, char* newQuestion) {
    unsigned int lastnodeNumber = kerdo->size;
    if (nodeNumber > lastnodeNumber|| nodeNumber == 0 || strlen(newQuestion) > 100)
        return -1;
 
    Kerdesek* p = kerdo->head;
    unsigned int i;
    for (i = 1; i < nodeNumber; i++)
        p = p->next;
 
    if (strcpy(p->question, newQuestion) != NULL)
        return 0;
    else
        return -1;
}
 
int modifyAnswer(Kerdesek* kerd, unsigned int ansNumber, char* newAnswer) {
   
    if (strcpy(kerd->answers[ansNumber-1], newAnswer) != NULL)
        return 0;
    else
        return -1;
}
 
void readString(char* string, unsigned int maxLength) {
    do {
        fgets(string, maxLength, stdin);
    } while (string[0] == '\n');
       
    char* lastChar = &string[strlen(string) - 1];
 
    if (*lastChar == '\n')
        *lastChar = '\0';
}
 
void readChar(char* c) {
    while ((*c = fgetc(stdin)) == '\n' && *c != EOF)
    ;
}
 
int readAnsNumber(unsigned int max) {
    char buffer[100];
    unsigned int ansNumber = 0;
    int tries = 0;
 
    while (ansNumber == 0 || ansNumber > max) {
        if (tries > 0) {
            printf("Egy 1-%i kozotti egesz szamot adjon meg!\n", max);
        }
        readString(buffer, sizeof(buffer));
        ansNumber = atoi(buffer);
        tries++;
    }
 
    return ansNumber;
}
 
int readAnsCount(unsigned int* ansCount) {
    char buffer[100];
    *ansCount = 0;
    int tries = 0;
 
    while (*ansCount == 0 || *ansCount < 2 || 4 < *ansCount) {
        if (tries > 0) {
            printf("Egy %i-%i kozotti egesz szamot adjon meg!\n", 2, 4);
        }
        readString(buffer, sizeof(buffer));
        *ansCount = atoi(buffer);
        tries++;
    }
 
    return 0;
}
 
void printMenu() {
    printf("\t[1] Kerdesek listazasa\n");
    printf("\t[2] Uj kerdes felvetele\n");
    printf("\t[3] Meglevo kerdes modositasa\n");
    printf("\t[4] Kerdes torlese\n");
	printf("\t[0] Kilepes\n");
}
 
int addAnswer(Kerdesek* kerd, char* answer) {
    if (kerd == NULL)
        return -1;
    if (kerd->answnumber >= 4) {
        fprintf(stderr, "Egy kerdeshez maximum %u valaszlehetoseg engedelyezett!\n", 4);
        return -1;
    }
    else {
        int i = 0;
        while (kerd->answers[i][0] != '\0') {
            i++;
        }
        strcpy(kerd->answers[i], answer);
        kerd->answnumber++;
        return 0;
    }
}
 
int deleteAnswer(Kerdesek* kerd, unsigned int ansNumber) {
    if (kerd == NULL)
        return -1;
 
    unsigned int ansCount = kerd->answnumber;
 
    if (ansCount <= 2) {
        fprintf(stderr, "Minimum %u valaszlehetoseget adjon meg!\n", 2);
        return -1;
    }
   
    unsigned int i;
    for (i = ansNumber; i < ansCount; i++) {
        strcpy(kerd->answers[i-1], kerd->answers[i]);
    }
    strcpy(kerd->answers[ansCount - 1], "\0");
    kerd->answnumber--;
 
    return 0;
}
 
int addQuestion(Kerdoiv* kerdo, char* question) {
    Kerdesek** p = &(kerdo->head);
    while (*p != NULL) {
        p = &((*p)->next);
    }
    *p = malloc(sizeof(Kerdoiv));
    (*p)->next = NULL;
    (*p)->answnumber = 0;
 
    strcpy((*p)->question, question);
 
    int i;
    for (i = 0; i < 4; i++)
        strcpy((*p)->answers[i], "\0");
 
    kerdo->size++;
    return 0;
}
 
int initializeQuestion(Kerdoiv* kerdo) {
    if (kerdo == NULL)
        return -1;
 
    kerdo->head = NULL;
    kerdo->size = 0;
 
    return 0;
};
 
void printQuestions(Kerdoiv* kerdo) {
    Kerdesek* p = kerdo->head;
    int i;
    if (p == NULL)
        printf("A kerdoiv meg ures!\n");
    while (p != NULL) {
        printf("%s\n", p->question);
        for (i = 0; i < 4; i++) {
            if (p->answers[i][0] != '\0')
                printf("%s\n", p->answers[i]);
        }
        printf("\n");
        p = p->next;
    }
}
 
Kerdesek* getLastNode(Kerdoiv* kerdo) {
    Kerdesek* p = kerdo->head;
    unsigned int i;
    for (i = 1; i < kerdo->size; i++) {
        p = p->next;
    }
 
    return p;
}
 
int menuAddAnswer(Kerdesek* kerd) {
    printf("\nMi legyen a valasz szovege?\t");
    char buffer[100];
    readString(buffer, sizeof(buffer));
    return addAnswer(kerd, buffer);
}
 
int menuAddQuestion(Kerdoiv* kerdo) {
    printf("Mi legyen az uj kerdes szovege?\t");
    char qBuffer[100];
 
    readString(qBuffer, 100);
    addQuestion(kerdo, qBuffer);
    printf("Kerdes hozzaadva!\n\n");
    printf("Hany valaszlehetoseget akar megadni(min %i, max %i)?\n", 2, 4);
    unsigned int answnumber;
    unsigned int* panswnumber = &answnumber;
    readAnsCount(panswnumber);
 
    Kerdesek* p = getLastNode(kerdo);
    unsigned int i;
    for (i = 0; i < answnumber; i++) {
        menuAddAnswer(p);
    }
   
    return 0;
}    
 
int menuDeleteQuestion(Kerdoiv* kerdo) {
    if (kerdo->size == 0) {
        printf("A kerdoiv meg ures!\n");
        return 0;
    }
    printf("Hanyadik kerdest szeretne torolni?\t");
    char buffer[16];
    readString(buffer, sizeof(buffer));
   
    if (deleteQuestion(kerdo, atoi(buffer)) == 0)
        return 0;
    else {
        printf("A torles nem sikerult.\n");
        return -1;
    }
}
 
int menuModifyAnswer(Kerdesek* kerd, unsigned int ansNumber) {
    printf("Mire modositja a kerdest?\t");
    char buffer[100];
    readString(buffer, sizeof(buffer));
    return modifyAnswer(kerd, ansNumber, buffer);
}
 
int menuEditAnswer(Kerdoiv* kerdo, unsigned int nodeNumber) {
    if (kerdo == NULL)
        return -1;
   
    Kerdesek* currentNode = kerdo->head;
    unsigned int i;
    for (i = 1; i < nodeNumber; i++)
        currentNode = currentNode->next;
 
    printf("Szeretne uj valaszt hozzaadni?(i: igen, barmi mas: nem)\t");
    char newAnswer;
    char* pnewAnswer = &newAnswer;
    readChar(pnewAnswer);
 
    while (newAnswer == 'i') {
        menuAddAnswer(currentNode);
 
        if (currentNode->answnumber < 4) {
            printf("Szeretne meg uj valaszt hozzaadni?(i: igen, barmi mas: nem)\t");
            readChar(pnewAnswer);
        }
        else
            newAnswer = 'n';
    }
    printf("Szeretne meg valtoztatni a valaszokon? (t: torles, m: modositas, mas: befejezes )\n\t");
    char delOrModify;
    char* pdelOrModify = &delOrModify;
    readChar(pdelOrModify);
 
    if (delOrModify != 't' && delOrModify != 'm')
        return 0;
    else {
        printf("Hanyadik valaszt szeretne ");
        (delOrModify == 't') ? (printf("torolni? ")) : (printf("modositani? "));
        printf("(jelenleg %i  valasz van a kerdesre)\n", currentNode->answnumber);
 
        int ansNumber = readAnsNumber(currentNode->answnumber);
 
        if (delOrModify == 't')
            return deleteAnswer(currentNode, ansNumber);
        else
            return menuModifyAnswer(currentNode, ansNumber);
    }
}
 
int menuModifyQuestion(Kerdoiv* kerdo, unsigned int nodeNumber) {
    printf("Mire szeretne modositani a kerdest?\t");
    char buffer[100];
   
    readString(buffer, sizeof(buffer));
    return modifyQuestion(kerdo, nodeNumber, buffer);
}
 
int menuModifyQuestions(Kerdoiv* kerdo) {
    if (kerdo->head == NULL) {
        printf("A kerdoiv meg ures!\n");
        return -1;
    }
    printf("Hanyadik kerdesnek a szovegen vagy a valaszain szeretne modositani?\t");
    char buffer[100];
    int nodeNumber = 0;
 
    while (nodeNumber == 0) {
        readString(buffer, sizeof(buffer));
        nodeNumber = atoi(buffer);
    }
 
    printf("Szeretne a kerdes szovegen valtoztatni?(i:igen, mas:nem)\t");
    char changeQuestion;
    char* pchangeQuestion = &changeQuestion;
   
    readChar(pchangeQuestion);
    int returnVal = 0;
   
    if (changeQuestion == 'i')
        returnVal = menuModifyQuestion(kerdo, nodeNumber);
   
    if (returnVal == 0) {
        char editAnswers = 'i';
        while (editAnswers == 'i') {
            printf("Szeretne a valaszokon modositani? (i:igen, mas: nem)\t");
           
            char* peditAnswers = &editAnswers;
            readChar(peditAnswers);
 
            if (editAnswers == 'i')
                returnVal = menuEditAnswer(kerdo, nodeNumber);
        }
    }
 
    return returnVal;
}
 
void delQuestions(Kerdoiv* kerdo) {
    Kerdesek* current = kerdo->head;
    while (current != NULL) {
        Kerdesek* next = current->next;
        free(current);
        current = next;
    }
    kerdo->head = 0;
}
 
int main() {
    Kerdoiv fociKerdesek;
    Kerdoiv* pfociKerdesek = &fociKerdesek;
    initializeQuestion(pfociKerdesek);
    int menu = 1;
 
    while (menu) {
        printMenu();
        char menuOption;
        char* pMenuOption = &menuOption;
        readChar(pMenuOption);
 
        switch (menuOption) {
        case '0':
            menu = 0;
            break;
        case '1':
            printQuestions(pfociKerdesek);
            break;
        case '2':
            menuAddQuestion(pfociKerdesek);           
            break;
        case '3':
            if (pfociKerdesek->head != NULL)
                printQuestions(pfociKerdesek);
            menuModifyQuestions(pfociKerdesek);
            break;
        case '4':
            if (pfociKerdesek->head != NULL)
                printQuestions(pfociKerdesek);
            menuDeleteQuestion(pfociKerdesek);
            break;
        default:
            printf("Rossz szamot adott meg!\n");
            break;
        }
    }
 
    delQuestions(pfociKerdesek);
 
    printf("Koszonjuk, hogy segitette a munkankat!\n");
    return 0;
}