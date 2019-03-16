#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void welcome(void) {
  printf("Welcome to the Society of Hungarian Ball Games consultation!\n");
}

void menu(void) {
  printf("Please enter 1, 2, 3, 4 corresponding to the number of your answer\n");
  printf("1: List questions\n");
  printf("2: Add new question and answer\n");
  printf("3: Modify/delete question and answer\n");
  printf("4: Export survey\n");
  char input[50];
  while(1) {
    scanf("%s",input);
    int input_int = atoi(input);
    switch(input_int) {
      case 1 :
        break;
      case 2 :
        break;
      case 3 :
        break;
      case 4 :
        break;
      default:
        printf("Invalid input\n");
    }
  }
}

void answers(char *str1,char *str2, char *str3, char *str4) {
  printf("Please enter 1, 2, 3, 4 corresponding to the number of your answer\n");
  printf("1: %s\n",str1);
  printf("2: %s\n",str2);
  printf("3: %s\n",str3); 
  printf("4: %s\n",str4);  
} 

int finished(void) {
  printf("Thanks for your answer! Would you like to answer another question?\n");
  char answer[4];
  char command[50];
  strcpy(command, "clear");
  char yes[] = "yes";
  char no[] = "no";
  scanf("%s",answer);
  while(1) {
    if(strcmp(answer,yes) == 0) {
      system(command);
      break;
    }
    else if(strcmp(answer,no) == 0) {
      printf("Thanks for your time!\n");
      exit(0);
    }
    else {
      printf("Please enter yes/no\n");
      scanf("%s",answer);
    }
  } 
}


void ask(void) {
  printf("y");

}