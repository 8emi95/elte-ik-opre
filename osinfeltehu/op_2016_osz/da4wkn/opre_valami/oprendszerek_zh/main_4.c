#define _POSIX_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <string.h>


#define THE_SIZE 1024

struct msgbuf{
    long mtype;
    char q[300];
};

int main(int argc, char *argv[])
{
    printf("SZULO: This is the SZULO process!\n");
    
    struct msgbuf parent_buf;
    int parent_q;
    key_t parent_key;

    if ((parent_key = ftok("main.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((parent_q = msgget(parent_key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    pid_t pid1, pid2;
    int pfds[2]; // 0: read end, 1: write end
    pipe(pfds);
    
    int p_shmid;

    
    
    switch(pid1 = fork()) {
    case -1:
        perror("fork"); 
        exit(1); 
    case 0: // // // // // //// // //// // //// // //// // //// // //// // student1
        printf("STUDENT1: This is the STUDENT1 process!\n");
        close(pfds[0]);        
        key_t student1_key;
        int student1_q;
        struct msgbuf student1_buf;
        
        if ((student1_key = ftok("main.c", 'B')) == -1) {
            perror("ftok");
            exit(1);
        }

        if ((student1_q = msgget(student1_key, 0644)) == -1) {
            perror("msgget");
            exit(1);
        }
        
        if (msgrcv(student1_q, &student1_buf, sizeof(student1_buf.q), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("STUDENT1: Got message -- %s\n", student1_buf.q);
        sleep(1);
        
        // send answer
        srand(getpid());        
        int student_answer = rand() % 4 + 1;
        write(pfds[1], &student_answer, sizeof(int));
        printf("STUDENT1: I'm outta here!\n");
        exit(0);
        close(pfds[1]);        
    default: //parent
        ; int rv;
        switch(pid2 = fork()) {
            case -1:
                perror("fork");  
                exit(1);         
            case 0: // // // //// // //// // //// // //// // //// // // student 2
                close(pfds[0]);
                printf("STUDENT2: This is the STUDENT2 process!\n");
                // kill(getppid(), SIGUSR2);
                key_t student2_key;
                int student2_q;
                struct msgbuf student2_buf;
                
                if ((student2_key = ftok("main.c", 'B')) == -1) {
                    perror("ftok");
                    exit(1);
                }

                if ((student2_q = msgget(student2_key, 0644)) == -1) {
                    perror("msgget");
                    exit(1);
                }
                
                if (msgrcv(student2_q, &student2_buf, sizeof(student2_buf.q), 2, 0) == -1) {
                    perror("msgrcv");
                    exit(1);
                }
                printf("STUDENT2: Got message -- %s\n", student2_buf.q);
                sleep(1);
                time_t t;
                srand(getpid());
                int student2_answer = rand() % 4 + 1;
                write(pfds[1], &student2_answer, sizeof(int));
        
                // write to shared memory
                /* make the key: */
                if ((student2_key = ftok("main_4.c", 'S')) == -1) {
                    perror("ftok");
                    exit(1);
                }

                /* connect to (and possibly create) the segment: */
                int s2_shmid;
                if ((s2_shmid = shmget(student2_key, THE_SIZE, 0644 | IPC_CREAT)) == -1) {
                    perror("shmget, s2");
                    exit(1);
                }
                char* student2_data = shmat(s2_shmid, (void *)0, 0);
                if (student2_data == (char *)(-1))
                    perror("shmat, s2");
                printf("STUDENT2: printing %d\n", *student2_data);
                printf("STUDENT2: I'm outta here!\n");
                close(pfds[1]);                
                exit(0);
            default: //parent
                break;
        }        
        close(pfds[1]);
        // send message
        parent_buf.mtype = 1;
        strcpy(parent_buf.q, "Itt vagytok mind?");
        if (msgsnd(parent_q, &parent_buf, strlen(parent_buf.q) + 1, 0) == -1){ /* +1 for '\0' */
            perror("msgsnd");
        }
        parent_buf.mtype = 2;
        if (msgsnd(parent_q, &parent_buf, strlen(parent_buf.q) + 1, 0) == -1){ /* +1 for '\0' */
            perror("msgsnd");
        }
        
        int parent_answer = 0;
        read(pfds[0], &parent_answer, sizeof(int));
        printf("SZULO: Got answer: %d\n", parent_answer);
        read(pfds[0], &parent_answer, sizeof(int));
        printf("SZULO: Got answer: %d\n", parent_answer);
        
        waitpid(pid1, &rv, 0);

        parent_key = ftok("main_4.c", 'S');
        p_shmid = shmget(parent_key, THE_SIZE, 0644 | IPC_CREAT);
        char* parent_data = shmat(p_shmid, (void *)0, 0);
        if (parent_data == (char *)(-1))
            perror("shmat, parent");
        
        printf("Data in shared memory: %s", *parent_data);        
        printf("SZULO: STUDENT1's exit status is: %d\n", WEXITSTATUS(rv));
        waitpid(pid2, &rv, 0);
        printf("SZULO: STUDENT2's exit status is: %d\n", WEXITSTATUS(rv));
        
        msgctl(parent_q, IPC_RMID, NULL);
        printf("SZULO: I'm outta here!\n");
        close(pfds[0]);
}
return 0; }