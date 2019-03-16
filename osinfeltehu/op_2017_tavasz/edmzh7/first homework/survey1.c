/*
   Dimitar Egumenovski
   EDMZH7
   First homework OS
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

const int ql = 250; // question length
const int al = 250; // answer length
const int maxAnswerNumber = 4;

struct Question
{
  bool isPermanent;
  int numOfAns, qsl, ansl[4];
  char qs[250];   // question string
  char ans[4][250]; // answers
  char lmod[25]; // last modified date
} q[500];

int qIndex = 0;

void readQuestions()
{
    FILE* f, * g;
    f = fopen("questions.txt", "r");
    if(f == NULL) // if there are no previous questions
    {
        fclose(f);
        g = fopen("questions.txt", "w"); // then create
        if(g == NULL) // some error happened when creating the file
        {
            perror("Error at creating the file!\n");
            fclose(g);
            exit(1);
        }
        fclose(g);
    }
    else
    {
        
        int noq = 0; // number of questions
        fscanf(f, "%d", &noq);

        char c;
        fgetc(f); // newline
        fgetc(f); // reading the delimiter for better readability of the source file
        fgetc(f); // newline

        // reading the questions
        int i, j, k, r;
        for(i = 0; i < noq; ++i)
        {
            // reading the number of given answers for the actual question
            fscanf(f, "%d", &q[qIndex].numOfAns);

            fgetc(f); // reading the new line character

            // reading the question
            for(k = 0; k < ql; ++k) { q[qIndex].qs[k] = fgetc(f);}
            // getting the real length of the question
            k = ql-1;
            while(q[qIndex].qs[k] == ' ') {--k;}
            q[qIndex].qsl = k+1;

            //reading the given number of answers
            for(j = 0; j < q[qIndex].numOfAns; ++j)
            {
                fgetc(f); // reading the new line character
                // one line of answer
                for(r = 0; r < al; ++r) { q[qIndex].ans[j][r] = fgetc(f); }

                // getting the real length of the answer
                r = al-1;
                while(q[qIndex].ans[j][r] == ' ') {--r;}
                q[qIndex].ansl[j] = r+1;
            }

            fgetc(f); // reading the new line character
            fgets(q[qIndex].lmod, 25, f); // reading the date modified
            q[qIndex].isPermanent = false; // the question is permanent or not

            fgetc(f); // reading the delimiter for better readability of the source file
            fgetc(f); // newline

            // the next question to be stored as the next element of the question array
            ++qIndex;
        }
    }
    fclose(f);
}

void printOutQuestionsFormatted()
{
    int i, j, k;// read;
    for(i = 0; i < qIndex; ++i)
    {
        printf("%d. ", i+1); // question number
        for(j = 0; j < q[i].qsl; ++j) { printf("%c", q[i].qs[j]); }// question
        printf("\n\n");
        for(k = 0; k < q[i].numOfAns; ++k)
        {
            printf("   %d. ", k+1); // answer number
            for(j = 0; j < q[i].ansl[k]; ++j) { printf("%c", q[i].ans[k][j]); } // answer
            printf("\n");
        }
       // printf("  Final: "); q[i].isPermanent ? printf("Yes") : printf("No");
        printf("  Last modify: %s\n\n", q[i].lmod);
    }
}

void powerOn()
{
    printf("WELCOME TO\n");
    printf("THE SOCIETY OF HUNGARIAN BALL GAMES\n");
    printf("SURVEY\n\n");


    readQuestions();
}

void showMenu()
{
    printf("MENU:\n");
    printf("   1. INSERT  QUESTION\n");
    printf("   2. MODIFY QUESTION\n");
    printf("   3. DELETE QUESTION\n");
    printf("   4. LIST QUESTIONS\n");
    printf("   5. FINILIZE QUESTION\n");
}

bool properContinueAnswer(char* ch)
{
    if(*ch == 'i' || *ch == 'n')
    {
        if(*ch == 'i') {*ch = 'y'; }
        return true;
    }
    else if(*ch == 'y') { return true; }
    while(fgetc(stdin) != '\n');
    return false;
}
void askForRunAgain(char* ch)
{
    printf("Do u want to back in menu?\n");
    printf("   i - yes / n - no\n");
    do
    {
        printf("> ");
        int result = scanf(" %c", ch);
        if(result == EOF) printf("Something wrong happened.");
    } while(!properContinueAnswer(ch));
    while(fgetc(stdin) != '\n');
    printf("\n");
}

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
int getLine (char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL)
    {
        // printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n')
    {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

bool properLine(int mySwitch, int index, char whatImTesting)
{
    // whatImTesting - q = question
    //               - 1 .. 4 (0..3) = answer index
    if(mySwitch == 0)
    {
        if(whatImTesting == 'q')
        {
            if(strlen(q[index].qs) < 2) return false;
        }
        else if(whatImTesting >= '0' && whatImTesting <= '3')
        {
            if(strlen(q[index].ans[whatImTesting - '0']) < 2) return false;
        }
        return true;
    }
    return false;
}

void getAnswerSentences(int index, int* qNums)
{
    int i, count = 1, read;
    for(i = 0; i < maxAnswerNumber; ++i)
    {
        if(qNums[i] > 0)
        {
            ++count;
            printf("%d. Answer:\n", i+1);
            do
            {
                printf("> ");
                read = getLine("prompt", q[index].ans[i], sizeof(q[index].ans[i]));
            } while(!properLine(read, index, '0'+i));

            q[index].ansl[i] = strlen(q[index].ans[i]);
            for(read = q[index].ansl[i]; read < al; ++read) { q[index].ans[i][read] = ' '; }
        }
        if(count > q[index].numOfAns) { break; }
    }
}

void getQuestionSentence(int index)
{
    int read;
    printf("Insert the question:\n");
    do
    {
        printf("> ");
        read = getLine("prompt", q[index].qs, sizeof(q[index].qs));
    } while(!properLine(read, index, 'q'));

    q[index].qsl = strlen(q[index].qs);
    for(read = q[index].qsl; read < ql; ++read) { q[index].qs[read] = ' '; }
}

void setModifyingTime(int index)
{
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    strcpy(q[index].lmod, asctime(timeinfo));
    if(q[index].lmod[ strlen(q[index].lmod) - 1 ] == '\n')
    {
        q[index].lmod[ strlen(q[index].lmod) - 1 ] = '\0';
    }
}

void printQuestion(int index)
{
    int i, read;
    for(read = 0; read < q[index].qsl; ++read) { printf("%c", q[index].qs[read]); }
    printf("\n");

    for(i = 0; i < q[index].numOfAns; ++i)
    {
        printf("   %d. ", i+1);
        for(read = 0; read < q[index].ansl[i]; ++read) { printf("%c", q[index].ans[i][read]); }
        printf("\n");
    }
    printf("\n");
}

void getQuestion(int index)
{
    int i, read;
    getQuestionSentence(index);

    q[index].isPermanent = false;

    printf("\nHow much answer u gonna write?\n");
    q[index].numOfAns = -1;
    do
    {
        printf("> ");
        int result = scanf("%d", &q[index].numOfAns);
        if(result == 0) {while(fgetc(stdin) != '\n');}
        if(result == EOF) printf("Something happened wrong.");
    } while(!(q[index].numOfAns > 1 && q[index].numOfAns < 5));

    while(fgetc(stdin) != '\n'); // reading the end line character to not show twice the console log

    int t[4]={1,1,1,1};
    getAnswerSentences(index, t);

    // saving the time of creating
    setModifyingTime(index);

    printf("\nYour question:\n");
    printQuestion(index);
}

void addQuestion()
{
    if(qIndex < 499)
    {
        getQuestion(qIndex);
        ++qIndex; // the number of questions is increased
        // printOutQuestionsFormatted();
    }
    else
    {
        printf("There is no more space!\n");
    }
}

int getBetween(int he, int le) // le = lower or equal; he = higher or equal
{
    int answer;
    do
    {
        printf("> ");
        int result = scanf("%d", &answer);
        if(result == 0) {while(fgetc(stdin) != '\n');}
        if(result == EOF) { printf("Something went wrong."); }
        while(fgetc(stdin) != '\n');
    } while(!(answer >= he && answer <= le));
    // while(fgetc(stdin) != '\n');
    return answer;
}

int selectModifyQuestion()
{
    printf("The questions:\n\n");
    printOutQuestionsFormatted();
    printf("Insert question number which u want to modify:\n");

    int result = getBetween(1, qIndex);

    --result;

    printf("\nThe chosen question to modify:\n");
    printQuestion(result);
    return result;
}

bool minGoodMods(int* t, int start, int end)
{
    int i, j;
    if(start != end)
    {
        for(i = start; i < end; ++i)
        {
            if(t[i] == 0)
            {
//                for(j = 0; j < 4; ++j) printf("%d", t[j]);
                printf("Wrong ! insert all items\n");
                return false;
            }
        }
    }
    return true;
}

void modifyThatQuestion(int index)
{
    
    
    printf("Insert 1 (yes) or 0 (no) !\n");
    printf("\nDo u want to modify question contex?\n");
    
    int answer = getBetween(0, 1);
    if(answer == 1) { getQuestionSentence(index); }

    printf("\nDo u want to change answer?\n");
    answer = getBetween(0, 1);

    if(answer == 1)
    {
        printf("\nDo u want to change the number of answer?\n");
        int oldNumOfA = q[index].numOfAns;
        if(getBetween(0, 1) == 1)
        {
            printf("\nHow many?\n");
            q[index].numOfAns = getBetween(2, 4);
        }

        int t[4] = {0, 0, 0, 0};
        if(oldNumOfA > q[index].numOfAns) // if the number of answers decreased
        {
            int i;
            for(i = 0; i < oldNumOfA; ++i)
            {
                strcpy(q[index].ans[i], "");
                q[index].ansl[i] = 0;
            }
            for(i = 0; i < maxAnswerNumber; ++i) { t[i] = 1; }
        }
        else
        {
            printf("\nWhich answer u want to modify?\n");
            printf("Write 0 or 1 and space which answer u want to modify!\n");
            printf("> ");
            int i;
            do
            {
                for(i = 0; i < q[index].numOfAns; ++i) scanf(" %d", &t[i]);
                while(fgetc(stdin) != '\n');
            } while( !(minGoodMods(t, oldNumOfA, q[index].numOfAns)) );
        }
        getAnswerSentences(index, t);
    }

    printf("\nDo u want to save question?\n");
    q[index].isPermanent = getBetween(0, 1);

    setModifyingTime(index);
}

void modifyQuestion()
{
    if(qIndex > 0)
    {
        int qn; // question number
        qn = selectModifyQuestion();
        modifyThatQuestion(qn);
        printQuestion(qn);
    }
    else printf("Emty question!\n");
}

int selectDeleteQuestion()
{
    printf("The question:\n\n");
    printOutQuestionsFormatted();
    printf("Insert which question u want to delete:\n");

    int result = getBetween(1, qIndex);

    printf("\nDeleted question:\n");
    printQuestion(result-1);
    return result;
}

void deleteNthQuestion(int qn)
{
    q[qn].isPermanent = false;
    q[qn].numOfAns = 0;
    q[qn].qsl = 0;
    strcpy(q[qn].qs, "");
    strcpy(q[qn].lmod, "");
    int i, j;
    for(i = 0; i < 4; ++i)
    {
        q[qn].ansl[i] = 0;
        strcpy(q[qn].ans[i], "");
    }
    for(i = qn+1; i < qIndex; ++i)
    {
        q[i-1].isPermanent = q[i].isPermanent;
        q[i-1].numOfAns = q[i].numOfAns;
        q[i-1].qsl = q[i].qsl;
        strcpy(q[i-1].qs, q[i].qs);
        strcpy(q[i-1].lmod, q[i].lmod);
        for(j = 0; j < 4; ++j)
        {
            q[i-1].ansl[j] = q[i].ansl[j];
            strcpy(q[i-1].ans[j], q[i].ans[j]);
        }
    }


    --qIndex;
}
void deleteQuestion()
{
    if(qIndex > 0)
    {
        int qn = selectDeleteQuestion();

        int numOfAns, qsl, ansl[4];
        char qs[250];   // question string
        char ans[4][250]; // answers
        char lmod[25]; // last modified date

        --qn;
        deleteNthQuestion(qn);
    }
    else
    {
        printf("There is not question with that number!");
    }
}

//void writeDataToFile(const char* fileName, int* array)
void writeDataToFile(const char* fileName, char mode)
{
    FILE* g = fopen(fileName, "w");

    int i, j, k = 0;
    if(mode == 'p')
    {
        for(i = 0; i < qIndex; ++i)
        {
            if(q[i].isPermanent == true) ++k;
        }
        fprintf(g, "%d\n-\n", k);
    }
    else // if(mode == 'n')
    {
        fprintf(g, "%d\n-\n", qIndex);
    }

    for(i = 0; i < qIndex; ++i)
    {
        if(mode == 'p')
        {
            if(q[i].isPermanent == true)
            {
                fprintf(g, "%d\n", q[i].numOfAns);
                for(k = 0; k < ql; ++k) fprintf(g, "%c", q[i].qs[k]);
                fprintf(g, "\n");
                for(j = 0; j < q[i].numOfAns; ++j)
                {
                    for(k = 0; k < ql; ++k) fprintf(g, "%c", q[i].ans[j][k]);
                    fprintf(g, "\n");
                }
                fprintf(g, "%s\n", q[i].lmod);
                fprintf(g, "-\n");
            }
        }
        else // if(mode == 'n')
        {
            fprintf(g, "%d\n", q[i].numOfAns);
            for(k = 0; k < ql; ++k) fprintf(g, "%c", q[i].qs[k]);
            fprintf(g, "\n");
            for(j = 0; j < q[i].numOfAns; ++j)
            {
                for(k = 0; k < ql; ++k) fprintf(g, "%c", q[i].ans[j][k]);
                fprintf(g, "\n");
            }
            fprintf(g, "%s\n", q[i].lmod);
            fprintf(g, "-\n");
        }
    }
}

void finalizeQuery()
{
    if(qIndex > 0){
        printf("Do u want to add this question to FInal Que File!\n");
        printf("   1 (yes) | 0 (no)\n\n");

//        int* req = malloc(qIndex * sizeof(int)); // required question numbers
        int i, reqL = 0;
        for(i = 0; i < qIndex; ++i)
        {
            printf("%2d. ", i+1);
            printQuestion(i);
            printf("Answer? ");
//            scanf("%d", &req[i]);
            scanf("%d", &q[i].isPermanent);
            while(fgetc(stdin) != '\n');
            printf("\n");
//            q[i].isPermanent = req[i];
        }



//        writeDataToFile("questionnaire.txt", req);
        writeDataToFile("questionnaire.txt", 'p'); // p, mint permanent
//        free(req);
    }
    else
    {
        printf("There is not question with this number!");
    }
}

void execute(int option)
{
//    printf("Chosen option: %d\n", option);
    printf("\n");
    switch(option)
    {
        case 1: addQuestion(); break;
        case 2: modifyQuestion(); break;
        case 3: deleteQuestion(); break;
        case 4: printOutQuestionsFormatted(); break;
        case 5: finalizeQuery(); break;
        default: printf("Unknown option chosen.");
    }
}

void shutDown()
{
//    int* t = malloc(sizeof(int) * qIndex);
//    int i;
//    for(i = 0; i < qIndex; ++i) { t[i] = 1; }
    writeDataToFile("questions.txt", 'n'); // n = not permanent
//    free(t);
    printf("Goodbye... Tnx for using!\n");
}

int main()
{
    powerOn();
    char c = 'y';
    while(c == 'y')
    {
        showMenu();
        int option;
        option = getBetween(1, 5);
        execute(option);
        askForRunAgain(&c);
    }
    shutDown();
}
