#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv) {
	char str1[] = "123 456 789";

	printf("A szovegem hossza: %d\n", strlen(str1) );
	
	char * pStr1 = str1;
	// Így csak a pointernek új értéket adok
	pStr1 = "abc";
	
	printf("nah mi van itt? %s\n", pStr1);
	printf("es itt? %s\n", str1);
	/*
	// 
	str2 = str1;
	// Így a pointeren keresztül megváltoztatom a "stringem" értékét
	*str2 = "baballlllllllllllll";
	
	printf("nah mi van itt? %s\n", str2);
	printf("es itt? %s\n", str1);
	*/
	
	
	
	printf("pointert leptetetve: %s\n ", pStr1+1);
}	