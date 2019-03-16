//Legyen benne 2 gyerek, jelentkezzenek be
//Beszélgessenek egymással
//Legyen benne egy sima pipe, műdköjön szülőből gyerekbe
//Legyen benne egy üzenetküldés
//Legyen benne egy osztott memória 

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 

#define KEY1 2003
#define KEY2 2004

void handler(int signumber);

struct MESSEGE { 
    long mtype;
    char mtext [ 1024 ]; 
}; 

int main(int argc, char *argv[]){
    signal(SIGTERM,handler);
    pid_t child_a, child_b;

    //Pipe
    char buffer[100];
    int myPipe[2];
    if (pipe(myPipe) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    //Messege
    int msgid = msgget((key_t) KEY1, IPC_CREAT | 0666); 
    if ( msgid < 0 ) { 
        perror("msgget"); 
        return 1; 
     } 
    int status; 

    //Osztott memória
    int oszt_mem_id;
    char *s;
    oszt_mem_id = shmget(KEY2, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat(oszt_mem_id, NULL, 0);

    child_a = fork();
    if(child_a < 0){
        printf("Hiba az elso gyerek megnyitasakor\n");
    }
    if(child_a == 0){
    //[ A GYEREK ]
        sleep(1);
        printf("[ A GYEREK ]: A gyerek itt\n");
        kill(getppid(), SIGTERM);

        /////////////////////////////////////////////////
        pause();
        printf("[ A GYEREK ]: Szia szulo!\n");
        kill(getppid(), SIGTERM);

        /////////////////////////////////////////////////
        pause();
        read(myPipe[0], buffer, sizeof(buffer));
        printf("[ A GYEREK ]: Ezt kaptam a szulotol: %s\n", buffer);
        kill(getppid(), SIGTERM);

        /////////////////////////////////////////////////
        sleep(3);
        printf("[ A GYEREK ]: Ezt olvasom az osztott memoriabol: %s\n", s);
        shmdt(s);
    }else{
        child_b = fork();
        if(child_b < 0){
            printf("Hiba a masodik gyerek megnyitasakor\n");
        }
        if(child_b == 0){
        //[ B GYEREK ]
            sleep(2);
            printf("[ B GYEREK ]: B gyerek itt\n");
            kill(getppid(), SIGTERM);

            /////////////////////////////////////////////////
            pause();
            printf("[ B GYEREK ]: Szia szulo!\n");
            kill(getppid(), SIGTERM);

            /////////////////////////////////////////////////
            pause();
            read(myPipe[0], buffer, sizeof(buffer));
            printf("[ B GYEREK ]: Ezt kaptam a szulotol: %s\n", buffer);
            kill(getppid(), SIGTERM);

            /////////////////////////////////////////////////
            struct MESSEGE uz; 
            status = msgrcv(msgid, &uz, 1024, 5, 0 ); 
            if ( status < 0 ) 
                perror("msgsnd"); 
            else
                printf("[ B GYEREK ]: A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
        }else{
        //[   SZULO  ]
            printf("[   SZULO  ]: Varok a gyerekekre\n");
            pause();
            pause();
            printf("[   SZULO  ]: Gyerekek megjottek\n\n");

            /////////////////////////////////////////////////
            printf("[   SZULO  ]: Hello A gyerek\n");
            kill(child_a, SIGTERM);
            pause();

            printf("[   SZULO  ]: Hello B gyerek\n");
            kill(child_b, SIGTERM);
            pause();

            printf("[   SZULO  ]: Kommunikiacio megvolt\n\n");

            /////////////////////////////////////////////////
            printf("[   SZULO  ]: Kuldok nekik csovon valamit\n");
            
            write(myPipe[1], "Almafa", sizeof("Almafa"));
            kill(child_a, SIGTERM);
            pause();

            write(myPipe[1], "Barackfa", sizeof("Barackfa"));
            kill(child_b, SIGTERM);
            pause();

            printf("[   SZULO  ]: Pipe megvolt\n\n");

            /////////////////////////////////////////////////   
            const struct MESSEGE uz = { 5, "Hajra Fradi!" }; 
            status = msgsnd( msgid, &uz, strlen(uz.mtext) + 1 , 0 ); 
            if ( status < 0 ) 
                perror("msgsnd"); 
            sleep(1);
            printf("[   SZULO  ]: Uzenet is megvolt\n\n");

            /////////////////////////////////////////////////                  
            sleep(2);
            char* szoveg = "Kalap-kabat";
            strcpy(s, szoveg);
            shmdt(s); 
            shmctl(oszt_mem_id, IPC_RMID, NULL);
            printf("[   SZULO  ]: Osztott memoria is megvolt\n\n");
            wait(NULL);
        }
    }
    return 0;
}

void handler(int signumber){
    //printf("Megkaptam a %d szamu signalt", signumber);
}