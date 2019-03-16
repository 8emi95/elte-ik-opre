#include "print.h"

int main() {

  char str1[] = "bill clinton";
  char str2[] = "george bush";
  char str3[] = "george foreman";
  char str4[] = "bill o reilly";

  char file[50];
  
  
  welcome();
  menu();  
  answers(str1,str2,str3,str4);
  finished();
  return 0;
}