#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int equals(char* s1, char* s2){
	while (s1 != 0 || s2 != 0){
		if (s1 != s2){
			return 1;
		} else {
			++s1;
			++s2;
		}
	}
	return 0;
}

int main(){
	char str1[] = "alma";
	char str2[] = "al";
	printf("Hello\n");
	equals(str1, str2);
	strcmp(str1, str2);
	return 0;
}