#include<stdlib.h>
#include<stdio.h>
int length(char* ourStr){
int counter =0;
	while(*(ourStr+counter)!=0){
		counter++;
	}
return counter;
}

int main(){
 char szoveg[13] = "Azta working";
	printf("%d\n",length(szoveg));
}
