#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

char* removeSpace(char * str)
{
 while (*str==' ')str++; //*str points to the next character
 return str;
}

int main(int argc, char ** argv)
{
int i;
char str1[]="            Hello";
printf("The content of variable str1: \'%s\'\n",str1); 
printf("The variable without spaces str1 \'%s\'\n",removeSpace(str1));

}
//h�zi: j�nnek szavak a paramba, ha alma akkor legyen korte; foglalni kell�j helyet �s abba �rni az �rt�ket