#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



int main()
{
	
	int i=0;
	while(i<10){
	int valtozo;
	valtozo=fork();
	
	
	
	if(valtozo==0) //szulo
	{
		//printf("%d\n", i);
		i++;
	}
	if(valtozo>0) //gyerek
	{
		printf("%d\n", i);
		sleep(1);
		i=100;
	}
}
	return 0;
}