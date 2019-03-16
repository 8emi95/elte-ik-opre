#include "utility.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

char * getlinec(void) {
	printf(">");
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
	if(linep[0] == '\n')
		return getlinec();
	line--;
	*line = '\0';
    return linep;
}

int getintc(int lowerBound, int upperBound)
{
	int ret;
	printf(">");
	while (scanf("%d", &ret) == 0 || ret < lowerBound || ret > upperBound)
	{
		printf("Input error. Please try again!\n>");
		int c;
		while ((c = getchar()) != '\n' && c != EOF ) {}
	}
	getchar();
	return ret;
}

time_t toTime_t(char* time)
{
	struct tm ret;
	char* tmp = strtok(time, " /:");
	ret.tm_mday = atoi(tmp);
	tmp = strtok(NULL, " /:");
	ret.tm_mon = atoi(tmp) - 1;
	tmp = strtok(NULL, " /:");
	ret.tm_year = atoi(tmp) - 1900;
	tmp = strtok(NULL, " /:");
	ret.tm_hour = atoi(tmp);
	tmp = strtok(NULL, " /:");
	ret.tm_min = atoi(tmp);
	tmp = strtok(NULL, " /:");
	ret.tm_sec = atoi(tmp);
	return mktime(&ret);
}