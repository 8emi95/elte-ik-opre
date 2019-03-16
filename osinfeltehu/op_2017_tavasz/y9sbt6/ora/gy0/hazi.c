
#include <stdio.h>
#include <stdlib.h>


/*
  A függvény visszaadja a paraméterül kapott stringet a
  bevezető szóközök elhagyásával
*/
char* remove_space(char* str)
{
  while (*str == ' ')
  {
    str++;
  }
  
  return str;
}

int main(int argc, char** argv)
{ 
  char* str_1 = "    en vagyok a szöveg";
  char* str_2 = remove_space(str_1);
  
  printf("%s\n", str_1);
  printf("%s\n", str_2);  
  
  return 0;
}