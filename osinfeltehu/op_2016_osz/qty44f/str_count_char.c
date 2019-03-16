#include <stdio.h>
#include <stdlib.h>

int countChar(const char *string, const char c){
	int count = 0;
	int i = 0;
	while(string[i] != '\0')
	{
		if(string[i] == c)
			count++;
		i++;
	}
		
	return count;
}

int main(int argc, char *argv[])
{
	if (argc < 3){
		printf("Usage: %s string char\n", argv[0]);
	} else {
		
		printf("Count: %d\n", countChar(argv[1], argv[2][0]));
	}
}
