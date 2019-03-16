#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include  <signal.h>

int main(int argc, char** argv){
    int p[2];
    if (pipe(p) == -1){
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid == -1){
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){// utas
        close(p[0]);
        
        int len = strlen(argv[1]);

        write(p[1], &len,sizeof len);

        write(p[1], argv[1],len);
        fflush(NULL);

        close(p[1]);
        exit(0);
    } else{
        close(p[1]);
        int pid2 = fork();
        if (pid2 == -1){
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0){// taxis
            close(p[0]);

            exit(EXIT_SUCCESS);
        } else{//központ 
            int len;

            read(p[0],&len, sizeof len);
            char address[len]; 

            read(p[0],address, len);
            close(p[0]);

            address[len]='\0';

            printf("Kapott lakcim: %s\n", address);
            wait();
        }
	}
	exit(EXIT_SUCCESS);
}