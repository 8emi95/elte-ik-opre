#include <stdio.h>

int main()
{
   char* str = "valami";
   int length = 0;
   char* ptr = str;
   while (ptr != 0)
   {
       ptr = ptr + 1;
       length = length + 1;
   }
   printf("&s/n", length);

}
