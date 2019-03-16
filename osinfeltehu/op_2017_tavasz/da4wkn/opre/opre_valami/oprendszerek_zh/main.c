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

struct msgbuf{
    long mtype;
    char q[300];
};

int main(int argc, char *argv[])
{
    printf("SZULO: This is the SZULO process!\n");
    
    time_t t;
    srand(time(&t));

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
    switch(pid1 = fork()) {
    case -1:
        perror("fork"); 
        exit(1); 
    case 0: // // // // // //// // //// // //// // //// // //// // //// // student1
        printf("STUDENT1: This is the STUDENT1 process!\n");
        key_t student1_key;
        int student1_q;
        struct msgbuf student1_buf;
        
        if ((student1_key = ftok("main.c", 'B')) == -1) {
            perror("ftok");
            exit(1);
        }

        if ((student1_q = msgget(student1_key, 0644 | IPC_CREAT)) == -1) {
            perror("msgget");
            exit(1);
        }
        
        if (msgrcv(student1_q, &student1_buf, sizeof(student1_buf.q), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("STUDENT1: Got message -- %s\n", student1_buf.q);
        sleep(1);

        printf("STUDENT1: I'm outta here!\n");
        exit(0);
    default: //parent
        ; int rv;
        switch(pid2 = fork()) {
            case -1:
                perror("fork");  
                exit(1);         
            case 0: // // // //// // //// // //// // //// // //// // // student 2
                printf("STUDENT2: This is the STUDENT2 process!\n");
                // kill(getppid(), SIGUSR2);
                key_t student2_key;
                int student2_q;
                struct msgbuf student2_buf;
                
                if ((student2_key = ftok("main.c", 'B')) == -1) {
                    perror("ftok");
                    exit(1);
                }

                if ((student2_q = msgget(student2_key, 0644 | IPC_CREAT)) == -1) {
                    perror("msgget");
                    exit(1);
                }
                
                if (msgrcv(student2_q, &student2_buf, sizeof(student2_buf.q), 2, 0) == -1) {
                    perror("msgrcv");
                    exit(1);
                }
                printf("STUDENT2: Got message -- %s\n", student2_buf.q);
                sleep(1);


                printf("STUDENT2: I'm outta here!\n");
                exit(0);
            default: //parent
                break;
        }
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
        
        waitpid(pid1, &rv, 0);
        printf("SZULO: STUDENT1's exit status is: %d\n", WEXITSTATUS(rv));
        waitpid(pid2, &rv, 0);
        printf("SZULO: STUDENT2's exit status is: %d\n", WEXITSTATUS(rv));
        
        msgctl(parent_q, IPC_RMID, NULL);
        printf("SZULO: I'm outta here!\n");
}
return 0; }