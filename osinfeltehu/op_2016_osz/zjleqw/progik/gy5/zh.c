// linekeléshez: -lrt

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <time.h>

int main (int argc, char* argv[])
{
    pid_t child;
    int uzenetsor, status;
    char* mqname="/usor";
    struct mq_attr attr;
    mqd_t mq1, mq2;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 50;

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a cso megnyitasakor!\n");
        exit(EXIT_FAILURE);
    }

    mq_unlink(mqname);
    mq1 = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    if (mq1 < 0)
    {
        printf("Hiba az uzenetsor nyitasakor: %d\n", errno);
        return 1;
    }
    char buffer[100];
    child = fork();
    if (child > 0)
    {
        sleep(3);
		// Kérdés feltevése: Mi van?
        char uzenet[30] = "Mi van?";
        int db = mq_send(mq1, uzenet, strlen(uzenet), 5);
        db = mq_send(mq1, uzenet, strlen(uzenet), 5);
        printf("Szulo: uzenet elkuldve: %d\n", db);
        wait(NULL);
        mq_close(mq1);
        mq_unlink(mqname);

        int answer1, answer2;
        read(pipefd[0], &answer1, sizeof(answer1));
        read(pipefd[0], &answer2, sizeof(answer2));

        printf("Szulo: Gyerek 1 valasza: %d\n", answer1);
        printf("Szulo: Gyerek 2 valasza: %d\n", answer2);

        printf("Szulo folyamat vege!\n");
    }
    else
    {
        // A gyerek is létrehoz egy gyereket

        printf("Gyerek 1 elindult\n");

        int childchild = fork();

        if (childchild > 0)
        {
            // Gyerek 1 kiolvassa a kérdést az üzenetsorból

            int db = mq_receive(mq1, buffer, 54, NULL);
            printf("Gyerek 1: a puffer hossza: %d\n", strlen(buffer));
            printf("Gyerek 1: az olvasott uzenet: %s\n", buffer);

            sleep(2);

            mq_close(mq1);

            // Gyerek 1 válaszol a kérdésre

            srand(getpid());
            int answer = rand() % 4 + 1;
            write(pipefd[1], &answer, sizeof(answer));

            printf("Gyerek 1 valasza: %d\n", answer);

            close(pipefd[0]);
            close(pipefd[1]);
            printf("Gyerek 1 folyamat vege!\n");
        }
        else
        {
            printf("Gyerek 2 elindult!\n");

            // Gyerek 2 kiolvassa a kérdést az üzenetsorból

            int db = mq_receive(mq1, buffer, 54, NULL);
            printf("Gyerek 2: a puffer hossza: %d\n", strlen(buffer));
            printf("Gyerek 2: az olvasott uzenet: %s\n", buffer);

            sleep(2);

            mq_close(mq1);

            // Gyerek 2 válaszol a kérdésre

            srand(getpid());
            int answer = rand() % 4 + 1;
            write(pipefd[1], &answer, sizeof(answer));

            printf("Gyerek 2 valasza: %d\n", answer);

            close(pipefd[0]);
            close(pipefd[1]);

            printf("Gyerek 2 folyamat vege!\n");
        }
        
    }
    return 0;
}