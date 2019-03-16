#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>

struct data 
{
	char ask[155];
};

int main()
{
	struct data alma;
	printf("Ez az %d:\n", strlen(alma.ask));

	printf("Ez az :\n");
	if(strlen(alma.ask)==0)
	{
		printf("aaaaa\n");
	}
	
	
	return 0;
}
