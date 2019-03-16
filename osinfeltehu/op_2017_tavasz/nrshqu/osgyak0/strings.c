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
int main(int argc, char** argv)
{
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="Hello world"; //its length is the length of the text
  char* str2; //it is a pointer variable (we have to allocate memory)
  str2 = (char*)malloc(80*sizeof(char));
  if (argc > 0)
    strcpy(str2,argv[1]);
  int i = 0;
  int equal = 1;
  while (str1[i] && equal)
  {
   equal = (str1[i] == str2[i]);
   i++;
  }
  //TODO hosszösszehasonlítás, eredmény kiírása 
  free(str2);
  return 0;
}
