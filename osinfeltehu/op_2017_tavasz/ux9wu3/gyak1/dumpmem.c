#include <stdio.h>
#include <stdlib.h>

int main()
{
 printf("lol\n");
 char* my;
 my = (char*)malloc(80*sizeof(char));
 printf("lol: %s", my);
 free(my);
 return 0;
}
