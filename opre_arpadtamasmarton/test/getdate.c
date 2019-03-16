#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

char* IntToStr(int number)
{
	char numberStrArray[20];
	sprintf(numberStrArray, "%d", number);
	char *numberStrPointer = (char *) malloc(1 + strlen(numberStrArray));
	strncpy(numberStrPointer, numberStrArray, (int)strlen(numberStrArray));
	return numberStrPointer;
}

char* getDateTime()
{
	time_t rawtime;
	struct tm timeinfo;
	char temp[20];
	char *buffer = (char*) malloc(20 * sizeof(char));

	time(&rawtime);

	timeinfo = *localtime(&rawtime);
	strftime(temp, sizeof(temp), "%Y:%m:%d %H:%M:%S", &timeinfo);
	sprintf(buffer, "%s", temp);

	return buffer;
}

// 2015:05:10 02:03:32

int getYear(char *dateTime)
{
	char *to = (char*) malloc(5);
	to = strndup(dateTime, 4);
	return atoi(to);
}

int getMonth(char *dateTime)
{
	char *to = (char*) malloc(2);
	to = strndup(dateTime + 5, 2);
	return atoi(to);
}

int getDay(char *dateTime)
{
	char *to = (char*) malloc(2);
	to = strndup(dateTime + 8, 2);
	return atoi(to);
}

int getHour(char *dateTime)
{
	char *to = (char*) malloc(3);
	to = strndup(dateTime + 11, 2);
	return atoi(to);
}

int getMinute(char *dateTime)
{
	char *to = (char*) malloc(3);
	to = strndup(dateTime + 14, 2);
	return atoi(to);
}

int getSecond(char *dateTime)
{
	char *to = (char*) malloc(3);
	to = strndup(dateTime + 17, 2);
	return atoi(to);
}






void main()
{
	printf("Date: %s\n", getDateTime());
	printf("year: %d\n", getYear(getDateTime()));
	printf("Month: %d\n", getMonth(getDateTime()));
	printf("Day: %d\n", getDay(getDateTime()));
	printf("Hour: %d\n", getHour(getDateTime()));
	printf("Minute: %d\n", getMinute(getDateTime()));
	printf("Second: %d\n", getSecond(getDateTime()));
	getDateTime();
}
