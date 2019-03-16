/*

Keszitette: Boros Attila Peter
Datum: 2017. aprilis 9.
Neptun-kod: ENJIPB

A program rovid leirasa:
A program mukodese soran indulaskor a a questions.txt fajlbol olvassa ki a mar meglevo kerdeseket. Ha ez a fajl nem letezik, akkor a program letrehozza.
A vegleges kerdeseket, amelyek a "kerdoivben szerepelni fognak" veglegesiteskor a questionnaire.txt nevu fajlba menti ki a program.
A programmal lehetosege van a felhasznalonak uj kerdeseket letrehozni,
                                             mar meglevo kerdeseket szekeszteni, illetve torolni
                                             listazni a mar meglevo kerdeseket
                                             valamint veglegesiteni a kerdoivet, ami soran a questionnaire.txt fajlba menti ki a kivalasztott kerdeseket es megjeloli az adott kerdest.
A program leallasakor a meglevo kerdeseket a questions.txt -be menti ki.

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <poll.h>

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
    char qs[250];     // question string
    char ans[4][250]; // answers
    char lmod[25];    // last modified date
} q[500];

int qIndex = 0;

void readQuestions()
{
    FILE *f, *g;
    f = fopen("questions.txt", "r");
    if (f == NULL) // if there are no previous questions
    {
        fclose(f);
        g = fopen("questions.txt", "w"); // then create
        if (g == NULL)                   // some error happened when creating the file
        {
            perror("Error at creating the file!\n");
            fclose(g);
            exit(1);
        }
        fclose(g);
    }
    else
    {
        // beolvasas
        int noq = 0; // number of questions
        fscanf(f, "%d", &noq);

        fgetc(f); // newline
        fgetc(f); // reading the delimiter for better readability of the source file
        fgetc(f); // newline

        // reading the questions
        int i, j, k, r;
        for (i = 0; i < noq; ++i)
        {
            // reading the number of given answers for the actual question
            fscanf(f, "%d", &q[qIndex].numOfAns);

            fgetc(f); // reading the new line character

            // reading the question
            for (k = 0; k < ql; ++k)
            {
                q[qIndex].qs[k] = fgetc(f);
            }
            // getting the real length of the question
            k = ql - 1;
            while (q[qIndex].qs[k] == ' ')
            {
                --k;
            }
            q[qIndex].qsl = k + 1;

            //reading the given number of answers
            for (j = 0; j < q[qIndex].numOfAns; ++j)
            {
                fgetc(f); // reading the new line character
                // one line of answer
                for (r = 0; r < al; ++r)
                {
                    q[qIndex].ans[j][r] = fgetc(f);
                }

                // getting the real length of the answer
                r = al - 1;
                while (q[qIndex].ans[j][r] == ' ')
                {
                    --r;
                }
                q[qIndex].ansl[j] = r + 1;
            }

            fgetc(f);                      // reading the new line character
            fgets(q[qIndex].lmod, 25, f);  // reading the date modified
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
    int i, j, k; // read;
    for (i = 0; i < qIndex; ++i)
    {
        printf("%d. ", i + 1); // question number
        for (j = 0; j < q[i].qsl; ++j)
        {
            printf("%c", q[i].qs[j]);
        } // question
        printf("\n\n");
        for (k = 0; k < q[i].numOfAns; ++k)
        {
            printf("   %d. ", k + 1); // answer number
            for (j = 0; j < q[i].ansl[k]; ++j)
            {
                printf("%c", q[i].ans[k][j]);
            } // answer
            printf("\n");
        }
        printf("  Vegleges: ");
        q[i].isPermanent ? printf("igen") : printf("nem");
        printf("  Utolso modositas: %s\n\n", q[i].lmod);
    }
}

void powerOn()
{
    printf("Jo napot!\n");
    printf("   Ezen program segitsegevel szekresztheti a kerdoivet :-)\n\n");
    // adatok beolvasasa
    readQuestions();
}

void showMenu()
{
    printf("Valasszon menupontot:\n");
    printf("   1. Uj kerdes hozzaadasa\n");
    printf("   2. Kerdes szerkesztese\n");
    printf("   3. Kerdes torlese\n");
    printf("   4. Kerdesek listazasa\n");
    printf("   5. Kerdesek veglegesitese\n");
}

bool properContinueAnswer(char *ch)
{
    if (*ch == 'i' || *ch == 'n')
    {
        if (*ch == 'i')
        {
            *ch = 'y';
        }
        return true;
    }
    else if (*ch == 'y')
    {
        return true;
    }
    while (fgetc(stdin) != '\n')
        ;
    return false;
}

void askForRunAgain(char *ch)
{
    printf("Folytatja a kerdoiv szerkeszteset?\n");
    printf("   i - igen / n - nem\n");
    do
    {
        printf("> ");
        int result = scanf(" %c", ch);
        if (result == EOF)
            printf("Something wrong happened.");
    } while (!properContinueAnswer(ch));
    // biztonsag kedveert :-)
    while (fgetc(stdin) != '\n')
        ;
    printf("\n");
}

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2
int getLine(char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL)
    {
        // printf ("%s", prmpt);
        fflush(stdout);
    }
    if (fgets(buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff) - 1] != '\n')
    {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff) - 1] = '\0';
    return OK;
}

bool properLine(int mySwitch, int index, char whatImTesting)
{
    // whatImTesting - q = question
    //               - 1 .. 4 (0..3) = answer index
    if (mySwitch == 0)
    {
        if (whatImTesting == 'q')
        {
            if (strlen(q[index].qs) < 2)
                return false;
        }
        else if (whatImTesting >= '0' && whatImTesting <= '3')
        {
            if (strlen(q[index].ans[whatImTesting - '0']) < 2)
                return false;
        }
        return true;
    }
    return false;
}

void getAnswerSentences(int index, int *qNums)
{
    int i, count = 1, read;
    for (i = 0; i < maxAnswerNumber; ++i)
    {
        if (qNums[i] > 0)
        {
            ++count;
            printf("%d. valasz:\n", i + 1);
            do
            {
                printf("> ");
                read = getLine("prompt", q[index].ans[i], sizeof(q[index].ans[i]));
            } while (!properLine(read, index, '0' + i));

            q[index].ansl[i] = strlen(q[index].ans[i]);
            for (read = q[index].ansl[i]; read < al; ++read)
            {
                q[index].ans[i][read] = ' ';
            }
        }
        if (count > q[index].numOfAns)
        {
            break;
        }
    }
}

void getQuestionSentence(int index)
{
    int read;
    printf("Adja meg az uj kerdest:\n");
    do
    {
        printf("> ");
        read = getLine("prompt", q[index].qs, sizeof(q[index].qs));
    } while (!properLine(read, index, 'q'));

    q[index].qsl = strlen(q[index].qs);
    for (read = q[index].qsl; read < ql; ++read)
    {
        q[index].qs[read] = ' ';
    }
}

void setModifyingTime(int index)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(q[index].lmod, asctime(timeinfo));
    if (q[index].lmod[strlen(q[index].lmod) - 1] == '\n')
    {
        q[index].lmod[strlen(q[index].lmod) - 1] = '\0';
    }
}

void printQuestion(int index)
{
    int i, read;
    for (read = 0; read < q[index].qsl; ++read)
    {
        printf("%c", q[index].qs[read]);
    }
    printf("\n");

    for (i = 0; i < q[index].numOfAns; ++i)
    {
        printf("   %d. ", i + 1);
        for (read = 0; read < q[index].ansl[i]; ++read)
        {
            printf("%c", q[index].ans[i][read]);
        }
        printf("\n");
    }
    printf("\n");
}

void getQuestion(int index)
{
    getQuestionSentence(index);

    q[index].isPermanent = false;

    printf("\nHany valaszt szeretne megadni?\n");
    q[index].numOfAns = -1;
    do
    {
        printf("> ");
        int result = scanf("%d", &q[index].numOfAns);
        if (result == 0)
        {
            while (fgetc(stdin) != '\n')
                ;
        }
        if (result == EOF)
            printf("Something happened wrong.");
    } while (!(q[index].numOfAns > 1 && q[index].numOfAns < 5));

    while (fgetc(stdin) != '\n')
        ; // reading the end line character to not show twice the console log

    int t[4] = {1, 1, 1, 1};
    getAnswerSentences(index, t);

    // saving the time of creating
    setModifyingTime(index);

    printf("\nA hozzaadott kerdes:\n");
    printQuestion(index);
}

void addQuestion()
{
    if (qIndex < 499)
    {
        getQuestion(qIndex);
        ++qIndex; // the number of questions is increased
    }
    else
    {
        printf("Nincs hely tobb kerdes szamara.\n");
    }
}

int getBetween(int he, int le) // le = lower or equal; he = higher or equal
{
    int answer;
    do
    {
        printf("> ");
        int result = scanf("%d", &answer);
        if (result == 0)
        {
            while (fgetc(stdin) != '\n')
                ;
        }
        if (result == EOF)
        {
            printf("Something went wrong.");
        }
        while (fgetc(stdin) != '\n')
            ;
    } while (!(answer >= he && answer <= le));
    // while(fgetc(stdin) != '\n');
    return answer;
}

int selectModifyQuestion()
{
    printf("Eddigi kerdesek:\n\n");
    printOutQuestionsFormatted();
    printf("Adja meg a modositani kivant kerdes szamat:\n");

    int result = getBetween(1, qIndex);

    --result;

    printf("\nA kivalasztott kerdes:\n");
    printQuestion(result);
    return result;
}

bool minGoodMods(int *t, int start, int end)
{
    int i;
    if (start != end)
    {
        for (i = start; i < end; ++i)
        {
            if (t[i] == 0)
            {
                printf("Nem jo (az osszes elemre adja meg!)\n");
                return false;
            }
        }
    }
    return true;
}

void modifyThatQuestion(int index)
{
    printf("Mit szeretne modositani?\n");
    printf("Valaszait 1 (igen) es 0 (nem) jelekkel adja meg!\n");

    printf("\nModositja a kerdest?\n");
    int answer = getBetween(0, 1);
    if (answer == 1)
    {
        getQuestionSentence(index);
    }

    printf("\nModositja a valaszokat?\n");
    answer = getBetween(0, 1);

    if (answer == 1)
    {
        printf("\nModositja a valaszok szamat?\n");
        int oldNumOfA = q[index].numOfAns;
        if (getBetween(0, 1) == 1)
        {
            printf("\nHany uj valasz legyen?\n");
            q[index].numOfAns = getBetween(2, 4);
        }

        int t[4] = {0, 0, 0, 0};
        if (oldNumOfA > q[index].numOfAns) // if the number of answers decreased
        {
            int i;
            for (i = 0; i < oldNumOfA; ++i)
            {
                strcpy(q[index].ans[i], "");
                q[index].ansl[i] = 0;
            }
            for (i = 0; i < maxAnswerNumber; ++i)
            {
                t[i] = 1;
            }
        }
        else
        {
            printf("\nMely valaszokat szeretne modositani?\n");
            printf("Egyesevel (0 es 1-eket irva), szokozzel elvalasztva adja meg mely valaszokat modositja!\n");
            printf("> ");
            int i;
            do
            {
                for (i = 0; i < q[index].numOfAns; ++i)
                    scanf(" %d", &t[i]);
                while (fgetc(stdin) != '\n')
                    ;
            } while (!(minGoodMods(t, oldNumOfA, q[index].numOfAns)));
        }
        getAnswerSentences(index, t);
    }

    printf("\nLegyen vegleges a kerdes?\n");
    q[index].isPermanent = getBetween(0, 1);

    setModifyingTime(index);
}

void modifyQuestion()
{
    if (qIndex > 0)
    {
        int qn; // question number
        qn = selectModifyQuestion();
        modifyThatQuestion(qn);
        printf("\nA megvaltoztatott kerdes:\n");
        printQuestion(qn);
    }
    else
        printf("A kerdoiv ures!\n");
}

int selectDeleteQuestion()
{
    printf("Eddigi kerdesek:\n\n");
    printOutQuestionsFormatted();
    printf("Adja meg a torolni kivant kerdes szamat:\n");

    int result = getBetween(1, qIndex);

    printf("\nA torolt kerdes:\n");
    printQuestion(result - 1);
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
    for (i = 0; i < 4; ++i)
    {
        q[qn].ansl[i] = 0;
        strcpy(q[qn].ans[i], "");
    }
    // a tobbit eggyel visszabb hozom a tombben
    for (i = qn + 1; i < qIndex; ++i)
    {
        q[i - 1].isPermanent = q[i].isPermanent;
        q[i - 1].numOfAns = q[i].numOfAns;
        q[i - 1].qsl = q[i].qsl;
        strcpy(q[i - 1].qs, q[i].qs);
        strcpy(q[i - 1].lmod, q[i].lmod);
        for (j = 0; j < 4; ++j)
        {
            q[i - 1].ansl[j] = q[i].ansl[j];
            strcpy(q[i - 1].ans[j], q[i].ans[j]);
        }
    }

    // csokkentem a kerdesek szamat
    --qIndex;
}

void deleteQuestion()
{
    if (qIndex > 0)
    {
        int qn = selectDeleteQuestion();
        --qn;
        deleteNthQuestion(qn);
    }
    else
    {
        printf("A kerdoiv nem tartalmaz kerdeseket!");
    }
}

void writeDataToFile(const char *fileName, char mode)
{
    FILE *g = fopen(fileName, "w");

    int i, j, k = 0;
    if (mode == 'p')
    {
        for (i = 0; i < qIndex; ++i)
        {
            if (q[i].isPermanent == true)
                ++k;
        }
        fprintf(g, "%d\n-\n", k);
    }
    else // if(mode == 'n')
    {
        fprintf(g, "%d\n-\n", qIndex);
    }

    for (i = 0; i < qIndex; ++i)
    {
        if (mode == 'p')
        {
            if (q[i].isPermanent == true)
            {
                fprintf(g, "%d\n", q[i].numOfAns);
                for (k = 0; k < ql; ++k)
                    fprintf(g, "%c", q[i].qs[k]);
                fprintf(g, "\n");
                for (j = 0; j < q[i].numOfAns; ++j)
                {
                    for (k = 0; k < ql; ++k)
                        fprintf(g, "%c", q[i].ans[j][k]);
                    fprintf(g, "\n");
                }
                fprintf(g, "%s\n", q[i].lmod);
                fprintf(g, "-\n");
            }
        }
        else // if(mode == 'n')
        {
            fprintf(g, "%d\n", q[i].numOfAns);
            for (k = 0; k < ql; ++k)
                fprintf(g, "%c", q[i].qs[k]);
            fprintf(g, "\n");
            for (j = 0; j < q[i].numOfAns; ++j)
            {
                for (k = 0; k < ql; ++k)
                    fprintf(g, "%c", q[i].ans[j][k]);
                fprintf(g, "\n");
            }
            fprintf(g, "%s\n", q[i].lmod);
            fprintf(g, "-\n");
        }
    }
}

void finalizeQuery()
{
    if (qIndex > 0)
    {
        printf("Mely kerdeseket szeretne, hogy tartalmazza a vegleges kerdoiv?\n");
        printf("A kivant kerdeseknel adja meg, hogy szerepeljen/e a kerdes vagy sem!\n");
        printf("   0 (nem) // 1 (igen)\n\n");

        int i; // int reqL = 0;
        for (i = 0; i < qIndex; ++i)
        {
            printf("%2d. ", i + 1);
            printQuestion(i);
            printf("Vegleges? ");
            scanf("%d", &q[i].isPermanent);
            while (fgetc(stdin) != '\n')
                ;
            printf("\n");
        }
        writeDataToFile("questionnaire.txt", 'p'); // p, mint permanent
    }
    else
    {
        printf("A kerdoiv nem tartalmaz kerdeseket!");
    }
}

void execute(int option)
{
    printf("\n");
    switch (option)
    {
    case 1:
        addQuestion();
        break;
    case 2:
        modifyQuestion();
        break;
    case 3:
        deleteQuestion();
        break;
    case 4:
        printOutQuestionsFormatted();
        break;
    case 5:
        finalizeQuery();
        break;
    default:
        printf("Unknown option chosen.");
    }
}

void shutDown()
{
    writeDataToFile("questions.txt", 'n'); // n = not permanent
    // printf("Viszlat!\n");
}

void editQuestionnaire()
{
    char c = 'y';
    while (c == 'y')
    {
        showMenu();
        int option;
        option = getBetween(1, 5);
        execute(option);
        askForRunAgain(&c);
    }
}

void checkIn(int sig)
{
    printf("Received signal %d ...\n", sig);
}

pid_t child;
int questionNumbersChosen[3];

void waitForChild()
{
    printf("A kerdezobiztos megerkezett!\n");

    // Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak
    //kivalasztok harom kerdest, amit kikuldok a kerdezobiztosnak
    printf("MLSZ: Kivalasztom a kerdeseket ...\n");

    if (qIndex >= 3)
    {
        int qi;
        int temp_arr[500];
        for (qi = 0; qi < 500; ++qi)
        {
            temp_arr[qi] = qi;
        }
        for (qi = 0; qi < 3; ++qi)
        {
            // questionNumbersChosen[qi] = qi;
            // i have to generate a random permutation of numbers in array questionNumbersChosen
            questionNumbersChosen[qi] = (rand() % (qIndex - qi)) + qi;

            // swap(temp_arr[qi], temp_arr[ questionNumbersChosen[qi] ]);
            // printf("%d | %d", temp_arr[qi], temp_arr[questionNumbersChosen[qi]]);

            if (temp_arr[questionNumbersChosen[qi]] != temp_arr[qi])
            {
                temp_arr[qi] = temp_arr[qi] + temp_arr[questionNumbersChosen[qi]];
                temp_arr[questionNumbersChosen[qi]] = temp_arr[qi] - temp_arr[questionNumbersChosen[qi]];
                temp_arr[qi] = temp_arr[qi] - temp_arr[questionNumbersChosen[qi]];
            }

            // printf(" --> %d | %d\n", temp_arr[qi], temp_arr[questionNumbersChosen[qi]]);
        }

        for (qi = 0; qi < 3; ++qi)
        {
            questionNumbersChosen[qi] = temp_arr[qi];
            // printf("%d \n", temp_arr[qi]);
        }

        int send_questions = open("oda", O_WRONLY);
        write(send_questions, questionNumbersChosen, sizeof(questionNumbersChosen));
        printf("MLSZ: Elkuldtem a kerdeseket ...\n");
        close(send_questions);
    }
    else // szivas!
    {
        printf("Sajnos nincs eleg kerdes ... :/ \n  De legalabb nem hagytam zombi folyamatot futni!:)\n");
        kill(child, SIGTERM);
        exit(1);
    }
}

void doConsultation(char *location)
{
    srand(time(NULL));
    // szulotol -> gyereknek
    int f = mkfifo("oda", 0744);
    if (f < 0)
    {
        perror("Error a fradi_cso letrehozasanal!");
        exit(1);
    }

    // gyerektol -> szulonek
    int g = mkfifo("vissza", 0744);
    if (g < 0)
    {
        perror("Error a fradi_cso2 letrehozasanal!");
        exit(1);
    }

    // az MLSZ (szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott labdajáték centrumba, városba.

    child = fork();

    if (child < 0)
    {
        printf("Valami hiba tortent a gyerek folyamat letrehozasa soran! // Somethong went wrong at fork() command! - creating the child process");
        exit(1);
    }
    if (child > 0) // now we are the parent process, the MLSZ
    {

        int status;

        //sleep(1);
        printf("MLSZ: Kerdezobiztost kuldok a varosba ...\n");

        // 1. varok egy szignalra a kerdezobiztostol, hogy jelentkezzen be
        signal(SIGUSR1, waitForChild); // feliratkozom a signalra

        // varom a valaszokat a kerdesekre
        // Az MLSZ fogadja az eredményt
        int citizen_count;
        int answers[60];

        int result_reader = open("vissza", O_RDONLY);
        read(result_reader, &citizen_count, sizeof(citizen_count));
        read(result_reader, answers, sizeof(answers));

        // Az MLSZ összegez
        int selected_question_array[3][4] = {0}; // 3 = atadott kerdesek szama; 4 = maximum valaszlehetosegek szama
        int i, j;
        for (i = 0; i < 3 * citizen_count; ++i)
        {
            ++selected_question_array[i % 3][answers[i]];
        }

        // a képernyőre írja a konzultáció eredményét.
        printf("\nA konzultacio eredmenye:\n\n");
        for (i = 0; i < 3; ++i)
        {
            printf("%d. kerdes:\n\n", i + 1);
            printQuestion(questionNumbersChosen[i]);

            for (j = 1; j <= q[questionNumbersChosen[i]].numOfAns; ++j)
            {
                printf("  %d. valasz %d szavazatot kapott\n", j, selected_question_array[i][j - 1]);
            }
            printf("\n");
        }

        waitpid(child, &status, 0);
        close(result_reader);
    }
    else // if(child == 0) // now we are the child process, the Interviewer
    {
        // a kérdezőbiztos bejelentkezik az MLSZ-hez és jelzi, hogy készen áll a feladat végrehajtására
        sleep(1); // csakhogy ne egybol jelentkezzen es nehogy veletlensegbol elobb erkezzen meg, mint ahogy az MLSZ kikuldene
        printf("Kerdezobiztos: Bejelezntkezem %s helyrol! :D\n", location);
        kill(getppid(), SIGUSR1); // 2. kuldok egy szignalt az MLSZ-nek, hogy bejelentkeztem

        // A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni kívánó polgárt, akik kitöltik a kérdőívet (véletlenszerűen)
        int citizen_count = (rand() % 11) + 10;

        // fogadom a kerdesek sorszamait
        // varom a feladatomat
        int child_pipe = open("oda", O_RDONLY);

        int questionIndices[3];
        // printf("Kerdezobiztos: olvasnam a kerdeseket\n");
        read(child_pipe, questionIndices, sizeof(questionIndices));
        printf("Kerdezobiztos: beolvastam a kerdeseket\n");

        // a polgárok kitöltik a kérdőívet
        int completing_results[60]; // 60 = 20 x 3 -> every citizen answers on 3 question
        int i;
        // printf("Kerdezobiztos: ez lefutott\n citizencount %d \n", citizen_count);
        for (i = 0; i < 3 * citizen_count; ++i)
        {
            completing_results[i] = (rand() % q[questionIndices[i % 3]].numOfAns);
        }

        // Az eredményt a kérdezőbiztos visszaküldi csővezetéken az MLSZ-be, majd a kérdezőbiztos befejezi a konzultációt.
        int return_results_pipe = open("vissza", O_WRONLY);
        write(return_results_pipe, &citizen_count, sizeof(citizen_count)); // valasztopolgarok szama
        write(return_results_pipe, completing_results, sizeof(completing_results));

        // befejeztem
        close(child_pipe);
        close(return_results_pipe);
    }
    close(f);close(g);
    unlink("oda");
    unlink("vissza");
}

int main(int argc, char **argv)
{
    powerOn();
    // editQuestionnaire();
    if (argc == 2)
    {
        doConsultation(argv[1]);
    }
    else if (argc <= 1)
    {
        printf("Egy parametert szukseges lenne megadni, Fradipolis-t, azaz a labdajatek centrum vagy a varos nevet!!!\n");
    }
    else
    {
        printf("Rossz a parameterezes! Csupan egyetlen parametert kell megadni, Fradipolis, azaz a labdajatek centrum vagy a varos nevet!!!\n");
        printf("\n  De ha Onnek jo, akkor tovabbmehetunk az elso parameterrel ;) :D");
        printf("\n    (1 (igen) || 0 (nem)");
        int answer = -1;
        answer = getBetween(0, 1);
        if (answer)
            doConsultation(argv[1]);
    }
    shutDown();
    return 0;
}
