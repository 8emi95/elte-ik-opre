#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

//there is no string type - instead of it You have to use char array
int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}

int main() {
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="My string is here!"; //it's length is the length of the text
  //1. parameter formatstring, next parameter(s) variables
  //%s = string, %i = integer \n = new line e.g.
  
  printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n",str1,(int)length(str1),(int)strlen(str1)); 
  
  char* str2=(char*)malloc(80 * sizeof(char)); //allocate new memory 
  //calloc, realloc, free - functions for allocating memory and freeing up the memory
  
  strcpy(str2,"New content by using strcpy");   
  printf("The new content of variable str2: \'%s\'\n",str2); //new content
  //instead of giving value character by character use function in string.h
  //important other functions: strcmp, strcat, strlen;
      
  free(str2);
  return 0;
}
