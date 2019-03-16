#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
 char str[80];
  printf("Enter a string: ");
  fgets(str, 10, stdin);
  
  char str2[80];
scanf("%c", &str2);
 
   printf("This is your string: %s", str);
   printf("This is your 2. string: %s", str2);

 return 0;
}