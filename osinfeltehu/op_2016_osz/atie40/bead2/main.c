#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

const char *randomHely()
{
    static const char *helyek[] = {
        "Budapest",
        "Moszkva",
        "Vlagyivosztok"
    };
    return helyek[rand() % 3];
}

int random10()
{
    return ((double)rand() / (double)RAND_MAX) > 0.1;
}

void signal_handler(int signal)
{
}

int main()
{
    const char resztvevok[][80] = {
        "Miklós",
        "Attila",
        "Juli",
        "Peti",
        "Stella"
    };

    srand(time(0));

    int pipefd[2];

    pipe(pipefd);

    const pid_t child = fork();

    if (child > 0) //szülő folyamat
    {
        signal(SIGUSR1, signal_handler);

        const char *hely = randomHely();
        write(pipefd[1], hely, strlen(hely) + 1);

        pause();

        int resztvevok_szama = 1 + rand() % 4;
        write(pipefd[1], &resztvevok_szama, sizeof(resztvevok_szama));

        pause();

        int sikeresseg;
        read(pipefd[0], &sikeresseg, sizeof(sikeresseg));
        printf("Szülő: Sikeresség: %d", sikeresseg);

        waitpid(child, 0, 0);
    }
    else //gyerek folyamat
    {
        srand(time(0));
        const pid_t parent = getppid();

        char hely[80];
        read(pipefd[0], hely, sizeof(hely));
        printf("Gyerek: Elutaztam ide: %s\n", hely);

        sleep(1);
        kill(parent, SIGUSR1);

        int resztvevok_szama;
        read(pipefd[0], &resztvevok_szama, sizeof(resztvevok_szama));

        sleep(1);


        printf("Gyerek: Résztvevők száma: %d\n", resztvevok_szama);

        int *megerkezett_e = malloc(resztvevok_szama * sizeof(int));

        for (int i = 0; i < resztvevok_szama; ++i) {
            printf("Gyerek: Résztvesz a rendezvényen: %s\n", resztvevok[i]);
            megerkezett_e[i] = random10();
            printf("Gyerek: Megérkezett-e: %d\n", megerkezett_e[i]);
        }

        printf("Gyerek: Sikeresség küldése\n");
        int sikeresseg = rand();
        write(pipefd[1], &sikeresseg, sizeof(sikeresseg));
        printf("Gyerek: Vége\n");

        kill(parent, SIGUSR1);
    }

    return 0;
}
