#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork, pipe
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <signal.h>   //kill
#include <sys/ipc.h>  //key_t
#include <sys/time.h> //time(randhoz)
#include <sys/shm.h>  // osztott memoria
#include <sys/stat.h> //IPC_CREAT | S_IRUSR | S_IWUSR etc
#include <string.h> //strcpy


#define CAN_VOTE 1
#define CANT_VOTE 0

#define RESULT_READY "READY"

typedef struct vote_report
{
    int can_vote;
    int cant_vote;
} Report;

void signal_handler(int signal_id) {}

int main(int argc, char **argv)
{
    key_t shared_memory_token;
    int shared_memory_id;
    char *shared_memory;

    shared_memory_token = ftok(argv[0], 1);
    shared_memory_id = shmget(shared_memory_token, 500, IPC_CREAT | S_IRUSR | S_IWUSR);
    shared_memory = shmat(shared_memory_id, NULL, 0);

    signal(SIGUSR1, signal_handler);
    int pipe__head_to_overseer[2];
    int pipe__overseer_to_stamper[2];
    int pipe__stamper_to_head[2];
    if (
        pipe(pipe__head_to_overseer) == -1 ||
        pipe(pipe__overseer_to_stamper) == -1 ||
        pipe(pipe__stamper_to_head) == -1)
    {
        perror("Cannot open pipe oszt cso"); //basz/6
        exit(EXIT_FAILURE);
    }
    else
    {
        puts("csoo, itt 2 cso!");
    }

    pid_t pid1 = fork();
    if (pid1 > 0)
    {
        pid_t pid2 = fork();
        if (pid2 > 0)
        {
            printf("[Head]: Head of committee is ready\n");
            puts("[Head]: Waiting for my comittee members... ");
            pause();
            pause();
            puts("[Head]: Comittee members are ready... ");
            int number_of_voters = atoi(argv[1]);
            key_t tokens[number_of_voters]; //tokens tipusa: key_t* tokens
            int i;
            for (i = 0; i < number_of_voters; ++i)
            {
                tokens[i] = ftok(argv[0], i);
            }
            close(pipe__head_to_overseer[0]);
            puts("[Head]: Sending IDs to Overseer...");
            write(pipe__head_to_overseer[1], tokens, sizeof(key_t) * number_of_voters);
            close(pipe__head_to_overseer[1]);
            puts("[Head]: Waiting report from stamper...");
            Report r;
            close(pipe__stamper_to_head[1]);
            read(pipe__stamper_to_head[0], &r, sizeof(Report));
            close(pipe__stamper_to_head[0]);

            printf("[Head]: can vote: %d\n", r.can_vote);
            printf("[Head]: can't vote: %d\n", r.cant_vote);

            shmdt(shared_memory);
            wait(NULL); //"a szulo bevarja a gyerekek befejezodeset"
        }
        else
        {
            printf("[Stamper]: Ready\n");
            kill(getppid(), SIGUSR1);
            int number_of_voters;
            close(pipe__overseer_to_stamper[1]);
            puts("[Stamper]: Waiting for votes validity...");
            while(0 != strcmp(shared_memory, RESULT_READY))
            ;
            shmdt(shared_memory);
            read(pipe__overseer_to_stamper[0], &number_of_voters, sizeof(int));
            Report r;
            r.can_vote = 0;
            r.cant_vote = 0;
            int i;
            for (i = 0; i < number_of_voters; ++i)
            {
                int validity;
                read(pipe__overseer_to_stamper[0], &validity, sizeof(int));
                if (validity)
                {
                    ++r.can_vote;
                }
                else
                {
                    ++r.cant_vote;
                }
            }
            printf("[Stamper]: can vote: %d\n", r.can_vote);
            printf("[Stamper]: can't vote: %d\n", r.cant_vote);

            close(pipe__stamper_to_head[0]);
            write(pipe__stamper_to_head[1], &r, sizeof(Report));
            close(pipe__stamper_to_head[1]);
        }
    }
    else
    {
        printf("[Overseer]: Ready\n");
        kill(getppid(), SIGUSR1);
        close(pipe__head_to_overseer[1]);
        int number_of_voters = atoi(argv[1]);
        key_t tokens[number_of_voters];
        read(pipe__head_to_overseer[0], tokens, sizeof(key_t) * number_of_voters);
        close(pipe__head_to_overseer[0]);
        int i;
        srand(time(NULL));
        puts("[Overseer]: IDs received: ");
        close(pipe__overseer_to_stamper[0]);
        write(pipe__overseer_to_stamper[1], &number_of_voters, sizeof(int));
        int can_vote = CAN_VOTE;
        int cant_vote = CANT_VOTE;
        for (i = 0; i < number_of_voters; ++i)
        {
            printf("\t%d\n", tokens[i]);
            if ((rand() % 100) < 20)
            {
                write(pipe__overseer_to_stamper[1], &cant_vote, sizeof(int));
            }
            else
            {
                write(pipe__overseer_to_stamper[1], &can_vote, sizeof(int));
            }
        }
        strcpy(shared_memory, RESULT_READY);
        shmdt(shared_memory);
        puts("[Overseer]: Sent validity data.");
    }

    return 0;
}