#include <stdio.h>

int length(char * str)
{
	int l=0;
	while(*str != 0) {
		l++;
		str++;
	}
	return l;
}

void compStr(char* str1, char* str2) {
	if(strcmp(str1, str2) == 0) {
		printf("Egyenlő a két string\n");
	}
	else {
		printf("Nem egyenlő a két string\n");
	}	
}

int main(int argc,char ** argv){
	char str1[] = "Milyen hosszu ez a szoveg";
	int l = length(str1);
	printf("A szovegem hossza %d\n", l);
	
	char str2[] = "alma";
	char str3[] = "korte";
	char str4[] = "alma";

	compStr(str2, str3);
	compStr(str2, str4);
	
	int i;
	printf("Number of command line arguments are: %i\n",argc);
	for (i=0;i<argc;i++){
		printf("%i. argument is %s\n",i,argv[i]);
	}
	
}
