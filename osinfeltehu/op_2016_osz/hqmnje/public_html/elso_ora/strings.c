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

int egyenloe(char * str1 , char * str2){
	int db=0
	int lengthstr1=length(str1);
	if(lengthstr1==length(str2)){
		while(*str1++==++*str2 && db<lengthstr1){
			db++;
		}
	}
	if(lengthstr1==db){
		return 1;
	}else{
		return 0;
	}
}
int main()
{
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty 
  char str1[]="Hello world\0"; //it's length is the length of the text
  char str2[]="'''";
  
  return 0;
}
