#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#define PROC_NUMBER 2
#define BUFFER_SIZE 1024
#define SHARED_MEMORY_SIZE 10240
#define MESSAGE_QUEUE_PATH "./queue"
#define SHARED_MEMORY_PATH "./shared_memory"
#define QUESTION "Who is Negan?"
#define UNDERSTAND "Negan is awesome!\n"
#define NOT_UNDERSTAND "Need to shut that shit down...No exceptions\n"


int answerPipe[2];

struct MessageStruct
{
    long id;
    char text[BUFFER_SIZE];
};
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
void childProcess()
{
    struct MessageStruct messageStruct;
    key_t queueKey = ftok(MESSAGE_QUEUE_PATH, 'b');
    int queueID = msgget(queueKey, 0666);
    int status = msgrcv(queueID, &messageStruct, BUFFER_SIZE, 0, 0);
    if(status < 0)
    {
        error("Error recv from queue");
    }

    //2
    printf("Message received from message queue: %s\n", messageStruct.text);
    sleep(2);

    //3
    srand(getpid());
    int number = (rand() % 4) + 1;
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "%d", number);

    close(answerPipe[0]);
    write(answerPipe[1], buffer, BUFFER_SIZE);
    close(answerPipe[1]);

    //4
    key_t shmKey = ftok(SHARED_MEMORY_PATH, 'R');
    int shmID = shmget(shmKey, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT);
    char *shmData = shmat(shmID, (void *)0, 0);
    if(shmData == (char *)-1)
    {
        error("Shared memory error");
    }
    srand(getpid());
    int understandNumber = (rand() % 10) + 1;
    struct sembuf sb = {0, -1, 0};
    sb.sem_op = -1; //Lock
    semop(shmID,(struct sembuf *)&sb, 1);
    if(understandNumber < 5)
    {
        strncpy(shmData, UNDERSTAND, SHARED_MEMORY_SIZE);
    }
    else
    {
        strncpy(shmData, NOT_UNDERSTAND, SHARED_MEMORY_SIZE);
    }
    sb.sem_op = 1; //Unlock
    semop(shmID,(struct sembuf *)&sb, 1);


    kill(getpid(), SIGKILL);
}
int main()
{
    pid_t pidArray[PROC_NUMBER];
    int n = PROC_NUMBER;
    int i;
    char buffer[BUFFER_SIZE];

    //Init message queue
    key_t queueKey = ftok(MESSAGE_QUEUE_PATH, 'b');
    int queueID = msgget(queueKey, 0666 | IPC_CREAT);

    //Init pipe
    if(pipe(answerPipe) == -1)
    {
        error("Pipe init error");
    }

    //Init shared memory
    key_t shmKey = ftok(SHARED_MEMORY_PATH, 'R');
    int shmID = shmget(shmKey, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT);
    char *shmData = shmat(shmID, (void *)0, 0);
    if(shmData == (char *)-1)
    {
        error("Shared memory error");
    }
    struct sembuf sb = {0, -1, 0};

    //Send messages to the queue
    struct MessageStruct messageStruct[PROC_NUMBER];
    for(i = 0; i < n; ++i)
    {
        messageStruct[i].id = i + 1;
        strcpy(messageStruct[i].text, QUESTION);
        struct MessageStruct temp = messageStruct[i];
        int qStatus = msgsnd(queueID, &temp, BUFFER_SIZE, 0);

        if(qStatus < 0)
        {
            error("Sending queue error.");
        }
    }
    //Init processes
    for(i = 0; i < n; ++i)
    {
        pidArray[i] = fork();
        if(pidArray[i] < 0)
        {
            error("Fork error\n");
        }
        else if(pidArray[i] == 0)
        {
            childProcess();
        }
    }

    //Wait for the children to finish
    int status;
    pid_t  pid;
    while(n > 0)
    {
        pid = wait(&status);

        //3
        bzero(buffer, BUFFER_SIZE);
        close(answerPipe[1]);
        int readStatus;
        while ((readStatus = read(answerPipe[0], buffer, BUFFER_SIZE) > 0))
        {
            printf("Number received from child: %s\n", buffer);
        }
        if (readStatus < 0)
        {
            perror("read from pipe");
        }

        //4
        sb.sem_op = -1; //Lock
        semop(shmID,(struct sembuf *)&sb, 1);
        printf("Printing shared memory content:\n");
        printf("%s", shmData);
        sb.sem_op = 1; //Unlock
        semop(shmID,(struct sembuf *)&sb, 1);


        printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
        --n;
    }

    if (shmdt(shmData) == -1)
    {
        error("shmdt");
    }

    return 0;
}