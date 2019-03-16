#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>  //fork
#include <wait.h> //waitpid
#include <signal.h> //getpid()
#include <sys/types.h>


//Function Declaration
void mainLoop();
void showWelcome();
FILE* fileHandle();
void parseFile(FILE *pFile);
int mainMenuSelection();
void addQuestion();
void listQuestions();
void editQuestion();
void deleteQuestion();
void exportSurvey();
void saveProgress(FILE* pFile);
int* select3RandomNumbers();
void handler(int signalNumber);


struct QandA{
    char* question;
    char* creationTime;
    char* answer1;
    char* answer2;
    char* answer3;
    char* answer4;
};

struct PersonAnswer{
    char* question[3];
    char* answer[3];
    int ID;
};


//Globals
int Number_of_Questions = 0;
const int MAX = 1000;
bool quit = false;
struct QandA arrayOfQuestions[1000];
pid_t child;
int pipefd[2];
int pipefd2[2];
int counter;




int main() {

    srand(time(NULL));
    showWelcome();
    mainLoop();

    return 0;
}


void mainLoop(){

    FILE* pFile = fileHandle();
    parseFile(pFile);
    int *pointerToRandomNumbers;
    signal(SIGTERM,handler);
    int amountOfPeople = rand()%11+10;


    //Making Pipes

    pipe(pipefd);
    pipe(pipefd2);

    while (quit != true){

        int userInput = mainMenuSelection();

        switch  (userInput) {

            case ('a'):
                addQuestion();
                break;

            case ('l'):
                listQuestions();
                break;

            case ('e'):
                editQuestion();
                break;

            case ('d'):
                deleteQuestion();
                break;

            case ('x'):
                exportSurvey();
                break;

            case ('q'):
                saveProgress(pFile);
                quit = true;
                printf("\n\n\n* * * Have a good day! * * *\n\n\n");
                break;

            case ('t'): //TODO Maybe don't have case and put at the end of switch case



                break;

            default:
               printf("\n\n* * * YOU DID NOT ENTER A CORRECT LETTER * * *");
        }

    }

    //Maybe put error check thing
    child = fork();


    if (child > 0) { //Parent

        close(pipefd[0]); //Close read end for parent

        pause(); //Waits for the arrival of the commissionaire
        printf("Child arrived\n.");


        //Each time this is called random numbers change
        pointerToRandomNumbers = select3RandomNumbers();
        for (int i = 0; i < 3; ++i) {
            printf("Random %d. = %d, ", i,pointerToRandomNumbers[i]);
        }
        printf("\n");

        for (int j = 0; j < 3; ++j) {
            write(pipefd[1], &arrayOfQuestions[pointerToRandomNumbers[j]], sizeof(arrayOfQuestions));
            printf("SHOULD SEE: %s", arrayOfQuestions[pointerToRandomNumbers[j]].question);
        }





        //Getting back findings and printing on screen
        close(pipefd2[1]); //Close write end

        struct PersonAnswer personAnswer[MAX];

        sleep(3);
        for (int k = 0; k < amountOfPeople; ++k) {
            read(pipefd2[0],&personAnswer[k], sizeof(personAnswer[k]));
        }


        for (int k = 0; k < amountOfPeople; ++k) {
            printf("Person ID: %d\n", personAnswer[k].ID);
            for (int i = 0; i < 3; ++i) {
                printf("Question :%s\n", personAnswer[k].question[i]);
                printf("Answer: %s\n", personAnswer[k].answer[i]);
            }
            printf("********\n");
        }


    } else{ //Child

        sleep(2); //The commissionaire takes some time to get there
        kill(getppid(), SIGTERM); //I have arrived at the place

        close(pipefd[1]); //Close write end

        struct QandA threeRandomQ[MAX];

        sleep(1);
        for (int i = 0; i < 3; ++i) {
            read(pipefd[0], &threeRandomQ[i], sizeof(threeRandomQ));
            printf("HERE: %s", threeRandomQ[i].question);
        }

        struct PersonAnswer personAnswer[MAX];

        // 10-20 People need to fill
        int randomDescision = rand()%4;

        for (int j = 0; j < amountOfPeople; ++j) {
            personAnswer[j].ID = j;
            for (int i = 0; i < 3; ++i) {//For 3 Questions + Answer
                personAnswer[j].question[i] = threeRandomQ[i].question;
                if (randomDescision == 0){personAnswer[j].answer[i] = threeRandomQ[i].answer1;}
                if (randomDescision == 1){personAnswer[j].answer[i] = threeRandomQ[i].answer2;}
                if (randomDescision == 2){personAnswer[j].answer[i] = threeRandomQ[i].answer3;}
                if (randomDescision == 3){personAnswer[j].answer[i] = threeRandomQ[i].answer4;}
                randomDescision = rand()%4;
            }
        }

//        Now we must send back our findings

        close(pipefd2[0]); //Close the read end

        for (int k = 0; k < amountOfPeople; ++k) {
            write(pipefd2[1], &personAnswer[k], sizeof(personAnswer[k]));
        }



    };




}

