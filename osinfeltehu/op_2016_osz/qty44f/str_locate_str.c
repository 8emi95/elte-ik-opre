#include <stdio.h>
#include <stdlib.h>

int stringLength(const char *string){
	int length = 0;
	while(string[length] != '\0')
		length++;
	
	return length;
}

int locateStr(const char *string, const char *substr){
	int i = 0;
	while(i < stringLength(string))
	{
		if(string[i] == substr[0])
		{
			int j = i;
			int k = 0;
			while(k < stringLength(substr) && substr[k] == string[j]) {
				k++;
				j++;
			}
			if(k == stringLength(substr))
				return i;
		}
			
		i++;
	}
		
	return -1;
}

int main(int argc, char *argv[])
{
	if (argc < 3){
		printf("Usage: %s string string\n", argv[0]);
	} else {
		
		printf("Position of substring: %d\n", locateStr(argv[1], argv[2]));
	}
}
