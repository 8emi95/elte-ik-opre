#include <stdio.h>
#include <stdlib.h>

void welcome(void) {
  printf("Welcome to the Society of Hungarian Ball Games consultation!\n");
}

void menu(void) {
  printf("Please enter 1, 2, 3, 4 corresponding to the number of your answer\n");
}

void answers(char *str1,char *str2, char *str3, char *str4) {
  printf("1: %s\n",str1);
  printf("2: %s\n",str2);
  printf("3: %s\n",str3); 
  printf("4: %s\n",str4);  
} 


void ask(void) {
  printf();

}