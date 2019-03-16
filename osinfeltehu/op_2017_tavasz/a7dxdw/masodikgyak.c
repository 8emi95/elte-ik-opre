#include <stdio.h>
#include <stdlib.h>

int length(char str[])
{
	return (sizeof(str) / sizeof(str[0]));
}


int main()
{

char str[]= "hajra Vasas!";
int hossz=length(str);
printf("%d", hossz);

return (0);
}
