#include <stdio.h>


void main(){
  
  //how to find the size of data types
  // how to compare diff. variables
  /*
  int num1 = 1, num2 = 2;
  
  printf("is 1 > 2: %d \n\n", num1 > num2);
  
  // if statment
  if(num1 > num2){
    printf("%d is greater than %d \n\n", num1, num2);
  }else if(num1 < num2){
    printf("%d is less than %d \n\n", num1, num2);
  }else{
    printf("%d is equal than %d \n\n", num1, num2);
  }
  */
  
  /*
  // operators
  int cusAge = 21;
  
  if(cusAge > 20 && cusAge < 35)
    printf("they are welcome\n\n");
  else
    printf("They are not welcome\n\n");  
  
  printf("! turns a true into a false: %d\n\n", !1);
  */
  
  // missed less than 10 days work AND has over 30000 
  // OR signed up more than 30 new customers
 /* 
  int missedDays = 8, totalSales = 24000, newCust = 32;
  
  if(missedDays < 10 && totalSales >= 30000 || newCust >= 30)
    printf("You get a raise\n\n");
  else
          ("You don't get a raise \n\n");
  */
  /*
  // shortcut comparison : conditional operator
  int custAge = 21;
  char* legalAge = (custAge>21) ? "true" : "false";
  printf("Is the customer of legal age ? %s\n\n", legalAge);
  
  int numOfProd = 10;
  printf("I bought %s products\n\n", (numOfProd>1) ? "many" : "one"); 
  */
  
  // Size of data types
  //printf("A char takes up %d bytes \n\n", sizeof(char));
 // printf("An int takes up %d bytes \n\n", sizeof(int));
 // printf("A long takes up %d bytes \n\n", sizeof(long));
 // printf("A float takes up %d bytes \n\n", sizeof(float));
 // printf("A double takes up %d bytes \n\n", sizeof(double));
  
 // int bigInt = 2147483647; //this num will appear if bigger neg num will appear ;
 // printf("I'm bigger than you may have heard %d\n\n", bigInt);
  
  /*
  // while loop
  int numHowBig = 0;
  printf("how many bits ? ");
  scanf("%d", &numHowBig);
  
  printf("\n\n");
  
  //0 : print what's given
  //1 : print what's given
  //2 : 1 + 2 = 3 11
  //3 : 3 + 4 = 7 111
  //4 : 7 + 8 = 15 11111 
  
  int myIncr = 1, myMulti = 1, finalVal = 1;
  
  while(myIncr < numHowBig){
    
    myMulti *= 2;
    finalVal = finalVal + myMulti;
    myIncr++;
      
  }
  
  if((numHowBig) == 0 || (numHowBig == 1)){
    printf("Top value: %d\n\n", numHowBig);  
  }else{
    printf("Top value: %d\n\n", finalVal);
  }
  */
  /*
  // while / break loop
  
  int secNum = 10, numGuessed = 0;
  while (1){
    
    printf("Guess my secret number: ");
    scanf("%d", &numGuessed);
    
    if(numGuessed == secNum){
      printf("Good job !!!\n\n");
      break;
    }
  
  }
  */ 
 // do/ while loop  
 /* 
  char sizeOfShirt;
  
  do{
    printf("What size of shirt you need ? (S, M, L): ");
    scanf(" %c", &sizeOfShirt);   
  
  }while(sizeOfShirt != 'S' && sizeOfShirt != 'M' && sizeOfShirt != 'L');  
  
  */
  
  
  //For loop
  
  
  for(int counter = 0; counter <= 40; counter++){
    
    if((counter % 2) == 0) continue;
    printf("%d\n", counter);
  
  } 
    
}