#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include "survey_system.h"

//static int in_town = 0;

void arrival(int sig) {
    printf("We received message that the commissioner has arrived in town.\n");
    //in_town = 1;
}

void departure(int sig) {
    printf("Commissioner has finished his job and is leaving.\n");
    signal(sig, departure);
    //in_town = 2;
}

int *randomNumbers() {
    int n = total_q();
    int m = 3;

    int in, im;

    static int arr[3];

    im = 0;
    for(in = 0;in < n && im < m;in++) {
        int rn = n-in;
        int rm = m-im;
        if(rand() % rn < rm) {
            arr[im++] = in + 1;
        }
    }
    return arr;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    struct sigaction sigact;
    sigact.sa_handler = arrival;

    signal(SIGUSR1, arrival);
    signal(SIGUSR2, departure);

    int ptc[2]; //parent to child pipe
    int ctp[2]; //child to parent pipe
    int intpipe[2]; // random int pipe
    pid_t pid;
    char city_name[256];

    strcpy(city_name, argv[1]);
    printf("Welcome to %s!\n", city_name);

    if (pipe(ptc) == -1) {
        perror("error piping\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(ctp) == -1) {
        perror("error piping\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(intpipe) == -1) {
        perror("error piping\n");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) {
        perror("error forking\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        kill(getppid(), SIGUSR1);
        close(ptc[1]); //close write
        close(ctp[0]); //close read
        close(intpipe[0]);
        int i, j;
        DS ds[3];

        int people = rand() % 11 + 10;
        int answers[3][people];
        write(intpipe[1], &people, sizeof(people));
        for (i = 0; i < 3; i++) {
            read(ptc[0], &ds[i], sizeof(DS));
            for (j = 0; j < people; j++) {
                int r1 = rand() % count_alt(ds[i]) + 1;
                answers[i][j] = r1;
            }
        }
        write(ctp[1], &answers, sizeof(answers));
        close(ptc[0]); //close read
        close(ctp[1]); //close write
        close(intpipe[1]);
        kill(getppid(), SIGUSR2);
        exit(EXIT_SUCCESS);
    } else {
        pause();
        close(ptc[0]); //close read
        close(ctp[1]); //close write
        close(intpipe[1]);
        int i;

        DS ds[3];
        //int cnt = total_q();
        //int random;
        int size;
        read(intpipe[0], &size, sizeof(size));
        int answers[3][size];
        int *random = randomNumbers();
        for (i = 0; i < 3; i++) {
            //random = rand() % cnt + 1;
            ds[i] = pick_question(random[i]);
            write(ptc[1], &ds[i], sizeof(DS));
        }
        int m, n;
        read(ctp[0], &answers, sizeof(answers));

        for (m = 0; m < 3; m++) {
            printf("------------------------------\n");
            printf("Question %d, possible choices: %d\n", m + 1, count_alt(ds[m]));
            printf("------------------------------\n");
            for (n = 0; n < size; n++) {
                printf("Person #%d picked choice %d\n", n + 1, answers[m][n]);
            }
        }
        printf("------------------------------\n");
        close(ptc[1]); //close write
        close(ctp[0]); //close read
        close(intpipe[0]);
    }


    return 0;
}