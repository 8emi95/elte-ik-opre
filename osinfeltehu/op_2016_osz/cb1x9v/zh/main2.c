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
//#include <semaphore.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 1
#define PROC_NUMBER 1
#define BUFFER_SIZE 256
#define SHARED_MEMORY_SIZE 10240
#define MESSAGE_QUEUE_PATH "./queue"
#define SHARED_MEMORY_PATH "./shared_memory"

int trains[PROC_NUMBER];

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
void initTrainSignal(int sign)
{
    printf("Signal received (init train): %d\n", sign);
    signal(SIGUSR1, initTrainSignal);
}
void startTrainSignal(int sign)
{
    printf("Signal received (start train): %d\n", sign);
    signal(SIGUSR1, startTrainSignal);
}
void initTrain(int id, pid_t parent)
{
    printf("%d started\n", id);
    char buffer[BUFFER_SIZE];
    struct MessageStruct initMessage;
    struct MessageStruct maxTrainMessage;
    key_t queueKey = ftok(MESSAGE_QUEUE_PATH, 'b');
    int queueID = msgget(queueKey, 0666);
    int status = msgrcv(queueID, &initMessage, BUFFER_SIZE, 0, 0);
    if(status < 0)
    {
        error("Error recv from queue");
    }
    status = msgrcv(queueID, &maxTrainMessage, BUFFER_SIZE, 0, 0);
    if(status < 0)
    {
        error("Error recv from queue");
    }


    printf("Message received from message queue:\n");
    printf("Train number: %s\n", initMessage.text);
    printf("Max train number: %s\n", maxTrainMessage.text);
    srand(getpid());
    int random = rand() % 10;


    struct MessageStruct resultMessage;
    resultMessage.id = 2;
    if(random < 5)
    {
        trains[id] = 1;
        strcpy(buffer, "Train arrived");
    }
    else
    {
        trains[id] = 0;
        strcpy(buffer, "Train not arrived because something gave it banana");

    }
    strcpy(resultMessage.text, buffer);

    int qStatus = msgsnd(queueID, &resultMessage, BUFFER_SIZE, 0);

    if(qStatus < 0)
    {
        error("Sending queue error child");
    }

    kill(parent, SIGUSR1);
    kill(getpid(), SIGKILL);
    //exit(0);
}
void sendReplacementTrain(int id, pid_t parent)
{
    printf("Sending replacement: %d\n", id);
    srand(getpid());
    int random = rand() % 10;
    if(random < 5)
    {
        printf("Train arrived");
    }
    else
    {
        printf("Train not arrived because something gave it banana");

    }
}
int main(int argc, char *argv[])
{
    signal(SIGUSR1, initTrainSignal);
    signal(SIGUSR2, startTrainSignal);

    pid_t pidArray[PROC_NUMBER];
    int n = PROC_NUMBER;
    int i;
    char buffer[BUFFER_SIZE];
    int maxTrainNumber = 10;
    //int maxTrainNumber = atoi(argv[0]);

    //Init message queue
    key_t queueKey = ftok(MESSAGE_QUEUE_PATH, 'b');
    int queueID = msgget(queueKey, 0666 | IPC_CREAT);


    //Send message
    struct MessageStruct initMessage[PROC_NUMBER];
    struct MessageStruct maxTrainMessage[PROC_NUMBER];
    for(i = 0; i < n; ++i)
    {
        srand(time(NULL));
        initMessage[i].id = i + 1;
        maxTrainMessage[i].id = i+2;
        int trainNumber = rand() % 100;
        snprintf(buffer, BUFFER_SIZE, "%d", trainNumber);
        snprintf(maxTrainMessage[i].text, BUFFER_SIZE, "%d", maxTrainNumber);
        strcpy(initMessage[i].text, buffer);
        bzero(buffer, BUFFER_SIZE);
        struct MessageStruct temp = initMessage[i];
        int qStatus = msgsnd(queueID, &temp, BUFFER_SIZE, 0);

        if(qStatus < 0)
        {
            error("Sending queue error, initmessage");
        }
        qStatus = msgsnd(queueID, &maxTrainMessage[i], BUFFER_SIZE, 0);

        if(qStatus < 0)
        {
            error("Sending queue error maxtrainMessage");
        }
    }


    //Init shared memory
    key_t shmKey = ftok(SHARED_MEMORY_PATH, 'R');
    int shmID = shmget(shmKey, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT);
    void *shmData = shmat(shmID, (void *)0, 0);
    if(shmData == (char *)-1)
    {
        error("Shared memory error");
    }

    for(i = 0; i < n; ++i)
    {
        pidArray[i] = fork();
        pid_t parent = getpid();
        if(pidArray[i] < 0)
        {
            error("Fork error");
        }
        else if(pidArray[i] == 0)
        {
            initTrain(i,parent);
        }
    }
    int status;
    pid_t pid;
    int procCounter = 0;
    int replacementNumber = 0;
    while(n > 0)
    {
        kill(pidArray[procCounter], SIGUSR2);
        struct MessageStruct resultMessage;
        pid = wait(&status);
        printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);

        int qStatus = msgrcv(queueID, &resultMessage, BUFFER_SIZE, 0, 0);
        if(qStatus < 0)
        {
            error("Error recv from queue");
        }
        printf("Message received from child: %s\n", resultMessage.text);

        ++procCounter;
        --n;
    }
    return 0;
};
