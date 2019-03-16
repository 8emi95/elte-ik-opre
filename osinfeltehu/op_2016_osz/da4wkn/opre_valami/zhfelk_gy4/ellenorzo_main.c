#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include "projhead.h"

void ellenorzo_main(int pfds[]) {
    kill(getppid(), SIGUSR1);    
    close(pfds[1]); // won't write
    
    int id = 0;
    // while (id != -1){
    for (int i = 0; i < 5; ++i) {
        if (id != -1 )
            printf("ELLENORZO: Got id from SZULO: %d\n", id);
        read(pfds[0], &id, sizeof(int));
    }
    close(pfds[0]);
}