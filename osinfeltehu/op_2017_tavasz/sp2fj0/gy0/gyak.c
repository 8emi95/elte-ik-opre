#include <stdio.h>

void novel(int * a)
{
	*a = *a + 1;
}

int main()
{
	printf("hello world\n");
	char str[]= "hellobello ";
	printf("Szia %s Szia\n",str);
	printf("Egy int: %i \n", 5);
	int i = 0;
	i++;
	++i;
	novel(&i);
	printf("novelt ertek: %i\n", i);
	return 0;
}
