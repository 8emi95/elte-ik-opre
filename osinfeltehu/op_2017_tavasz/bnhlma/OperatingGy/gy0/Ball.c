#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <time.h>


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



struct QandA{
    char* question;
    char* creationTime;
    char* answer1;
    char* answer2;
    char* answer3;
    char* answer4;
};


//Globals
int Number_of_Questions = 0;
const int MAX = 1000;
bool quit = false;
struct QandA arrayOfQuestions[MAX];



int main() {

    showWelcome();
    mainLoop();

    return 0;
}




void mainLoop(){

    FILE* pFile = fileHandle();
    parseFile(pFile);

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
                printf("* * * Have a good day! * * *\n\n\n");
                break;

            default:
               printf("\n\n* * * YOU DID NOT ENTER A CORRECT LETTER * * *");
        }



    }


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
    printf("Number of Questions in Bank : %d \n", Number_of_Questions);

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
                   "\n  Input Answer Here: "
                   "");
    int c = getchar();
    return c;
}

void addQuestion(){

    printf("You have chosen to \"Add a Question\""
                   "\nPlease enter the Question now: ");

    Number_of_Questions++;

    fseek(stdin,0,SEEK_END);
    char ch[1000];
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

    printf("\n* * * * * * * * * * *\n");

    for (int i = 0; i < Number_of_Questions; ++i) {
        printf("\n%d.\n", i+1);
        printf("Question Creation Time: %s \n", arrayOfQuestions[i].creationTime);
        printf("Q. %s\n", arrayOfQuestions[i].question);
        printf("   a. %s\n",arrayOfQuestions[i].answer1);
        printf("   b. %s\n",arrayOfQuestions[i].answer2);
        printf("   c. %s\n",arrayOfQuestions[i].answer3);
        printf("   d. %s\n",arrayOfQuestions[i].answer4);
        printf("* * * * * * * * * * *");
    }


    printf("\n\n\n* * * Listing Complete * * * \n");
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


