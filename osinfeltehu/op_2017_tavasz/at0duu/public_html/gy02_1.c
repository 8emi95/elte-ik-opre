#include <stdio.h>
#include <string.h>

int length(char *ptr){
	int n = 0;
	while (*ptr != '\0'){
		n++;
		ptr++;
	}
	return n;
}

int main(){
	char *str = "ez egy szoveg";
	printf("'%s' hossza: %d\n", str, length(str));
}