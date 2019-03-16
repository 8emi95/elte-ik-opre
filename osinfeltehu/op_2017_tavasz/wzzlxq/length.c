#include <stdio.h>

int length(char *mut);

int main(){
	char* str="";
	int hossz= length(str);
	printf("Salamalejcum %d\n" ,hossz);
}

int length(char *mut){
	int l=0;
	while(mut!="\0")
	{
		l=l+1;
		mut=mut+1;
	}
	return l;
}