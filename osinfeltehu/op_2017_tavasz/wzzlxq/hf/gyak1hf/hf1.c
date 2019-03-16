//Definiáljunk egy stringet majd írjuk ki a hosszát!

#include <stdlib.h>	//mallochoz kell
#include <stdio.h>	//ki es bemeneti fuggvenyek pl printf


int main(){
	char *str="Helloaaa";
	int h = length(str);
	//int h = length2(str);
	//str= (char*)malloc(length(argv[0])*(sizeof(char)));
	printf("Argumentum hossza: %d\n", h);
	//free(str);
}

int length(char *x){
	int db=0;
	while(1)
	{
		if(*x=='\0'){
			return db;
		}
		x++;
		db++;
	}
	return db;
}

int length2(char *x){
	char *ptr;
	while(*x++);
	return --x-ptr;
}