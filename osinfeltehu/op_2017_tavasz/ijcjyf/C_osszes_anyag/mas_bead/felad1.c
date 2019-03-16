#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <wait.h>

int condition = 1;

#define MEMSIZE 4

struct messg {
    long mtype;
    char text[255];
};

void handler1(int sig) {
    printf("Signal %d received\n", sig);
}

void handler2(int sig) {
    condition = 0;
}

int semaphore_create(const char* pathname,int semaphore_value){
    int semid;
    key_t key;
    
    key=ftok(pathname,1);    
    if((semid=semget(key,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,semaphore_value)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}

void semaphore_operation(int semid, int op){

    struct sembuf operation;
    
    operation.sem_num = 0;
    operation.sem_op  = op; // op=1 up, op=-1 down 
    operation.sem_flg = 0;
    
    if(semop(semid,&operation,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void semaphore_delete(int semid){
      semctl(semid,0,IPC_RMID);
}

int send( int mqueue, char* string)
{
    const struct messg m = { 5,string};
    int status;

    status = msgsnd( mqueue, &m, strlen ( m.text ) + 1 , 0 );
    // a 3rd parameter may be : sizeof(m.mtext)
    // a 4th parameter may be  IPC_NOWAIT, equivalent with  0-val
    if ( status < 0 )
        perror("msgsnd error");
    return 0;
}

int receive( int mqueue )
{
    struct messg m;
    ssize_t status;
    // the last parameter is the number of the message
    // if it is 0, then we read the first one
    // if  >0 (e.g. 5), then message type 5 will be read

    status = msgrcv(mqueue, &m, 1024, 5, 0 );

    if ( status < 0 )
        perror("msgsnd error");
    else
        printf("%s\n", m.text );
    return 0;
}


int main() {
    srand(time(NULL));

    signal(SIGUSR1, handler1);

    pid_t child;
    int num1, num2, messg, sh_mem_id,semid;
    int com[2];
    char buffer[255];
    int *countGuess;
    key_t key = ftok("CMakeCache.txt",1);
    key_t key2 = ftok("CMakeLists.txt",1);
    sh_mem_id=shmget(key2,MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    countGuess = shmat(sh_mem_id,NULL,0);
    messg = msgget(key, 0600 | IPC_CREAT);
    if(messg < 0) {
        perror("message queue error");
    }
    num1 = rand() % 100 + 1;
    struct messg m;
    printf("Player 1 thought of number: %d\n", num1);

    if (pipe(com) < 0) {
        perror("pipe error");
    }

    semid = semaphore_create("CMakeLists.txt",1);

    if ((child = fork()) < 0) {
        perror("fork error");
    }

    if (child > 0) { //Player 1
        close(com[1]);
        int answer;

        pause();
        read(com[0], &answer, sizeof(answer));
        printf("Player 2 guessed: %d\n", answer);

        if (answer > num1) {
            strcpy(buffer,"Player 2's guess is too high.\n");
        } else if (answer == num1) {
            strcpy(buffer,"Player 2's guess is equal!\n");
        } else {
            strcpy(buffer,"Player 2's guess is too low.\n");
        }

        m.mtype = 5;
        strcpy(m.text,buffer);
        msgsnd(messg,&m,strlen(m.text)+1,0);
        //semaphore_operation(semid,-1);
        while(read(com[0],&answer,sizeof(int))) {
            printf("Got number: %d\n",answer);
            if(answer == num1) {
                kill(child,SIGUSR2);
                break;
            }
        }
        semaphore_operation(semid,1);

        close(com[0]);
        wait(NULL);
        printf("It took %d guesses\n", countGuess);
        shmdt(countGuess);
        msgctl(messg,IPC_RMID,NULL);
    } else {         //Player 2
        countGuess = 1;
        close(com[0]);
        num2 = rand() % 100 + 1;
        printf("before entering pipe: %d\n", num2);
        kill(getppid(), SIGUSR1); // ready to play
        write(com[1], &num2, sizeof(num2));
        semaphore_operation(semid,-1);
        while(condition) {
            num2 = rand() % 100 + 1;
            countGuess++;
            write(com[1],&num2, sizeof(num2));
        }
        semaphore_operation(semid,1);
    	shmdt(countGuess);
        close(com[1]);
        return(receive(messg));
        //printf("%s",m.text);
    }

    return 0;
}