void showWelcome(){
    printf("* * * * * * * * * * * * * * * * * *\n"
                   "*  Welcome to Survey Creator 2.4  *"
                   "\n* * * * * * * * * * * * * * * * * *\n\n"
    );
}

FILE* fileHandle(){

    FILE *pFile;

    //Tries to open old file, if doesn't exist makes one
    pFile = fopen("survey.txt", "r+");

    if(pFile == NULL) {
        pFile = fopen("survey.txt", "w+");
        fputs("0", pFile);
    }

    //If file creation has failed print error
    if (pFile == NULL){
        perror("ERROR OCCURRED");
        printf("Error Code: %d\n", errno);
        exit(1);
    }

    return pFile;
}

void parseFile(FILE *pFile){

    fscanf(pFile, "%d", &Number_of_Questions);

    //Skips a line
    rewind(pFile);
    fscanf(pFile, "%*[^\n]\n", NULL);


    ssize_t read2;
    size_t length = 0;

    for (int i = 0; i < Number_of_Questions; ++i) {
        read2 = getline(&arrayOfQuestions[i].creationTime, &length, pFile);
        read2 = getline(&arrayOfQuestions[i].question, &length, pFile);
        read2 = getline(&arrayOfQuestions[i].answer1, &length, pFile);
        read2 = getline(&arrayOfQuestions[i].answer2, &length, pFile);
        read2 = getline(&arrayOfQuestions[i].answer3, &length, pFile);
        read2 = getline(&arrayOfQuestions[i].answer4, &length, pFile);
    }



}

int mainMenuSelection(){
    fseek(stdin,0,SEEK_END);
    printf("\n\nWhat would you like to do? : "
                   "\n  Press 'a' to add a questions"
                   "\n  Press 'l' to List the questions and answers"
                   "\n  Press 'e' to Edit"
                   "\n  Press 'd' to Delete a question"
                   "\n  Press 'x' to EXport survey"
                   "\n  Press 'q' to quit"
                   "\n\n\n  Input Answer Here: "
                   "");
    int c = getchar();
    return c;
}

