#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	
	char str[24] = "First string";
	char *ptr = "Second string";

	printf("str = %s\n",str);
	printf("ptr = %s\n",ptr);

	ptr++;
	printf("ptr = %s\n", ptr);

	str  = str+1;
	printf("str = %s\n",str);

	
	return 0;
}