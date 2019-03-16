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

char* removeLeadingWhiteSpaces(char * myChar) { // char mutatóval térünk vissza, azt is várunk
	while(' ' == *myChar) ++myChar; // Ha whitespace van, léptetjük a mutatót.
	
	return myChar;
}

int main(int argc, char ** argv)
{
  char * newString = removeLeadingWhiteSpaces(argv[1]);
  printf("%s\n", argv[1]);
  return 0;
}
