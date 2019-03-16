#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

char *munkasorok[30];
int arak[30];
int k = 0;

void szures_surgossegre(char surgossegie[4])
{
    char orig_line[160];
    FILE *f;
    f = fopen("output.txt", "r");
    if (f == NULL)
    {
        printf("Jelenleg nincs aktív bejelentés.\n");
        exit(1);
    }

    else
    {
        bool success = false;
        char line[160];

        while (!feof(f))
        {
            fgets(line, sizeof(line), f);

            strncpy(orig_line, line, sizeof(orig_line));
            orig_line[sizeof(orig_line) - 1] = '\0';

            char *splitted_line = strtok(line, ",\n");
            while (splitted_line != NULL)
            {

                if (strcmp(splitted_line, surgossegie) == 0)
                {
                    munkasorok[k] = (char *)malloc(160 * sizeof(char));
                    strcpy(munkasorok[k++], orig_line);
                    success = true;
                }

                splitted_line = strtok(NULL, ",\n");
            }
        }
    }

    fclose(f);
}

void szamla()
{
    FILE *f;
    f = fopen("szamla.txt", "w");

    int i;
    for (i = 0; i < k; ++i)
    {
        fprintf(f, "%sMegbízás teljesítve!\nÖsszesen %i 000 000 000 Ft lesz.\n\n", munkasorok[i], arak[i]);
    }
}

void handler(int signumber) // Signal handler
{
    printf("Az idomár munkára kész!\n");
}

int main(int argc, char *argv[])
{
    k = 0;
    szures_surgossegre("Igen");
    szures_surgossegre("Nem");

    if (k == 0)
    {
        printf("Jelenleg nincs aktív bejelentés.\n");
        exit(1);
    }

    int fid = mkfifo("fifo.ftc", S_IRUSR | S_IWUSR);
    int munkafid = mkfifo("munkafifo.ftc", S_IRUSR | S_IWUSR);
    int rfid = mkfifo("randomfifo.ftc", S_IRUSR | S_IWUSR);
    int fd1, fd2, fd3;

    pid_t idomar, madar;
    int status;
    char sz[160]; // char array for reading from pipe

    signal(SIGTERM, handler);

    idomar = fork();
    if (idomar == 0)
    { // idomar process
        printf("Az idomár most végzi a madár körüli teendőket...\n");
        sleep(2);
        kill(getppid(), SIGTERM);
        sleep(1);

        //char r_to_s[5];
        int r;

        int i;
        for (i = 0; i < k; ++i)
        {
            fd1 = open("munkafifo.ftc", O_RDONLY);
            read(fd1, sz, sizeof(sz));
            printf("\nKövetkező megbízás: %s", sz);
            close(fd1);

            fd3 = open("randomfifo.ftc", O_RDONLY);
            read(fd3, &r, sizeof(int));
            close(fd3);

            fd2 = open("fifo.ftc", O_WRONLY);
            write(fd2, &r, sizeof(int));
            close(fd2);
        }
    }
    else
    {
        madar = fork();
        if (madar == 0) // madar process
        {
            sleep(4);
            int i;
            for (i = 0; i < k; ++i)
            {
                printf("A madár most fogja be a drónt...\n");
                int r = rand() % 5 + 1;

                fd3 = open("randomfifo.ftc", O_WRONLY);
                write(fd3, &r, sizeof(int));
                close(fd3);

                sleep(1);
            }
        }
        else
        {
            // szulo process (kft.)
            pause();

            int fogadott;

            int i;
            for (i = 0; i < k; ++i)
            {
                fd1 = open("munkafifo.ftc", O_WRONLY);
                write(fd1, munkasorok[i], sizeof(munkasorok));
                close(fd1);

                fd2 = open("fifo.ftc", O_RDONLY);
                read(fd2, &fogadott, sizeof(int));
                printf("A madár ennyi idő alatt fogta be a drónt: %i másodperc.\n", fogadott);
                close(fd2);
                arak[i] = fogadott;
            }

            szamla();
            remove("fifo.ftc");
            remove("munkafifo.ftc");
            remove("randomfifo.ftc");
        }
    }
}