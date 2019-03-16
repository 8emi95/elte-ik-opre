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


/*
6. Irjuk ki, ha egy programnak legalabb ket parametere van, es ezek mind megegyeznek!
7. Ellenorizzuk, hogy egy programnak pontosan ket bemeno parametere van, majd irjuk ki ezeknek konkatenaltjat!
*/

int main(int argc, char** argv)
{
  
  char str1[]="Hello world\n"; //it's length is the length of the text
  printf(str1); 
  
  char str2[]="Hello world\n";
  printf(str2); 
  
  int res = compare(str1, str2);
  
  /*
3. 	Foglaljunk le teruletet egy char*-nak, adjuk ertekul egy masiknak, majd ellenorizzuk, 
	hogy nem ugyanoda vannak foglalva!
*/
	char* str3; //it is a pointer variable (we have to allocate memory)
	char* str4;
	str3=str4;
	printf("%p\n%p\n", (void *)str3, (void *)str4);
	
	int comp = strcmp(str3,str4);
	printf("%d\n", comp);
	
//4. Irjuk ki egymas utan a program bemeneti parametereit! Ugyanez forditott sorrendben.	
 
	int i;
    for(i=0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
	
//5. Irjuk ki egy program bemeneti parametereinek hosszainak az osszeget!

	printf("%zu\n", strlen(argv[0]));
  
  return 0;
}
