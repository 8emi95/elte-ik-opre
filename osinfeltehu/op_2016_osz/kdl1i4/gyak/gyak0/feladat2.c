#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//majdnem, elvileg a fv jo
//HF szavak jonnek a parancssorbol, ha megtalaljuk, hogy alma, akkor ki kene cserelni korte -re
char* deletewhitespace(char * str)
{
 while (*str==' ') str++;
 return str;
}
int main(char * arg)
{
    printf(deletewhitespace(arg));
  return 0;
}
