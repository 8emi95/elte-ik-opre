#include <stdio.h>


int length(char * str)
{
	int l=0;
	while(*str != 0){
		l++;
		*str++;
	}
	return l;
}

int main(){
	char str1[] = "Milyen hosszu a szovegem";
	int l = length(str1);
	printf("A szoveg hossza %d\n", l);
}
