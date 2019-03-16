#include <stdio.h>
#include <stdlib.h>

int stringLength(const char *string){
	int length = 0;
	while(string[length] != '\0')
		length++;
	
	return length;
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		printf("Usage: %s string\n", argv[0]);
	} else {		
		printf("Length: %d\n", stringLength(argv[1]));
	}
}
