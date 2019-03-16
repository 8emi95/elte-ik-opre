#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

szavazokszama(argv[1]);

if(pipe(fd)>0){
	perror("Hibas a cso letrehozas\n");
	exit(EXIT_FALIURE);
}

pid_t ellen=fork();

if(ellen>0){
	
	pid_t pecsetelo=fork();
	
	if(pecsetelo>0){
		while(szamlalo!=2){;} //pause helyett
		//....
	}
}
else{
	
}