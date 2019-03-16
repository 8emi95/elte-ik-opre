#include "strings.h"

int length(char* str)
{
  int length = 0;
  while(*(str++) != 0) ++length;
  return length;
}

char* truncate(char* str)
{
  int i = 0;
  while(*str == ' ') str++;
  return str;
}

int main(int count, char** args )
{
  char hello[] = "Hello World";
  char* second = hello;
  
  printf("%s\n", second);
  
  char* third = (char*)malloc(80 * sizeof(char));
  third[0] = 'M';
  third[1] = 'I';
  third[2] = 'V';
  third[3] = 0; 
  
  printf("%s\n", third);
  
  strcpy(third, "MIIIVAN");
  
  printf("Length: %i\n", length(third));
  printf("Copy: %s\n", third);
  
  char* x = "  hello x ";
  x = truncate(x);
  printf("Truncate: %s\n", x);
  
  return 0;
}
      