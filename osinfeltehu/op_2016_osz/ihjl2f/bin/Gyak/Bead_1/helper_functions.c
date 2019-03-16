#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include "helper_functions.h"

void get_formated_time(char *ts) {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (ts,20,"%G-%m-%d %H:%M",timeinfo);
}

void separator() {
	printf("\n\n******************\n\n");
}