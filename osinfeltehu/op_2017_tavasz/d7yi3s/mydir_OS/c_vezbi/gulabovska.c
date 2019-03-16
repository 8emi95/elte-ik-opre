#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define length 256

typedef struct qb{
    //TITLE
    char question[length];
    //QNUMBER
    char qNumber[10];
    //DATE
    char qctime[100];
    //ANSWERS
    char answer1[length];
    char answer2[length];
    char answer3[length];
    char answer4[length];
} QB;

const char fname[20] = "qustionBank.txt";
void createIds();



int askForAction(){
    int actionInt;
    printf("*********************************\n"
                   "SURVEY\n"
                   "*********************************\n"
                   "Hello and welcome to the survey!\n\n"
                   "What would you like to do?\n\n"
                   "TYPE 1 for adding a question\n\n"
                   "TYPE 2 for modifying a qustion\n\n"
                   "TYPE 3 for deleting question\n\n"
                   "TYPE 4 to export the survey\n\n"
                   "TYPE 5 to close the survey\n\n"
                   "\n");


    scanf("%i", &actionInt);

    return actionInt;
}

void addQuestionToFile(QB* qb){

    FILE* fp = fopen(fname, "a");
    if(fp != NULL){
        fwrite(qb,sizeof(QB),1,fp);
        printf("Your question is added to the survey.\n"); 
    }
    fclose(fp);
    //createIds();
}

void listQuestions(){
    FILE* fp = fopen(fname, "r");

    QB* qb = malloc(sizeof(QB));

    if(fp != NULL){
        while (fread(qb,sizeof(QB),1, fp)) {
            printf("question number: %s question title: %s question date: %s answer1: %s answer2: %s answer3: %s answer4: %s",
                   qb->qNumber,qb->question, qb->qctime,qb->answer1,qb->answer2,qb->answer3,qb->answer4);
        }
        fclose(fp);
    }

}

void clean_stdin()
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

