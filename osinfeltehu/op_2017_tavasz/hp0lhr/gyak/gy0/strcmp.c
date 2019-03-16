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
int main()
{
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="Hello world"; //it's length is the length of the text
  char str2[]="Hello world";
  
  int i;
  int azonos = 1;
  for (i=0; i<length(str1); ++i){
    if (str1[i] != str2[i]) {azonos = 0;}
  }
  if (azonos) {printf("Azonos");}
  else  {printf("Nem azonos");}

  return 0;
}
