#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	srand(time(NULL));
	int i;
	int r;
	for(i=0;i<100;i++)
	{	r = rand()%100;
		if(r > 50 && r < 75)
		printf("%i\n", r);
	}	
	return 0;
}
