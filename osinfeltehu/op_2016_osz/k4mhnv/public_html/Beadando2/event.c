#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 
#include <signal.h>
#include <string.h>

main(int argc, char *argv[]){
    char destination[128];
    char buffer[128];

    int piperead;
    dup2(atoi(argv[1]), piperead);

    printf("Event started\n");
    sleep(3);

    read(piperead, buffer, sizeof(buffer));
    strcpy(destination, buffer);
    printf("destination is: %s\n", destination);

    kill(getppid(),SIGTERM);

    return 0;
}