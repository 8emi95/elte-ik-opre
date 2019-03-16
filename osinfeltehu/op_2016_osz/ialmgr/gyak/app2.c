#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

void stop_handler(int);
void cont_handler(int);

int main() {
    signal(SIGTERM, stop_handler);
    signal(SIGCONT, cont_handler);

    pid_t result = fork();

    if (result < 0) {
        perror("Fork failed.");
        
        exit(EXIT_FAILURE);
    }

    if (result == 0) { //child process
        printf("Child running.\n");
        pause();
        printf("Child running again.\n");
        printf("Stopping again.\n");
        pause();
        printf("Aaaand child is running again.\n");
        //exit(0);
    } else { //parent process
        printf("Parent running.\n");
        kill(result, SIGTERM);
        sleep(3);
        kill(result, SIGCONT);
        printf("Parent running again.\n");
        kill(result, SIGTERM);
        sleep(3);
        kill(result, SIGCONT);
        printf("Parent is back.\n");
        //wait(NULL);
    }
    
    return 0;
}

void stop_handler(int signum) { }

void cont_handler(int signum) { }

