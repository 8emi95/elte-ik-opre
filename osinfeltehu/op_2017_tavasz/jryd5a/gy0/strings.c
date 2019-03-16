#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

char * printTrue(char * str1, char * str2);
//there is no string type - instead of it You have to use char array
int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}
int main()
{
  printTrue("egy", "egy");
//  printTrue("fradi", "ujpest");
//  printTrue("", "");
//  printTrue("1", "2");
/*
  int i = 0;
  while (str1[i] == str2[i]
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="Hello world"; //it's length is the length of the text
  printf("The content of variable str1: \'%s\'\n",str1); 
  //1. parameter formatstring, next parameter(s) variables
  //%s = string, %i = integer \n = new line e.g.
  
  printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n",str1,length(str1),strlen(str1)); 
  //The end of a string is a 0 character 
  //There is the function strlen!! in string.h
  
  char* str2; //it is a pointer variable (we have to allocate memory)
  str2=str1; //the pointer shows to the same memory place
  printf("The content of variable str2 \'%s\' is the same as str1 \'%s\' \n(pointing the same memory place)\n",str2,str1);
  str2=(char*)malloc(80*sizeof(char)); //allocate new memory 
  //calloc, realloc, free - functions for allocating memory and freeing up t*/
 
  return 0;
}

char * printTrue(char * str1, char * str2) {
  if (areEqual(str1, str2) != 0) {
    printf("%s and %s are equal", str1, str2);
  } else {
    printf("%s and %s are not equal", str1, str2);
  }
  printf("\n");
}

int areEqual(char * str1, char * str2) {
  int i = 0;
printf("eddig ok\n");
  while (str1[i] == str2[i]) {
    i++;
  }
  return (str1[i] == 0 && str1[i] == str2[i]);
}
