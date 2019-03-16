#ifndef UTIL_H
#define UTIL_H
#include "time.h"

char * getlinec(void);

int getintc(int lowerBound, int upperBound);

time_t toTime_t(char* time);

#endif