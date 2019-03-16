#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

struct Data{
    char feladat[100];
    char nev[20];
    int sorsz;
};

void handler(int x)
{
    x = x;
}

int main(int argc, char *argv[])
{
    struct sigaction sigact;
    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1, &sigact, NULL);
    
    int fd[2];
    struct Data buff;
    char name[20];
    strcpy(name, argv[1]);
    int Pipe = pipe(fd);
    if (Pipe < 0)
    {
        perror("Pipe(fd) error!");
        exit(-1);
    }

    pid_t child = fork();
    if (child < 0)
    {
        perror("fork() error!");
        exit(-1);
    }

    /* parent process */
    if (0 != child)
    {
        sigset_t sigset;
        sigfillset(&sigset);
        sigdelset(&sigset, SIGUSR1);

        buff.sorsz = 2;
        strcpy(buff.feladat, "Menj át a vizsgán.");
        strcpy(buff.nev, name);

        write(fd[1], &buff, sizeof(buff));

        if (sigsuspend(&sigset) == -1)
            {
                printf("A versenyző üzenetet küldott\n");
            };  

        //wait(NULL);
        char str[50];
        read(fd[0], str, sizeof(str));

        printf("A versenyző üzenete: %s\n", str);
        
        
    }
    /* child process */
    else
    {
       
        read(fd[0], &buff, sizeof(buff));
        printf("%s megkapta a feladatát ami: %s \nŐ a %d. \n", buff.nev, buff.feladat, buff.sorsz);
        int sikerult = 0;
        srand(time(NULL));
        char str[50];
        if (rand() % 10 == 0)
            {
                sikerult = 1;
                printf("Sikerült megoldanom a feladatot!!! :)\n");
                strcpy(str, "Sikerült megoldanom a feladatot!!! :)");
            }else{
                printf("Nem sikerült megoldanom a feladatot! :(\n");
                strcpy(str, "Nem sikerült megoldanom a feladatot! :(\n");
            }
        
        
        
        kill(getppid(), SIGUSR1);
        write(fd[1], str, sizeof(str));
        exit(0);
        /*****************************************************/
    }

    return 0;
}
