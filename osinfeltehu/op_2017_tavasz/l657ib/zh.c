#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 512

void handler(int signum) {

}


int main(int argc, char* argv[]) {
    /*if(argc<2) {
        printf("Add meg a termék nevét és árát!\n");
        exit(1);
    }

    char termek[MAX_BUFFER_SIZE];
    int i;
    for(i = 1;i<argc;i++) {
       printf("%s\n",argv[i]);
    }*/

    signal(SIGTERM,handler);
    int pipefd[2];

    pid_t pid;
    pid_t ugynokPid;

    int msgid;
    msgid = msgget((key_t) 2017,IPC_CREATE | 0666);
    struct msgbuf uzenet;

    if(pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitásakor!\n");
        exit(1);
    }

    pid = fork();

    if(pid == -1) {
        perror("Forkolási hiba!\n");
        exit(1);
    }

    if(pid>0) { //vállalat
        
        
        char *uzen = "probauzenet";
        uzenet.mytype = 100;
        strcpy(uzenet.mtext,uzen);
        msgsnd(msgid,&uzenet,strlen(uzen),IPC_NOWAIT);

    } else { //ügynök
        int msgrcv()
    }

    return 0;
}