QB makeQuestion() {
    char buffer[length];

    QB* qb = malloc(sizeof(QB));

    printf("Enter the question:\n");
    clean_stdin();
    fgets(buffer,length,stdin);
    strcpy(qb->question,buffer);


    strcpy(qb->qNumber,"0\n");

    char dateStr[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dateStr, sizeof(dateStr)-1, "%d %m %Y %H:%M\n", t);

    strcpy(qb->qctime,dateStr);


    printf("How many answers would like to add? (1-4)\n");
    //clean_stdin();
    fgets(buffer,length,stdin);
    int n = atoi(buffer);

    switch(n)
        {
            case 1:
                printf("Write your answer1:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer1,buffer);
                strcpy(qb->answer2,"\n");
                strcpy(qb->answer3,"\n");
                strcpy(qb->answer4,"\n");

                break;
            case 2:
                printf("Write your answer1:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer1,buffer);
                printf("Write your answer2:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer2,buffer);
                strcpy(qb->answer3,"\n");
                strcpy(qb->answer4,"\n");
                break;
            case 3:
                printf("Write your answer1:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer1,buffer);
                printf("Write your answer2:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer2,buffer);
                printf("Write your answer3:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer3,buffer);
                strcpy(qb->answer4,"\n");
                break;
            case 4:
            printf("Write your answer1:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer1,buffer);
                printf("Write your answer2:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer2,buffer);
                printf("Write your answer3:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer3,buffer);
                printf("Write your answer4:\n");
                fgets(buffer,length,stdin);
                strcpy(qb->answer4,buffer);
                break;
            default:
                printf("error wrong input, choose number between 1-4 next time\n");
        }



    //REMEMBER TO free(qb);
    return *qb;

}

void exportSurvey(char* filename){

    char buffer[length];
    char fileN[length];
    strcpy(fileN,filename);
    strcat(fileN,".txt");
    FILE* newSurvey = fopen(fileN, "w");

    QB* qb = malloc(sizeof(QB));


    int n;
    printf("Enter the number of the question you want to export:\n");

    while(fgets(buffer,length,stdin)) {
        FILE* readFromBank = fopen(fname, "r");

        n = atoi(buffer);

        if(readFromBank != NULL){
            while (fread(qb, sizeof(QB), 1, readFromBank)) {
                if(atoi(qb->qNumber) == n) {
                    fwrite(qb,sizeof(QB),1, newSurvey);
                }
            }

        }
        fclose(readFromBank);
    }

    free(qb);

    fclose(newSurvey);
}

void modifyQuestion(QB* toAdd ,int n){

    FILE* readfile = fopen(fname, "r");
    FILE* tmpfile = fopen("tmp.txt","w");

    QB* qb = malloc(sizeof(QB));

    if(readfile != NULL){
        while(fread(qb,sizeof(QB),1,readfile)) {
            if(atoi(qb->qNumber) != n) {
                fwrite(qb,sizeof(QB),1,tmpfile);
            }
            else {
                strcpy(toAdd->qNumber,qb->qNumber);
                fwrite(toAdd,sizeof(QB),1,tmpfile);
            }
        }
    }
    remove(fname);
    rename("tmp.txt",fname);

    free(qb);

    fclose(readfile);
    fclose(tmpfile);

}

void createIds(){

    FILE* readfile = fopen(fname, "r");
    FILE* tmpfile = fopen("tmp.txt","w");

    int n = 0;
    char buffer[length];

    QB* qb = malloc(sizeof(QB));

    if(readfile != NULL){
        while(fread(qb,sizeof(QB),1,readfile)) {
            sprintf(buffer,"%d\n",++n);
            strcpy(qb->qNumber,buffer);
            fwrite(qb,sizeof(QB),1,tmpfile);
        }
    }
    remove(fname);
    rename("tmp.txt",fname);

    free(qb);

    fclose(readfile);
    fclose(tmpfile);

}




void deleteQuestion(int n){


    FILE* readfile = fopen(fname, "r");
    FILE* tmpfile = fopen("tmp.txt","w");

    QB* qb = malloc(sizeof(QB));



    if(readfile != NULL){
        while(fread(qb,sizeof(QB),1,readfile)) {
            if(atoi(qb->qNumber) != n) {
                fwrite(qb,sizeof(QB),1,tmpfile);
            }
        }
    }

    remove(fname);
    rename("tmp.txt",fname);


    free(qb);

    fclose(readfile);
    fclose(tmpfile);

    createIds();
}


int main(){

    QB qb;
    int actionInt;
    int changeQNum;
    char exportFile[length];

    do{
        actionInt=askForAction();

        switch(actionInt){
            case 1:
            printf("You chosed to ADD a QUESTION:\n");
            qb = makeQuestion();
            addQuestionToFile(&qb);
            break;

            case 2:
            listQuestions();
            printf("What question you would like to modify? (type number)\n");
            scanf("%i", &changeQNum);
            printf("Write your modification\n");
            qb = makeQuestion();
            modifyQuestion(&qb,changeQNum);
            printf("Question modified successfully\n");
            break;
            case 3:
            listQuestions();
            printf("What question you would like to delete? (type number)\n");
            scanf("%i", &changeQNum);
            deleteQuestion(changeQNum);
            printf("Question deleted successfully\n");
            break;
            case 4:
            //exoport the survey
            printf("Enter a file name for the exported survey:\n");
            scanf("%s", &exportFile);

            exportSurvey(exportFile);
            printf("exported in file %s.txt\n", exportFile);
            break;
            case 5:
            printf("Survey Closed\n");
            break;
        }
    }while(actionInt != 5);


    

    //qb = makeQuestion();

    //addQuestionToFile(&qb);
    //deleteQuestion(1);

    //exportSurvey("kikis");


    //modifyQuestion(&qb,1);
    //free(&qb);

    return 0;
}