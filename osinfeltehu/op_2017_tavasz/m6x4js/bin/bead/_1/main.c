//Created by Borbényi Áron András
//              M6X4JS
//EMAIL: ron.borbenyi@gmail.com

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


//Global variables
int line_num = 1; //number of lines in the working file.

struct PollNode{
    char question[255];
    char answer[4][255];
    char date[255];
    int ID;
};

void menu();
void menuCLUI(); //Write out the Command Line User Interface
void insertQuestion(char* filename);
void deleteQuestion();
void modifyQuestion();
int listPoll();
void commitToMaster();
char* timeStamp();
void deleteFile();


int main(int argc, const char * argv[]) {
    menu();
    return 0;
}

void menuCLUI(){
  printf("---MENU - M6X4JS---\n");
  printf("1. Insert poll node\n");
  printf("2. Delete poll node\n");
  printf("3. Modify poll node\n");
  printf("4. List the poll\n");
  printf("5. Commit poll\n");
  printf("6. Clock\n");
  printf("7. Delete the data file\n");
  printf("0. EXIT\n");
}

void menu(){
    int command = -1;
    do{
      menuCLUI();
      printf("Enter the number of the command: ");
      scanf("%d", &command);
    }while(command < 0 || command > 7);

    switch(command){
      case 0:
        printf("Exit from the program...\n");
        exit(0); //0 - means succesfully determinated -- other some error code;
        break;
      case 1:{
        insertQuestion("datafile.txt");
        printf("The new poll item is succesfully inserted!\n");
        menu();
        break;
      }
      case 2:{
        deleteQuestion();
        printf("The question is succesfully deleted!\n");
        menu();
        break;
      }
      case 3:{
        modifyQuestion();
        printf("The question is succesfully modified!\n");
        menu();
        break;
      }
      case 4:
        listPoll();
        menu();
        break;
      case 5:
        commitToMaster();
        menu();
        break;
      case 6:
        timeStamp();
        menu();
      case 7:
        deleteFile();
        menu();
        break;
      default:
        menu();
        break;
    }
}

char* timeStamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    char* currentTime = asctime( localtime(&ltime) );
    return currentTime;
}

void insertQuestion(char* filename) {

    struct PollNode newPollNode;

    printf("The question is: ");
    scanf("%s", newPollNode.question);
    printf("\n");

    printf("The ID of the question bellow: ");
    scanf("%d", &newPollNode.ID);
    printf("\n");

    int numberOfAnswers;
    printf("How many answear do you want? [2-4]\n");
    scanf("%d", &numberOfAnswers);
    printf("\n");

    while(numberOfAnswers < 2 || numberOfAnswers > 4){
      printf("How many answear do you want? [2-4]\n");
      scanf("%d", &numberOfAnswers);
    };

    FILE *datafile;
    datafile = fopen(filename, "a");

    char *timer = timeStamp();
    fprintf(datafile, "%d\n%s%s\n%d\n", newPollNode.ID, timer, newPollNode.question, numberOfAnswers );

    for (int i = 0; i < numberOfAnswers; i++) {
      printf("%d. answear: ", i+1 );
      scanf("%s", newPollNode.answer[numberOfAnswers]);
      fprintf(datafile, "%s\n", newPollNode.answer[numberOfAnswers]);
      //Write the answers at the end, because this way another /for(){}/ is not needed
    }

    fprintf(datafile, "\n");
    fclose(datafile);
};

void deleteFile(){

   int ret;

   FILE *fp;
   char filename[] = "datafile.txt";

   fp = fopen(filename, "w");
   fclose(fp);

   ret = remove(filename);

   if(ret == 0){
      printf("File deleted successfully");
      printf("\n");
   }
   else{
      printf("Error: unable to delete the file");
      printf("\n");
   }
};

void modifyQuestion() {
  line_num = 1;
  int find_result = 0;
  int c;
  char temp[255];
  char* str;

  printf("Insert the ID: \n");
  str = (char*)malloc(200);
  scanf("%s", str);

  FILE *oldData;
  oldData = fopen("./datafile.txt", "r");

  FILE *newData;
  newData = fopen("./datatemp.txt", "w");

   while(fgets(temp, 255, oldData) != NULL) {
      if((strstr(temp, str)) != NULL) {
        printf("A match found on line: %d\n", line_num);
        printf("\n%s\n", temp);
        find_result++;
        while(fgets(temp, 255, oldData) != NULL && (temp[0] != '\n')){
          printf("%s",temp);
        }
        printf("Now, modify the poll item!\n");
        struct PollNode newPollNode;

        printf("The question is: ");
        scanf("%s", newPollNode.question);
        printf("\n");

        printf("The ID of the question bellow: ");
        scanf("%d", &newPollNode.ID);
        printf("\n");

        int numberOfAnswers;
        printf("How many answear do you want? [2-4]\n");
        scanf("%d", &numberOfAnswers);
        printf("\n");

        while(numberOfAnswers < 2 || numberOfAnswers > 4){
          printf("How many answear do you want? [2-4]\n");
          scanf("%d", &numberOfAnswers);
        };

        char *timer = timeStamp();
        fprintf(newData, "%d\n%s%s\n%d\n", newPollNode.ID, timer, newPollNode.question, numberOfAnswers );

        for (int i = 0; i < numberOfAnswers; i++) {
          printf("%d. answear: ", i+1 );
          scanf("%s", newPollNode.answer[numberOfAnswers]);
          fprintf(newData, "%s\n", newPollNode.answer[numberOfAnswers]);
          //Write the answers at the end, because this way another /for(){}/ is not needed
        }

        fprintf(newData, "\n");
      }
      line_num++;
      fputs(temp, newData);
    }

    if(find_result == 0) {
      printf("\nSorry, couldn't find a match.\n");
    }

    remove("./datafile.txt");
    rename("./datatemp.txt", "./datafile.txt");

    fclose(oldData);
    fclose(newData);
};

void deleteQuestion(){
  line_num = 1;
  int find_result = 0;
  int c;
  char temp[255];
  char* str;

  printf("Insert the ID: \n");
  str = (char*)malloc(200);
  scanf("%s", str);

  FILE *oldData;
  oldData = fopen("./datafile.txt", "r");

  FILE *newData;
  newData = fopen("./datatemp.txt", "w");

  while(fgets(temp, 255, oldData) != NULL) {
      if((strstr(temp, str)) != NULL) {
        printf("A match found on line: %d\n", line_num);
        printf("\n%s\n", temp);
        while(fgets(temp, 255, oldData) != NULL && (temp[0] != '\n')){
          printf("%s",temp);
        }
        find_result++;
      }
      line_num++;
      fputs(temp, newData);
  }

    if(find_result == 0) {
      printf("\nSorry, couldn't find a match.\n");
    }

    remove("./datafile.txt");
    rename("./datatemp.txt", "./datafile.txt");

    fclose(oldData);
    fclose(newData);
};

int listPoll() {
  int c;

  FILE *file;
  file = fopen("datafile.txt", "r");

  if(file == NULL){
    printf("The file does not exists.");
    return (-1);
  }

  if (file) {
      while ((c = getc(file)) != EOF)
          putchar(c);
      fclose(file);
  }

  return 0;
};

void commitToMaster() {
  char ch;

  FILE *source;
  source = fopen("datafile.txt", "r");

  FILE *target;
  target = fopen("master.txt", "w"); // - definitive version

  while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, target);

   printf("File saved successfully.\n");

   fclose(source);
   fclose(target);
};
