#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <time.h>

int noOfErrors;
bool surveyOver=false;
int QuestionsCreated = 0;
const int MAX = 50;

struct QuestionwAnswers{
    char* Question;
    char* DOC;                                          //DATE OF CREATION
    char* firstAnswer;
    char* secondAnswer;
};
int main() {

    printf("This is the survey of The Society of Hungarian Ball Games");
    startProgram();

    return 0;
}

void startProgram(){

    FILE* fp = FileOperations();
    parseFile(fp);

    while (!surveyOver){
            int  choice;
        printf("\n Please choose from the following: "
                   "\n  Hit '1' to Add to Question List"
                   "\n  Hit '2' to List The Data"
                   "\n  Hit '3' to Modify The Data"
                   "\n  Hit '4' to Delete A Question"
                   "\n  Hit '5' to Export The Survey"
                   "\n  Hit '6' to Exit"
                   "\n  Please type the answer to the question: "
                   "")

        scanf(" %d", &choice);
		printf("The number you chose was %d\n", choice);

        switch  (choice) {
            default:
               printf("Please try again and choose one of the correct letters");

            case ('1'):
                addToData();
                break;

            case ('2'):
                listData();
                break;

            case ('3'):
                modifyData();
                break;

            case ('4'):
                deleteData();
                break;

            case ('5'):
                exportSurvey();
                break;

            case ('6'):
                saveAndQuit(fp);
                surveyOver = true;
                printf("Exiting Program");
                break;

        }


    }

}

FILE* FileOperations(){



    FILE *fp = fopen("Questionnaire.txt", "r+");

    if(fp == NULL) {
        fp = fopen("Questionnaire.txt", "w+");
        fputs("This is the Hungarian Questionnaire", fp);
    }

    if (fp == NULL){
        noOfErrors = errno;
      fprintf(stderr, "Number Of errors is: %d\n", errno);
      perror("File not created due to errors");
      fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    EXIT_FAILURE
    }

    return fp;
}
void addToData(){
QuestionsCreated++;
printf("Please Type Out The Question You Want To Create")






}
