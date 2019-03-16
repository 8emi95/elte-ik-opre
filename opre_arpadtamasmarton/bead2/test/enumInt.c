#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // DateTime kezeles
#include <fcntl.h> // Rendszerhivasok file
#include <unistd.h> // fork
#include <sys/types.h> // pid_t típus
#include <sys/wait.h> // wait, waitpid
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>


int main (void)
{
	typedef enum {BERELT, UJPEST, OBUDA} station_t;
	station_t tmp = 2;
	
	switch(tmp)
	{
		case BERELT:
			printf("Berelt");
			break;
		case UJPEST:
			printf("Ujpest");
			break;
		case OBUDA:
			printf("Obuda");
			break;
		default:
			break;
	}
	
	return 0;
}
