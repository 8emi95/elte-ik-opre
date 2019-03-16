#include <stdio.h>

void novel(int * a)
{
	*a = *a + 1;
}

int main()
{
	printf("Hello World!\n");
	
	char str[] = "HelloHello";
	printf("Szia %s Szia\n",str);
	printf("Egy int: %i\n", 5);
	int i = 0;
	i++;
	novel(&i);
	printf("i: %i\n", i);
}
