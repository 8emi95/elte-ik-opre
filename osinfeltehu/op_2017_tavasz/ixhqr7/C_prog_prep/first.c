#include <stdio.h>
#include <string.h>

#define MYNAME "Marouane Blej"

int globalVar = 100;

main(){

  char firstLetter = 'M';
  int age = 21;
  long int bigNum = -32767000;
  float piVal = 3.14159;
  double bigPi = 3.1415912348704134;
  printf("\n");//break line
  printf("\t this will print to the screen\n"); // creats a tab
  printf("I am %d years old \n\n", age );
  printf("Big number %ld\n\n", bigNum);
  printf("Pi = %.5f\n\n", piVal); //%.#  how many dicimal numbers to show
  
  printf("Big Pi = %.15f\n\n", bigPi);
  printf("the first letter in my name is: %c\n\n", firstLetter);  
  printf("My name is: %s  \n\n","Marouane");
  /*  
  // create a string
  char myName[] = "Marouane Blej";// we didn't put a number inside the brackets to make sure it holds the whole thing
  //every character is counted as on, space is couned as one also and there is the string terminator that needs to be counted too.
  //name is 12 characters plus the space and the string terminator it's 14.
  
  //to change the value the string created. it replaces the actual value
  strcpy(myName, "whatever whoever");
  printf("My name is: %s \n\n", myName);
  
  // how to get input from the user: scanf accepts one value at a time unless stated otherwise
  char middleInitial;
  printf("What is your middle intial ? ");
  scanf(" %c", &middleInitial); //& is  upperstand or emperstand more on it later
  printf("Middle initial %c\n\n", middleInitial); // %c accepts character as input
   
  // creat a string array
  char firstName[30], lastName[30];
  // %s accepts string as input
  printf("What is your name ? ");
  scanf(" %s %s", firstName, lastName);
  printf("Your name is: %s %c %s\n\n", firstName, middleInitial, lastName);

  int month, day, year;
  // & is to aasign the input value to that specific variable  
  // %d accepts integer as input
  printf("What is your birth date? ");
  scanf("%d/%d/%d", &month, &day, &year);  
  printf("\nBirth Date is: %d/%d/%d\n\n", month, day, year);
  
  */
  // Mathmatics in C
  /*
  int num1 = 12, num2 = 15, numAns;
  float decimal1 = 1.2, decimal2 = 1.5, decimalAns;
  printf("Integer Calculation: %d\n\n", num2 / num1);  
  printf("Float Calculation: %f\n\n", decimal2 / decimal1); // %f for floats
  printf("Modulus %d\n\n", num2 % num1); 
   /
  
  int randNum = 1;
  
  printf("1 += 2: %d\n\n", randNum += 2);
  printf("%d += 2: %d\n\n", randNum, randNum += 2);
  
  int exNum = 1;
  
  printf("++%d : %d \n\n", exNum, ++exNum);
 
  exNum = 1;
 
  printf("%d++ : %d \n\n", exNum, exNum++);
  */
  
  // How to cast a value
  int numEx = 12;
  int numEx2 = 14;
  
  printf("numEx / numEx2: %f \n\n", (float) numEx / numEx2);
  
}