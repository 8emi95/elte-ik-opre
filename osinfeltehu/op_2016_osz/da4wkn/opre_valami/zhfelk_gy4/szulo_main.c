#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "projhead.h"

void szulo_main(int argc, char* argv[], int pfds[]){
    pause();
    pause();
    
    int num_voters; 
    if (argc != 2){
        printf("SZULO: Incorrect arguments.\n");
        exit(1);
    }
    num_voters = strtol(argv[1], &argv[2], 10);
    printf("SZULO: Found %d voters\n", num_voters);
    
    int id= 0;
    int terminate = -1;   
    for (int i = 0; i < num_voters; ++i){
        int id = rand() % 100;
        write(pfds[1], &id, sizeof(int));
    }
    write(pfds[1], &terminate, sizeof(terminate));
    close(pfds[1]);
}