void addQuestion(){

    printf("You have chosen to \"Add a Question\""
                   "\nPlease enter the Question now: ");

    Number_of_Questions++;

    fseek(stdin,0,SEEK_END);
    char ch[MAX];
    fgets(ch, sizeof ch, stdin);


    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    arrayOfQuestions[Number_of_Questions-1].creationTime = malloc(1000);
    strncpy(arrayOfQuestions[Number_of_Questions-1].creationTime, s, 1000);

    strcat(arrayOfQuestions[Number_of_Questions-1].creationTime, "\n");


    arrayOfQuestions[Number_of_Questions-1].question = malloc(1000);
    strncpy(arrayOfQuestions[Number_of_Questions-1].question, ch, 1000);

    printf("\nPlease Enter the First Answer To the Question: ");
    fseek(stdin,0,SEEK_END);
    fgets(ch, sizeof ch, stdin);
    arrayOfQuestions[Number_of_Questions-1].answer1 = malloc(1000);
    strncpy(arrayOfQuestions[Number_of_Questions-1].answer1, ch, 1000);

    printf("\nPlease Enter the Second Answer To the Question: ");
    fseek(stdin,0,SEEK_END);
    fgets(ch, sizeof ch, stdin);
    arrayOfQuestions[Number_of_Questions-1].answer2 = malloc(1000);
    strncpy(arrayOfQuestions[Number_of_Questions-1].answer2, ch, 1000);



    printf("\nGreat! Would you like to add a third Answer?"
                   "\nPress 'y' or 'n' here: ");

    int wantToAddAnswer = getchar();

    if (wantToAddAnswer == 'y') {
        printf("\nEnter the third Answer here: ");
        fseek(stdin,0,SEEK_END);
        fgets(ch, sizeof ch, stdin);
        arrayOfQuestions[Number_of_Questions-1].answer3 = malloc(1000);
        strncpy(arrayOfQuestions[Number_of_Questions-1].answer3, ch, 1000);


        printf("\nGreat! Would you like to add a fourth Answer?"
                       "\nPress 'y' or 'n' here: ");
        fseek(stdin,0,SEEK_END);
        wantToAddAnswer = getchar();
        if (wantToAddAnswer == 'y') {
            printf("\nEnter the fourth Answer here: ");
            fseek(stdin,0,SEEK_END);
            fgets(ch, sizeof ch, stdin);
            arrayOfQuestions[Number_of_Questions-1].answer4 = malloc(1000);
            strncpy(arrayOfQuestions[Number_of_Questions-1].answer4, ch, 1000);
        } else{
            arrayOfQuestions[Number_of_Questions-1].answer4 = malloc(1000);
            arrayOfQuestions[Number_of_Questions-1].answer4 = '\0';
        }
    } else{
        arrayOfQuestions[Number_of_Questions-1].answer3 = malloc(1000);
        arrayOfQuestions[Number_of_Questions-1].answer3 = '\0';
        arrayOfQuestions[Number_of_Questions-1].answer4 = malloc(1000);
        arrayOfQuestions[Number_of_Questions-1].answer4 = '\0';
    }

    printf("\n\n* * * Great! Adding Complete * * * \n\n");

}

void listQuestions(){
    if(Number_of_Questions != 0) {
        printf("\n* * * * * * * * * * *\n");

        for (int i = 0; i < Number_of_Questions; ++i) {
            printf("\n%d.\n", i + 1);
            printf("Question Creation Time: %s \n", arrayOfQuestions[i].creationTime);
            printf("Q. %s\n", arrayOfQuestions[i].question);
            printf("   a. %s\n", arrayOfQuestions[i].answer1);
            printf("   b. %s\n", arrayOfQuestions[i].answer2);
            printf("   c. %s\n", arrayOfQuestions[i].answer3);
            printf("   d. %s\n", arrayOfQuestions[i].answer4);
            printf("* * * * * * * * * * *");
        }
        printf("\n\n\n* * * Listing Complete * * * \n");
    } else printf("\n\n\n* * * There are no questions in the question bank yet * * *\n\n\n");
}

