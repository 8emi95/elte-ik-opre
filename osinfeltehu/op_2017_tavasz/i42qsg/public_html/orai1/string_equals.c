#include<stdlib.h>
#include<stdio.h>
#include<string.h>


//if the strings are equal this function will return 0, if they are not, something else (number)

int length(char * str){
int counter = 0;
	while(*(str+counter)!=0){
		counter++;

	}	
	return counter;
}

int equals( char*  string1, char* string2){
	if(length(string1)!=length(string2)){
		return 2;
	}
	int where=0;
	while(*(string1+where) ==  *(string2+where)){
	//vizuális összehasonlítás  printf("%c %c", *(string1+where), *(string2+where));
	where++;
	}
	if(where==length(string1)+1){
		return 0;
	}else{
		return where;
	}
}

int main(int argc, char* argv[]){
	
char str1[18] = "Azta it's working";
char str2[18] = "Azta it's working";
char str3[18] = "Azta it's workins";

printf("%d\n",equals(str1,str2));

}
