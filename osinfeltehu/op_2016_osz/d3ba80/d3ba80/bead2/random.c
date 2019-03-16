#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // for pipe()
#include <stdlib.h>

void main(int argc, char** argv) {
   	int i;
   	srand(time(NULL));
   	for (i=0;i<10;i++) 
   	{
   		printf("Random: %d\n",rand() % 10);
   	}
}