#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

#define MAX 100

struct message { 
     long mtype;
     char mtext[1024]; 
};

void handler(int x) { x = x; }

int main(int argc, char* argv[])
{
    pid_t child,child_b;
    key_t key;
    int msgqueue;
    struct sigaction sigact;

    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGUSR1,&sigact,NULL);

    /*if (argc < 2)
    {
        printf("Nincs megadva a parancssori parameter (osszes szerelvenyszam)");
        exit(1);
    }*/

    key = ftok(argv[0], 1);
    msgqueue = msgget(key, 0600 | IPC_CREAT);
    if (msgqueue < 0)
    {
        perror("msgget");
        exit(1);
    }

    child = fork();
 /*   if (child == 0)
    {
        perror("The fork calling was not succesful\n");
        exit(1);
    }*/

    /* CHILD */
    if (child == 0)
    {

           struct message msg1, msg2;
        int hanyadik; 
        //osszes;
       // int siker;


        srand (time(NULL) ^ (getpid()<<16));

        if (msgrcv(msgqueue, &msg1, sizeof(struct message), 1, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }

        sscanf(msg1.mtext, "%d", &hanyadik);
      

        sleep(1);
       kill(getppid(), SIGUSR1);

        msg2.mtype = 2;
        
        sprintf(msg2.mtext, "%s", "Csak varj amig a kezem koze nem kerulsz");
        if (msgsnd(msgqueue, &msg2, sizeof(struct message), 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }


    }
    else
    {
       child_b = fork();

       if (child_b == 0){
      
       int hanyadik;
        struct message msg;

  

        srand(time(NULL));
        hanyadik = (rand() % 50) - (rand() % 20);
        msg.mtype = 1;
        sprintf(msg.mtext, "%d", hanyadik);
        if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }

        pause();

        if (msgrcv(msgqueue, &msg, sizeof(struct message), 2, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }

        printf("Betoro valasza: '%s' \n", msg.mtext);
          fflush(NULL);
        waitpid(child, 0, 0);
    

    //return 0;
    } else
    {
      pause();
      exit(0);
        //szilo
      
    }
}
return 0;
}

