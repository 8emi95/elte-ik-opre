//Foglaljunk le területet egy char*-nak, adjuk értékül egy másiknak, majd ellenõrizzük, 
//hogy nem ugyanoda vannak foglalva!

#include <stdio.h>

int main(){
	char* str1="mar kezdem nagyon utalni ezt";
	//str1=(char*)malloc(1);
	char* str2 = str1;
	if(*str1== *str2){
		printf("ugyanott");
	}
	else{
		printf("mashol");
	}
	//free(str1);
}