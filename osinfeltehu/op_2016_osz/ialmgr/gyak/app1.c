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
    signal(SIGSTOP, stop_handler);
    signal(SIGCONT, cont_handler);

    int i;
    int fd;
    int fid = mkfifo("fifo.ftc", S_IRUSR | S_IWUSR);

    if (fid == -1) {
        perror("Error");

        exit(EXIT_FAILURE);
    }

    printf("Fifo done.\n");

    pid_t result = fork();

    if (result < 0) {
        perror("Fork failed.");
        
        exit(EXIT_FAILURE);
    }

    if (result == 0) { //child process
        //char buffer[1024] = "empty";
        printf("Child running.\n");
        //pause();
        printf("Child running again.\n");
        //fd = open("fifo.ftc", O_RDONLY);
        //read(fd, buffer, sizeof(buffer));
        //close(fd);
        //printf("%s\n", buffer);
        exit(0);
    } else { //parent process
        //kill(SIGSTOP, result);
        printf("Parent running.\n");
        kill(SIGSTOP, result);
        sleep(1);

        //fd = open("fifo.ftc", O_WRONLY);

        //for(i = 0; i < 10; i++) {
        //    write(fd, "asd\n", 4);
        //}

        kill(SIGCONT, result);
        //close(fd);
        //kill(result, SIGUSR1);
        //unlink("fifo.c");
        printf("Parent running again.\n");
        wait(NULL);
    }
    
    return 0;
}

void stop_handler(int signum) {
    printf("Paused..\n");
}

void cont_handler(int signum) {
    printf("Continued..\n");
}

