#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <time.h>	// time_t, time, localtime
#include <unistd.h>     // access

#define MAX_KERDES_NAME_LENGTH 20
#define MAX_KERDES_LENGTH 100
#define MAX_KERDES_COUNT 20
#define MIN_VALASZ_COUNT 2
#define MAX_VALASZ_COUNT 4
#define MAX_VALASZ_LENGTH 100

#define MAX_DATETIME_LENGTH 30

typedef enum{true=1,false=0} bool;

struct Kerdes{
    char kerdes[MAX_KERDES_LENGTH];
    char valasz[MAX_VALASZ_COUNT][MAX_VALASZ_LENGTH];
};

static inline char *getTime(char *otime){
 	time_t rawtime;
 	struct tm *ido;
	time(&rawtime);
	ido = localtime(&rawtime);

	sprintf(otime,"[%d-%d-%d %d:%d:%d]",ido->tm_year+1900,ido->tm_mon+1,ido->tm_mday,ido->tm_hour,ido->tm_min,ido->tm_sec);
	return otime;
}

static inline bool checkFileExistence(char *filename){
        return (access(filename, F_OK) != -1);
}

static inline void readLine(char *str){
        str[0] = '\0';
        char buf[100];
        scanf("%s",str);

        while( getchar() != '\n' ){
                scanf("%s",buf);
                strcat(str," ");
                strcat(str,buf);
        }
}

#endif