void editQuestion(){
    if (Number_of_Questions != 0) {
        listQuestions();
        printf("\nWhich Question would you like to edit?\n"
                       "Press the corresponding number shown"
                       "in the above list: ");

        fseek(stdin, 0, SEEK_END);
        int numberToEdit;
        scanf("%d", &numberToEdit);
        numberToEdit--;


        printf("NUMBER TO EDIT: %d", numberToEdit);


        printf("\nGreat! \nTo Edit the Question press q"
                       "\nTo Edit Answer a press a"
                       "\nTo Edit Answer a press b"
                       "\nTo Edit Answer a press c"
                       "\nTo Edit Answer a press d: ");


        fseek(stdin, 0, SEEK_END);
        int editParam = getchar();

        printf("\nGreat! Please enter new item here: ");

        fseek(stdin, 0, SEEK_END);
        char ch[1000];
        fgets(ch, sizeof ch, stdin);


        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%c", tm);
        free(arrayOfQuestions[numberToEdit].creationTime);
        arrayOfQuestions[numberToEdit].creationTime = malloc(1000);
        strncpy(arrayOfQuestions[numberToEdit].creationTime, s, 1000);

        switch (editParam) {

            case ('q'):
                free(arrayOfQuestions[numberToEdit].question);
                arrayOfQuestions[numberToEdit].question = malloc(1000);
                strncpy(arrayOfQuestions[numberToEdit].question, ch, 1000);
                break;

            case ('a'):
                free(arrayOfQuestions[numberToEdit].answer1);
                arrayOfQuestions[numberToEdit].answer1 = malloc(1000);
                strncpy(arrayOfQuestions[numberToEdit].answer1, ch, 1000);
                break;

            case ('b'):
                free(arrayOfQuestions[numberToEdit].answer2);
                arrayOfQuestions[numberToEdit].answer2 = malloc(1000);
                strncpy(arrayOfQuestions[numberToEdit].answer2, ch, 1000);
                break;

            case ('c'):
                free(arrayOfQuestions[numberToEdit].answer3);
                arrayOfQuestions[numberToEdit].answer3 = malloc(1000);
                strncpy(arrayOfQuestions[numberToEdit].answer3, ch, 1000);
                break;

            case ('d'):
                free(arrayOfQuestions[numberToEdit].answer4);
                arrayOfQuestions[numberToEdit].answer4 = malloc(1000);
                strncpy(arrayOfQuestions[numberToEdit].answer4, ch, 1000);
                break;

            default:
                printf("\n* * * YOU DID NOT ENTER A CORRECT LETTER * * * \n");
                break;

        }
    } else printf("\n\n* * * THERE ARE NO QUESTIONS TO EDIT * *  *"
                   "\n* * * TRY ADDING ONE FIRST * * *\n");

}

void deleteQuestion(){

    listQuestions();

    printf("\nYou have selected \"Delete Question\""
                   "\nWhich Question would you like to delete?"
                   "\nPress the corresponding number shown in the list above: ");

    fseek(stdin,0,SEEK_END);
    int numberToDelete;
    scanf("%d", &numberToDelete);
    numberToDelete--;

    if (numberToDelete != Number_of_Questions-1) {

        free(arrayOfQuestions[numberToDelete].creationTime);
        arrayOfQuestions[numberToDelete].creationTime = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].creationTime, arrayOfQuestions[Number_of_Questions - 1].creationTime,
                1000);

        free(arrayOfQuestions[numberToDelete].question);
        arrayOfQuestions[numberToDelete].question = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].question, arrayOfQuestions[Number_of_Questions - 1].question, 1000);

        free(arrayOfQuestions[numberToDelete].answer1);
        arrayOfQuestions[numberToDelete].answer1 = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].answer1, arrayOfQuestions[Number_of_Questions - 1].answer1, 1000);

        free(arrayOfQuestions[numberToDelete].answer2);
        arrayOfQuestions[numberToDelete].answer2 = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].answer2, arrayOfQuestions[Number_of_Questions - 1].answer2, 1000);

        free(arrayOfQuestions[numberToDelete].answer3);
        arrayOfQuestions[numberToDelete].answer3 = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].answer3, arrayOfQuestions[Number_of_Questions - 1].answer3, 1000);

        free(arrayOfQuestions[numberToDelete].answer4);
        arrayOfQuestions[numberToDelete].answer4 = malloc(1000);
        strncpy(arrayOfQuestions[numberToDelete].answer4, arrayOfQuestions[Number_of_Questions - 1].answer4, 1000);
    }


    free(arrayOfQuestions[Number_of_Questions-1].creationTime);
    free(arrayOfQuestions[Number_of_Questions-1].question);
    free(arrayOfQuestions[Number_of_Questions-1].answer1);
    free(arrayOfQuestions[Number_of_Questions-1].answer2);
    free(arrayOfQuestions[Number_of_Questions-1].answer3);
    free(arrayOfQuestions[Number_of_Questions-1].answer4);

    Number_of_Questions--;

    printf("\n\n* * * * * * DELETION COMPLETE * * * * * *");

}

