#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int x)
{
    x = x;
}

void main(void)
{
int option;
    char id[2];
    do{
        printf("Adjon meg egy rendezvényazonosítót: ");
        scanf("%s", &id);
        struct sigaction sigact;
        sigact.sa_handler = handler;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = 0;
        sigaction(SIGUSR1, &sigact, NULL);

        char names[10][50];
        char namesOriginal[10][50];
        int i = 5;
        FILE *f;
        int size = 5;
        int missed;
        char missedNames[10][50];

        int fd[2];
        char buff[300];

        //f = fopen(id, "r");
        //if (f == NULL)
        //{
        //   perror("File opening error\n");
        //    exit(1);
        //}
        char line[50];
		strcpy(namesOriginal[0], "Elek Akos");
		strcpy(namesOriginal[1], "Ivan Rados");
		strcpy(namesOriginal[2], "Murtaz Daushvilli");
		strcpy(namesOriginal[3], "Jagodinskis");
		strcpy(namesOriginal[4], "Liptak Zoltan");

        int Pipe = pipe(fd);
        if (Pipe < 0)
        {
            perror("Pipe error!");
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

            write(fd[1], id, strlen(id));

            if (sigsuspend(&sigset) == -1)
            {
                printf("A partner megérkezett a(z) %s. rendezvényre\n", id);
            };

            write(fd[1], namesOriginal, sizeof namesOriginal);
            sigsuspend(&sigset);

            wait(NULL);

            read(fd[0], missedNames, sizeof missedNames);
            int j;
            for (j = 0; j < size; ++j)
            {
                if (strcmp(missedNames[j],namesOriginal[j]) == 0)
                {
                    printf("Nem volt itt: %s \n", missedNames[j]);
                }
            }
        }
        /* child process */
        else
        {
            memset(buff, 0, strlen(buff));
            read(fd[0], buff, strlen(id));
            kill(getppid(), SIGUSR1);
            printf("A partner megkapta arendezveny azonositojat: %s\n", buff);

            read(fd[0], names, sizeof names);
            kill(getppid(), SIGUSR1);
            printf("Partner: megkaptam a neveket\n");

            srand(time(NULL));
            missed = 0;
            int i;
            for (i = 0; i < size; i++)
            {
                if (rand() % 10 == 0)
                {
                    strcpy(missedNames[i], names[i]);
                }
                else
                {
                    strcpy(missedNames[i], "ittvolt");
                }
            }

            write(fd[1], missedNames, sizeof missedNames);
            exit(0);
            /*****************************************************/
        }
        printf("Ha ki akar lépni nyomja meg a 0-t. Vagy adjon meg másik rendezvény azonositot. ");
        scanf("%d", &option);
    }
    while(option != 0);
}