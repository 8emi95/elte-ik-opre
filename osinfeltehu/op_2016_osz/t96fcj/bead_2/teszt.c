#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <stdlib.h>




int main (int argc,char* argv[]) 
{	
	printf("Ezzzz azzz! Futunk!\n");
	int *i=(int *)malloc(10*sizeof(int));
	int j;
	for(j=0;j<10;j++)
	{
		i[j]=j+1;
	}
	
	for(j=9;j>-1;j--)
	{
		printf("%d \n",i[j]);
	}
	(int *)realloc()
	free(i);
	char tmp[10]="kakafej";
	char amessage[] = "this is an array";  /* define an array*/
	char *pmessage = "this is a pointer"; /* define a pointer*/
	printf("Ez az array defi : %s \n",amessage);
	printf("Ez a pointer defi : %s \n",pmessage);
	strcpy(amessage,tmp);
	pmessage=(char *)malloc(7*sizeof(char) );
	memcpy(pmessage,tmp,sizeof(tmp));
	printf("Ez az array defi : %s \n",amessage);
	printf("Ez a pointer defi : %s \n",pmessage);
	
	printf("Befejeztem a palyafutasomat! \n");
	
	
return 0;
}