void exportSurvey(){

    FILE *pFile;

    pFile = fopen("export.txt", "w+");
    fputs(          "           *  *  *  *  *  *  *  *"
                  "\n           *                    * \n"
                  "\n           *      Survey        * \n"
                  "\n           *                    *\n"
                    "           *  *  *  *  *  *  *  *\n\n\n\n", pFile);

    if (pFile == NULL){
        perror("ERROR OCCURRED");
        printf("Error Code: %d\n", errno);
        exit(1);
    }

    int counter = 1;

    for (int i = 0; i < Number_of_Questions; ++i) {

        printf("\nWould you like to include the question below in the final Survey?:\n\n");

        printf("Question Creation Time: %s \n", arrayOfQuestions[i].creationTime);
        printf("Q. %s\n", arrayOfQuestions[i].question);
        printf("   a. %s\n",arrayOfQuestions[i].answer1);
        printf("   b. %s\n",arrayOfQuestions[i].answer2);
        printf("   c. %s\n",arrayOfQuestions[i].answer3);
        printf("   d. %s\n",arrayOfQuestions[i].answer4);

        printf("Press 'y' or 'n': ");

        fseek(stdin,0,SEEK_END);
        int yesOrNo = getchar();

        if (yesOrNo == 'y') {
            fprintf(pFile, "\n\n* * * * * * * * * * * * * * * * * * *\n\n");
            fprintf(pFile, "Creation Time: %s\n", arrayOfQuestions[i].creationTime);
            fprintf(pFile, "Question %d. : %s\n\n", counter, arrayOfQuestions[i].question);
            fprintf(pFile, "a.%s\n", arrayOfQuestions[i].answer1);
            fprintf(pFile, "b.%s\n", arrayOfQuestions[i].answer2);
            if(arrayOfQuestions[i].answer3 == NULL){
                fprintf(pFile, "c.%s\n", "");
            } else {
                fprintf(pFile, "c.%s\n", arrayOfQuestions[i].answer3);
            }
            if(arrayOfQuestions[i].answer3 == NULL){
                fprintf(pFile, "d.%s\n", "");
            } else {
                fprintf(pFile, "d.%s\n", arrayOfQuestions[i].answer4);
            }
            counter++;
        }

    }

    printf("\n* * * * * * EXPORTED TO FILE * * * * * *");

}

void saveProgress(FILE* pFile){

    rewind(pFile);
    fprintf(pFile, "%d\n", Number_of_Questions);

    for (int i = 0; i < Number_of_Questions; ++i) {
        fprintf(pFile, "%s", arrayOfQuestions[i].creationTime);
        fprintf(pFile, "%s", arrayOfQuestions[i].question);
        fprintf(pFile, "%s", arrayOfQuestions[i].answer1);
        fprintf(pFile, "%s", arrayOfQuestions[i].answer2);

        if  (arrayOfQuestions[i].answer3 == NULL || arrayOfQuestions[i].answer3 == "\0"){
            fprintf(pFile, "%s", "\n");
        } else{
            fprintf(pFile, "%s", arrayOfQuestions[i].answer3);
        }

        if  (arrayOfQuestions[i].answer4 == NULL || arrayOfQuestions[i].answer4 == "\0"){
            fprintf(pFile, "%s", "\n");
        } else{
            fprintf(pFile, "%s", arrayOfQuestions[i].answer4);
        }
    }
    fclose(pFile);
}

int* select3RandomNumbers(){

    int randomA;
    int randomB;
    int randomC;

    randomA = rand() % Number_of_Questions;
    randomB = randomA;
    while (randomB == randomA){
        randomB = rand() % Number_of_Questions;
    }
    randomC = randomA;
    while (randomC == randomA || randomC == randomB) {
        randomC = rand() % Number_of_Questions;
    }

    int *randomNumbers = malloc(sizeof(int)*3);
    randomNumbers[0] = randomA;
    randomNumbers[1] = randomB;
    randomNumbers[2] = randomC;

    return randomNumbers;
}

void handler(int signalNumber){
    counter++;
}