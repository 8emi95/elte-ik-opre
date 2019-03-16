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

char * RemoveSpaces(char * chp){
while (*chp==' ') chp++;
return chp;
}

int main()
{
  char str[]= "     alma";
  printf("_%s\n", str);

  printf("_%s\n", RemoveSpaces(str));

  return 0;
}
