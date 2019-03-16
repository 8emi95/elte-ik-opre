#include <string.h>
#include <stdio.h>


int length (const char *szo)
{
	int i=0;
	while (*(szo+i)!='\0')   /*   while(szo[i]!=0) vagy '0'*/
 	{
		i=i+1;
	}
	return i;
}

int main(int argc, char *argv[])
{
	printf("%i\n", length(argv[argc-1]));
	return 0;
}

/*
int length (char *pointer)
{
	char *start;
	start=pointer;
	//int len=0;
	while(*pointer++!=0);//while(*pointer++)
	{
		//++len;
	}
	retutn --pointer-start;
}*/