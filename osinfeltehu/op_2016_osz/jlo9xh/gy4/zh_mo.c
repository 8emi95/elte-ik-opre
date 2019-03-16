#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
 
int szavazatokszama;
int fd[2];
int szamlalo;
 
void elnok(n) {
    int i, azon;
    printf("Elnok vagyok.\n Johetnek a valasztok!\n");
    close(fd[0]);
    for (i=0; i<n; ++i) {
        azon=rand()%10000;
        write(fd[1], &azon, sizeof(int));
    }
    close(fd[1]);
}
 
 
void pecsetelo() {
    kill(getppid(), SIGUSR1);
    printf("Pecsetelo vagyok.\n");
}
 
void ellenor() {
    kill(getppid(), SIGUSR1);
    int azon;
    close(fd[1]);
    while(read(fd[0], &azon, sizeof(int))) {
           printf("ELLENOR: vallaszto erkezett: azonosito: %d\n", azon);
    }
    close(fd[0]);
}
 
void handler() {
    szamlalo++;
}
 
 
int main (int argc, char *argv[]) {
    szamlalo = 0;
    signal(SIGUSR1, handler);
	
    if (argc < 2) {
        printf("Hianyzo arg.");
        exit(EXIT_FAILURE);
    }
 
    szavazatokszama = atoi(argv[1]);
   
    if (pipe(fd)<0) {
        printf("Cso letrehozasa nem sikerult.");
        exit(EXIT_FAILURE);
    }
 
    pid_t ellen = fork();
 
    if (ellen > 0) {
        pid_t pecset = fork();
 
        if (pecset > 0) {
            while(szamlalo<2);
            elnok(szavazatokszama);
            wait(NULL); wait(NULL);
        }
        else {
            pecsetelo();
        }
    }
    else {
        ellenor();
    }
}