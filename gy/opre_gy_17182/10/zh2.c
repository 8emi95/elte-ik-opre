/* ???
szülő - központ
két gyerek - sofőr, autó
konzolba címet
ezt az utas elküldi csővezetéken a központnak
kp kiolvassa, konzolra kiírja hogy feldolg.
kp üzenetet küld autónak
autó kiírja az üzenet tartalmát (üzenet: utas proc_id [pid])
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Pipe open error!");
        return 1;
    }

    int uzenetsor;
    char *mqname = "/uzenetsor";
    struct mq_attr attr;
    mqd_t mq1, mq2;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 50;
    mq_unlink(mqname);
    mq1 = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
    char buffer[100];

    pid_t pid1, pid2;
    pid1 = fork();
    int status;
    if (pid1 == 0) // child1
    {
        printf("Utas\n");

        close(pipefd[0]);
        write(pipefd[1], argv[1], strlen(argv[1]) + 1);
        close(pipefd[1]);
    }
    else
    {
        pid2 = fork();
        int status2;
        if (pid2 == 0) // child2
        {
            sleep(5);
            printf("Auto\n");

            int db = mq_receive(mq1, buffer, 100, NULL);
            printf("Pid: %s\n", buffer);
            mq_close(mq1);
        }
        else // parent
        {
            printf("Kozpont\n");

            waitpid(pid1, &status, 0);

            char hely[200];
            close(pipefd[1]);
            read(pipefd[0], hely, sizeof(hely));
            close(pipefd[0]);
            printf("Hely: %s\n", hely);

            char str[20];
            sprintf(str, "%d", pid1);
            mq_send(mq1, str, 20, 5);
            mq_close(mq1);

            waitpid(pid2, &status, 0);

            mq_unlink(mqname);
            printf("Szulo befejezte!\n");
        }
    }
}