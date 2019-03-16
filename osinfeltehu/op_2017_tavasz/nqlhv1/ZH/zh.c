#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

int gyerekSzam = 2;
int maxgyerek = 2;
void handler(int signumber){}


int main(int argc, char **argv)
{

    int cso[2];
    pid_t pids[maxgyerek];
    if (argc <2)
    {
         printf("Hiba argumentum\n");
        exit(1);
    }
    if (pipe(cso) ==-1)
    {
        printf("Hiba pipe\n");
        exit (1);
    }
    int i;
    int pid;
    for(i = 0; i < gyerekSzam; ++i)
    {
        pid =fork();
        if(pid<0)
        {
            printf("Hiba a %d gyerek letrehozasa kozben!\n",i);
            exit(EXIT_FAILURE);
        }
        else if (pid==0)
        {
            printf("%d. gyerek indul\n", i);
            char address[100] ="";
            int j;
            for (j =1; j<argc; j++)
            {
                strcat(address, argv[j]);
                strcat(address, " ");
            }
            write(cso[1], &address, sizeof(address));
            close(cso[1]);
            close(cso[0]);
            printf("%d gyerek befejezve\n", i);

        }
        else
        {
            pids[i] = pid;
        }

    }
    if (pid >0)
    {
        char valami [100];
        read(cso[0], &valami, sizeof(valami));
        printf("Az utas cime: %s\n", &valami);
    }
    int db = 0;
    int status;
    pid_t pid1;
    while (db<gyerekSzam)
    {
        pid1 = wait(&status);
        ++db;
    }
    waitpid(pid1, cso, 0);
    close(cso[1]);
    close(cso[0]);

return 0;
}
