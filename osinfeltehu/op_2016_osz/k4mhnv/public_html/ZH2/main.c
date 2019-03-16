#include <stdlib.h>  
#include <stdio.h>
#include <sys/ipc.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h> 

int main(int argc,char ** argv) {

struct structMessage{
    long mtype;
    int numberOfToys;
    char message[128];
};

struct paintAttack{
    int success;
    char* message;
};

void handler(){
    printf("A betoro jelzett!\n");
}

    pid_t kevin, bandits;
    key_t key;
    int ktobpipe[2];
    int btokpipe[2];
    int msq;
    struct structMessage data;
    struct paintAttack paint;
    const static struct paintAttack resetPaint;
    const static struct structMessage reset;


    srand(time(NULL));

    key = ftok(argv[0],1); 
    msq = msgget( key, 0600 | IPC_CREAT ); 
    if ( msq < 0 ) { 
         perror("msgget"); 
         return 1; 
    } 

    if (pipe(ktobpipe) == -1 || pipe(btokpipe) == -1) {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }   

    kevin = fork();
    bandits = fork();
    if(kevin < 0 || bandits < 0){
        perror("fork");
    }
    else if(kevin > 0 && bandits > 0){//parent
        int status;

        wait( &status );
        wait( &status );
    }else if(kevin == 0 && bandits != 0){ //kevin
        data = reset;
        int status;
        int randToys = rand()%30 + 20;
        int randsuccess = rand()%100;
        paint = resetPaint;
        paint.success = randsuccess;

        //signal(SIGUSR1, handler);

        close(ktobpipe[0]);
        close(btokpipe[1]);

        data.numberOfToys = randToys;
        data.mtype = getppid();
        status = msgsnd(msq, &data, sizeof(data), 0);
        if(status < 0){
            perror("msgsnd");
        }
        status = msgrcv(msq, &data, sizeof(struct structMessage), getppid(), 0);
        if(status < 0){
            perror("msgsnd");
        }

        printf("Betoro: %s\n", data.message);
        pause();

        printf("k writing\n");
        write(ktobpipe[1], &paint, sizeof(struct paintAttack));
        printf("k reading\n");
        read(btokpipe[0], &paint, sizeof(struct paintAttack));
        printf("Betoro: %s\n",paint.message);

        close(ktobpipe[1]);
        close(btokpipe[0]);

    }else if(bandits == 0 && kevin != 0){ //bandit
        int status;

        close(ktobpipe[1]);
        close(btokpipe[0]);

        status = msgrcv(msq, &data, sizeof(struct structMessage), getppid(), 0);
        if(status < 0){
            perror("msgsnd");
        }
        //printf("Ilyen sok jatekon csusztam el: %d\n", data.numberOfToys);
        strcpy(data.message, "Csak varj, mig a kezunk koze nem kerulsz!");
        status = msgsnd(msq, &data, sizeof(data), 0);
        if(status < 0){
            perror("msgsnd");
        }

        kill(kevin, SIGUSR1);

        printf("b reading\n");
        read(ktobpipe[0], &paint, sizeof(struct paintAttack));

        printf("Betoro: Hanyatt estem!\n");

        printf("paint success %d\n", paint.success);
        int success = paint.success;
        strcpy(paint.message, "Na megallj csak!");
        printf("beleptem\n\n\n");
        write(btokpipe[1], &paint, sizeof(struct paintAttack));
        if(success <50){
            strcpy(paint.message, "Na megallj csak!");
            printf("beleptem");
            write(btokpipe[1], &paint, sizeof(struct paintAttack));
        }else{
            printf("beleptem");            
            strcpy(paint.message, "Ezt nem uszod meg szarazon!");
            write(btokpipe[1], &paint, sizeof(struct paintAttack));
        }

        close(ktobpipe[0]);
        close(btokpipe[1]);
    }

    return 0;
}