#include <stdlib.h> // exit() ...
#include <stdio.h> // printf() ...

// In the start function you can do whatever you want.
void start (const int azon) {
    // For children processes
    srand( time(NULL) );
    unsigned t = rand()%5; // printf("%d\n", t);
    sleep(t);
    printf("%d. process reached the end.\n", azon);
    exit(0);
}

int main() {
    const int N = 2;
    pid_t pids[N];

    int i;

    // The 'for' loop make 'N' process with 'fork()'.
    // The children processes will call the start function.
    // Since after fork() you will get 2 process. One Parent, and One Child
    // The returning value from fork() is saved in "pids" which is an
    // integer AND it is (<0) IF something went wrong.
    //             it is (>0) IF 'we are' in the Parent process,
    // because this number is the Child process' ID (pid).
    // and Last    it is (==0) IF 'we are' in the Child process.
    for (i = 0; i < N; i++) {
		printf("Egy fork hívás start!\n");
        pids[i] = fork();
        sleep(1);
		printf("Egy fork hívás vége!\n");
        if (pids[i] == 0) start(i+1); // ... OR you can make a switch(..)
    }
    // This 'for' loop in the wait(NULL) statement ONLY move on when a
    // process ended, so it waits until 'N' proc ends.
    for (i = 0; i < N; i++)
        wait(NULL);

    printf("Partent process reached the end\n");

    return 0;
}