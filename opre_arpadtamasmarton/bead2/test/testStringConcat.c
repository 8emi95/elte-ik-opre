#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char* IntToStr(int number)
{
	char numberStrArray[20];
	sprintf(numberStrArray, "%d", number);
	char *numberStrPointer = (char *) malloc(1 + strlen(numberStrArray));
	strncpy(numberStrPointer, numberStrArray, (int)strlen(numberStrArray));
	return numberStrPointer;
}

int main (void)
{
	int bikeId = 10;
	
	char *bikeIdMessage, *bikeIdStr;
	bikeIdStr = IntToStr(bikeId);
	printf("bikeIdStr: -%s-\n", bikeIdStr);
	printf("bikeIdStr: -%d-\n", (int)strlen(bikeIdStr));
	
	bikeIdMessage = (char*) calloc(strlen(bikeIdStr) + 2, sizeof(char));
	//bikeIdMessage = (char *) malloc(strlen(bikeIdStr) + 2);
	snprintf(bikeIdMessage, strlen(bikeIdStr) + 2, "0%s", bikeIdStr);
	
	printf("EREDMENY: -%s-\n", bikeIdMessage);
	
	return 0;
}
