#ifndef FUNCTIONS_HD_GUARD
#define FUNCTIONS_HD_GUARD
#include <stdio.h>
#include "record.h"

int readInt(int* i,FILE* stream);
int readString (char string[],int slength,FILE* stream);
int flushStream(FILE*stream);
int rangeCheck(int n,int lowerlimit, int upperlimit);
int readIntInRange(int* n,const char* s,int lowerLim,int upperLim );
int writeQuestion(FILE* stream,struct Record* r,int qNum);
int genRandom(int max);
int cmpfunc (const void * a, const void * b);
int genRandom(int max);
#endif