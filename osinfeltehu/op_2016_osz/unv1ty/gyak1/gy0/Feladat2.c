//szó elején elhetnek szóközök -> írjunk fv-t, ami ezeket leszedi

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

char* spaceEraser(char * str)
{
 while (*str == ' ')str++; //*str points to the next character
 return str;
}

int main()
{
 char str1[] = "   Hello";
 printf("without space: \'%s\'\n", spaceEraser(str1));
 return 0;
}
