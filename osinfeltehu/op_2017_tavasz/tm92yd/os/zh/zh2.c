#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>

int main(int argc,char ** argv) {
    char *address = argv[1];
    char buffer[256];

//Pipe
    int childpipe[2];

    if (pipe(childpipe) == -1) {
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}

    pid_t child = fork();
    if(child > 0) {
        //Parent - Taxitársaság
        printf("T  --- Taxitarsasag varja az utast!\n");
        close(childpipe[1]);
        read(childpipe[0], buffer, sizeof(buffer));
        close(childpipe[0]);
        sleep(1);
        wait();
        printf("T  --- Utas cime: %s\n", buffer);
        printf("T  --- Vege\n");
    } else {
        //Child 1 - Utas
        printf("U-Utas bejelentkezik a taxitarsasaghoz!\n");
        close(childpipe[0]);
        write(childpipe[1], address, sizeof(buffer));
        close(childpipe[1]);
    }
}