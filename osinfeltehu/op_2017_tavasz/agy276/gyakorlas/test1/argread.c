#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(int arg, char *argc[]){
	printf("arg: %i\n", arg);
	long loop;
	for(loop = 1; loop < arg; loop++){
		printf("argc[%d]: %s, ",loop+1, argc[loop]);
	}

return 0;
}
