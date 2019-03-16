#include <stdio.h>
#include <stdlib.h> // to use exit()
#include <string.h> //to use strings .. strcmp

int globalVar = 0;

int add2Int(int num1, int num2){

  return num1 + num2;
  
}

void changeVar(){
  
  int age = 40;
  printf("age inside the changeVar function is = %d\n\n", age);
  
  globalVar = 100;
  
  printf("glovalVar inside the changeVar function is = %d\n\n", globalVar);

}

void main(){

  // the switch statment
/*
  int whatToDo = 0;
  
  do{
    printf("\n");
    printf("1. what time is it ? \n");
    printf("2. what is today's date ? \n");
    printf("3. what day is it ? \n");
    printf("4. Quit\n");
    
    scanf(" %d", &whatToDo);
      
  
  }while(whatToDo < 1 || whatToDo > 4);
  /*
  if(whatToDo == 1){
    printf("Print the time.\n");  
  }
  else if(whatToDo == 2){
      printf("Print date.\n");
  }
  
  else if(whatToDo == 3){
      printf("Print day.\n");
  }
    
  else{
      printf("Bye.\n\n");
      exit(0);
  }
  */
  // switch is better;
 *  
  switch(whatToDo){
    case(1) : printf("Print the time.\n");
    break;
    case(2) : printf("Print date.\n");
    break;
    case(3) : printf("Print day.\n");
    break;
    default : printf("Bye.\n\n");
    exit(0);
    break;  
  }  
 
  
  //Arrays
  /*
  char wholeName[14] = "Marouane Blej"; // array of size 14
  int primeNum[3] = {2,3,5}; // array of size 3
  int morePrime[] = {13, 17, 19, 23};// array that can be modified depending on its content.
  char newCity[] = "Paris";
  printf("The first prime in the list is %d\n\n", primeNum[0]);
    
  // array observation.
  char yourCity[30];
  printf("What your city's name ? ");
  // fgets: better than scanf and adds the null char, you must provide the data (how big the array is ...)
  fgets(yourCity, 30, stdin);
  printf("Hello %s\n\n", yourCity);
  
  for(int i=0; i< 30; i++){
    if(yourCity[i]=='\n'){
      yourCity[i] = '\0';
      break;
    }
  }
  printf("Hello %s\n\n", yourCity);
  
  
  //string comparison function strcmp
  printf("Is your city Paris? %d\n\n", strcmp(yourCity, newCity));  
  
  char yourState[] = ", Hungary";
  strcat(yourCity, yourState);
  
  printf("You live in %s\n\n", yourCity);
  
  printf("Letters in Paris: %d\n\n", strlen(newCity));
  //string copy: strcpy overrides certain parts of the memory
  //strlcpy or strncpy  is better than strcpy, since it copies the string but does not override it
  strncpy(yourCity,
          "Casablanca",
          sizeof(yourCity));
  
  printf("New city is: %s\n\n", yourCity);
  */
  
  
  //global var. local var. functions...
  // function (first lines)
  
  int total = add2Int(3,5);
  printf("The sum is %d\n\n", total); 
        
  int age = 10;
  globalVar = 50;
  printf("Age before a call to the function = %d\n\n", age);
  
  printf("global Variable before a call to the function = %d\n\n", globalVar);
  
  changeVar();
  
  printf("Age after a call to the function = %d\n\n", age);
  
  printf("global Variable after a call to the function = %d\n\n", globalVar);
    
}