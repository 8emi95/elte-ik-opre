#include <stdio.h>

int length(char * str) {
	int l=0;
	while(*str != 0) {
		l++;
		str++;
	}
	return l;
}


void compStr(char* str1, char* str2) {
	while(*str1 != 0 && *str2 != 0) {
		str1++;
		str2++;
	}
	if(*str1 == 0 && *str2 == 0) {
		printf("A ket szoveg egyenlo hosszu\n");
	}
	else if(*str1 == 0 && *str2 != 0) {
		printf("A masodik szoveg hosszabb\n");
	}
	else {
		printf("Az elso szoveg hosszabb\n");
	}
}

int main(int argc, char** argv) {
	char str1[] = "Milyen hosszu ez a szoveg\n";
	int l = length(str1);
	printf("\nA szoveg hossza :%i\n", l);

	char str2[] = "Ez egy masik szoveg";	compStr(str1, str2);
	char str3[] = "Ez meg egy harmadik";
	compStr(str2, str3);
	compStr(str3, str1);
	
	char str4[] = "abcdef"; 
	char* ptr_str = str4;
	// ptr_str = &str[0] ugyanaza

	printf("\nAz alabbiakban az adott karaktertol iratunk ki szoveget\n");	

	printf("1.karakterre mutat: %s\n", ptr_str);
	printf("3.karakterre mutat: %s\n", ptr_str + 2);
	printf("4.karakterre mutat: %s\n", &ptr_str[4]);

	printf("\nAz alabbiakban az adott karaktert iratjuk ki\n");

	printf("1.karakterre hivatkozás %c\n", *ptr_str);
	printf("3.karakterre hivatkozas %c\n", ptr_str[3]);
}
