/*
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <time.h> //system time
*/
#include "t_header.h"


int main(int argc,char *argv[]){
    loadPollIfExists();
	menu();
	return 0;
}
