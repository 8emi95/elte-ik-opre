#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

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
