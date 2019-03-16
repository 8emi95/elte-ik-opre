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

//strcmp függvény ua. csinálja
int compare(char* str1, char* str2)
{
	while( str1 != 0 || str2 != 0 )
	{
		if ( str1 != str2 )
		{
			return 1;
		}
		else 
		{
			str1++;
			str2++;
		}
	}
	return 0;
}

int main()
{
  
  char str1[]="Hello world\n"; //it's length is the length of the text
  printf(str1); 
  
  char str2[]="Hello world\n";
  printf(str2); 
  
  int res = compare(str1, str2);
  printf(res);
  
  return 0;
}
