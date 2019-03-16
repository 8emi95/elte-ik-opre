#include <stdio.h>
#include <stdlib.h>

/*void pointerRandNum(int* rand1, int* rand2){
  *rand1 = rand() % 50 + 1;
  *rand2 = rand() % 50 + 1;
  
  printf("new rand1 in function = %d\n\n", *rand1);
  printf("new rand2 in function = %d\n\n", *rand2);
  
}*/

//pointers and strings

void editMsgSent(char * message, int size){
 char newMsg[] = "New Message";

 if(size > sizeof(newMsg)){
   for(int i =0; i < sizeof(newMsg); i++){
    message[i] = newMsg[i];
   }
 }else {
  printf("New message is too big.");
 }

}


void main(){

  //Pointer, arrays and functions
/*  int rand1 = 12, rand2 = 15;
  
  printf("rand1 = %p : rand2 = %p\n\n", &rand1, &rand2);//%p hexadecimal version
  
  printf("rand1 = %d : rand2 = %d\n\n", &rand1, &rand2);
  
  printf("Size of int %d\n\n", sizeof(rand1));
  
  int * pRand1 = &rand1; //pRand for declaring a pointer
  
   printf("pointer = %p\n\n", pRand1);
   
   printf("pointer = %d\n\n", pRand1);

   //derefrencing the pointer
   
   printf("Value %d\n\n", *pRand1);

   //Arrays and Pointers
   int primeNum[] = {2,3,5,7};
   printf("First index: %d\n\n", primeNum[0]);//or
   printf("First index: %d\n\n", *primeNum);// * is the first element
 
   
   char * students[4] = {"Anna","Paolo","Me","Pollo"};
   
   for(int i = 0; i < 4; i++){
    
    printf("%s : %d\n\n", students[i], &students[i]);
   
   }
*/ 
   
   //Pointers and functions
    /*
    int rand1 = 0, rand2 = 0;
    
    printf("Main before func call\n\n");
    
    printf("rand1 = %d\n\n", rand1);
    printf("rand2 = %d\n\n", rand2);
    
    pointerRandNum(&rand1, &rand2);

    printf("Main after func call\n\n");

    printf("rand1 = %d\n\n", rand1);
    printf("rand2 = %d\n\n", rand2);
   */ 
   
   //pointer and strings
   
   char randMsg[] = "Edit my function";
  
   printf("Old message: %s\n\n", randMsg);
  
   editMsgSent(randMsg, sizeof(randMsg));
   
   printf("New message: %s\n\n", randMsg);
   
    
    
}