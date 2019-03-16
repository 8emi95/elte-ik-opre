#include <stdio.h>

int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}

int ugyanaz(char * str1, char * str2){
	if(length(str1) != length(str2)){
		return 0;
	}
	
	//for(int i = 0; i<length())
		
	return 1;
}

int main(){
	char str1[]="Ugyana2z";
	char str2[]="Ugyanaz";
	
	int result = ugyanaz(str1, str2);
	
	if(result == 0){
		printf("A ket string megegyezik");
	}
	
	return 0;
}