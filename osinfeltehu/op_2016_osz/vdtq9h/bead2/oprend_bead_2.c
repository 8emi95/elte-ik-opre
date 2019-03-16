#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct observation
{
    long mtype;
    char mtext[100];
} Observation;

const char* exp_names[] = {"medve", "farkas", "oroszlan", "beka", "mokus", "majom", "rigo", "kigyo", "kecske", "elefant"};

void handler(int sig, siginfo_t* info, void* context)
{
    //printf ("beerkezo jelzes (%d). pid: %d\n", sig, info->si_pid);
    switch (sig)
    {
        case 10:
            //printf("expedicio megkezdi a munkat\n");
            break;
        case 12:
            //printf("expedicio a helyszinre ert\n");
            break;
    }
}

void init_random(int i)
{
    srand(time(NULL) + i * 1024);
}

int generate_number(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

int generate_duration()
{
    return generate_number(2, 5);
}

int show_usage()
{
    printf("Hasznalat: bead2.o <expediciok_szama> <megfigyelesi_ido>\n");
    printf("Az expediciok szama legfeljebb 10 lehet!\n");
    printf("A megfigyelesi idonek legalabb 6 masodpercnek kell lennie!\n");
    return -1;
}

int main(int argc, char *argv[])
{
    //Paraméterek ellenőrzése
    if (argc < 3)
    {
        return show_usage();
    }
    const int N = atoi(argv[1]);
    const int T = atoi(argv[2]);
    if (T < 6 || N > 10)
    {
        return show_usage();
    }

    //Jelzések kezelése
    struct sigaction sigact;
    sigact.sa_sigaction = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
    sigaction(SIGUSR1,&sigact,NULL);
    sigaction(SIGUSR2,&sigact,NULL);

    pid_t pids[N];
    pid_t parent_id = getpid();

    //Pipe létrehozása
    int pipefd[2];
    char sz[100];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //Üzenetsor létrehozása
    int queue;
    key_t queue_key;

    queue_key = ftok(argv[0], 1);
    queue = msgget(queue_key, 0600 | IPC_CREAT);
    if (queue < 0)
    {
        perror("msgget");
        return 1;
    }

    printf("expediciok kikuldese...\n");

    //Gyerekek létrehozása
    int i;
    for (i = 0; i < N; ++i)
    {
        if (getpid() == parent_id)
        {
            if ((pids[i] = fork()) < 0)
            {
                perror("fork");
                abort();
            }
            else if (pids[i] == 0)
            {
                kill(getppid(), SIGUSR2);
                pause();

                close(pipefd[1]);
                int read_status = read(pipefd[0], sz, sizeof(sz));

                if (read_status == -1)
                {
                    perror("read");
                }

                close(pipefd[0]);

                printf("azonosito megkapva, a(z) %s csapat megfigyelesre kesz\n",sz);
                kill(getppid(), SIGUSR2);

                init_random(i);
                int duration = generate_duration();
                sleep(duration);

                int young = generate_number(1, 20);
                int adult = generate_number(10, 35);
                int old = generate_number(5, 15);

                char buffer[1024];
                sprintf(buffer, "%s,%d,%d,%d", sz, young, adult, old);

                Observation obs;
                obs.mtype = 1;
                strcpy(obs.mtext, buffer);

                int status;
                status = msgsnd(queue, &obs, strlen(obs.mtext) + 1, 0);

                if (status < 0)
                {
                    perror("msgsnd");
                }

                printf("a(z) %s csapat befejezte a munkat\n", sz);

                exit(0);
            }
            pause();
        }
    }

    //Várakozás, hogy minden készen álljon
    printf("minden expedicio megerkezett\n");
    sleep(1);

    //Azonosítók kiosztása a pipe segítségével
    close(pipefd[0]);
    int write_status;
    for (i = 0; i < N; i++)
    {
        write_status = write(pipefd[1], exp_names[i], strlen(exp_names[i]) + 1);

        if (write_status == -1)
        {
            perror("write");
        }

        kill(pids[i], SIGUSR1);
        pause();
    }
    close(pipefd[1]);

    //Várakozás a gyerekek befejeződésére
    int child_status;
    for (i = 0; i < N; i++)
    {
        waitpid(pids[i], &child_status, 0);
    }

    //Adatok kiolvasása az üzenetsorból és mentése fájlba
    Observation obs;
    int queue_status;

    FILE* f;
    f = fopen("data.txt","a+");
    if (f == NULL)
    {
        perror("file");
        exit(1);
    }

    for (i = 0; i < N; i++)
    {
        queue_status = msgrcv(queue, &obs, 100, 1, 0);
        if (queue_status < 0)
        {
            perror("msgrcv");
        }
        else
        {
            fprintf(f, "%s\n", obs.mtext);
        }
    }

    printf("adatbazis frissitve\n");

    //Üzenetsor megsemmisítése és a fájl bezárása
    queue_status = msgctl(queue, IPC_RMID, NULL);
    if (queue_status < 0)
    {
        perror("msgctl");
    }
    fclose(f);

    return 0;
}