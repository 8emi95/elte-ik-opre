#include "functions.h"
#include <limits.h>
#include <math.h>
#include <string.h>


int readInt(int* i,FILE* stream){
	int maxDigit = floor(log10(INT_MAX)) + 1;
	
	char maxIntString[maxDigit];
	sprintf(maxIntString,"%i",INT_MAX);
	
	char buffer[maxDigit+2];
	
	if( readString(buffer,sizeof(buffer)/sizeof(buffer[0]),stream) ) 
		return 1;
	
	int j = 0,isInt = 1,isNum = 1;
	while( buffer[j] != '\0' && isNum ){
		isNum = (0 <= buffer[j]-'0' && buffer[j]-'0' <= 9);
		isInt = isInt * (0 <= buffer[j]-'0' && buffer[j]-'0' <= maxIntString[j]-'0');
		j++;
	}
	
	if(j > maxDigit+1 || !isNum || ( j == maxDigit && !isInt )){
		printf("Not a valid number!\n");
		return 1;
	}
	
	*i = atoi(buffer);
	return 0;		
};

int readString (char string[],int slength,FILE* stream){
	string[0] = '\0';
	fgets(string,slength,stream);
	int length = strlen(string);
	
	if(string[length-1] != '\n'){
		printf("Too long(max: %i)!\n",slength-2);
		flushStream(stream);
		return 1;
	}
	
	string[length-1] = '\0';
	
	if(string[0] == '\0'){
		printf("Must enter at least one character!\n");
		return 1;
	}
	
	return 0;
};

int flushStream(FILE*stream){
	char c;
	while((c=getc(stream)) != '\n' && c != EOF);
	return 0;
};

int rangeCheck(int n,int lowerlimit, int upperlimit){
	return !(lowerlimit <= n && n <= upperlimit);
};

int readIntInRange(int* n,const char* s,int lowerLim,int upperLim ){
	int repeat = 0;
	do{
		printf("%s\n",s);
		repeat = readInt(n,stdin);
		if( repeat == 0 && rangeCheck(*n,lowerLim,upperLim) ){
			repeat = 1;
			printf("Not a valid input(%i-%i)!\n",lowerLim,upperLim);
		}	
	}while(repeat);
	return 0;
};

int writeQuestion(FILE* stream,struct Record* r,int qNum){
	
	fprintf(stream,"%i.Question: %s \n",qNum,(*r).question);
	int j,aCount=0;
	for(j=0;j < 4;j++)
		if((*r).answers[j][0] != '\0'){
			aCount++;
			fprintf(stream,"Answer %i. : %s \n",j+1,(*r).answers[j]);
			}
	fprintf(stream,"Date: %s \n\n",(*r).time);
	return aCount;
};

