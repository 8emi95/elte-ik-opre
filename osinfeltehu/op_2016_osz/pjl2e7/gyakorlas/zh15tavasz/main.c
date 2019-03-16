#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <wait.h>
#include <sys/stat.h>

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
};

volatile int i = 0;
volatile int kezdek = 0;
volatile int vege = 0;
void handler(int signumber){
    printf("%i \n", getpid());
    i++;
}

void child_1_handler(int signum){
    printf("1-es: en kezdek\n");
    i++;
    kezdek++;
}

void child_2_handler(int signum){
    printf("2-es: en kezdek\n");
    i++;
    kezdek++;
}

void child_sigrt(int signum){
    vege++;
};

int main(int argc, char** argv){
    struct sigaction sigact;
    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);  
    sigact.sa_flags=0;
    sigaction(SIGRTMIN,&sigact,NULL);
    
    int pipefd_1_2[2];
    int pipefd_2_1[2];
    
    int idok[2] = {0,0};
    
    int oszt_mem_id;
    int* oszt_mem_ptr;
    
    oszt_mem_id = shmget(IPC_PRIVATE, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    oszt_mem_ptr = shmat(oszt_mem_id, NULL, 0);
    
    
    int uzenetsor;
    uzenetsor = msgget(IPC_PRIVATE, 0600 |IPC_CREAT);
    
    if(uzenetsor < 0){
        perror("msgget");
        return 1;
    }
    
    
    if(pipe(pipefd_1_2) == -1){
        perror("cso 1_2");
        exit(1);
    }
    
    if(pipe(pipefd_2_1) == -1){
        perror("cso 2_1");
        exit(1);
    }
    
    pid_t child_1 = fork();
    if(child_1 < 0){
        perror("gyerek letrehozasa");
        exit(0);
    }
    
    if(child_1 > 0){//parent process
        pid_t child_2 = fork();
        if(child_2 < 0){
            perror("masodik gyerek letrehozasa");
            exit(0);
        }
        
        if(child_2 > 0){//parent process
            while(i <= 1){usleep(100);}
            srand(getpid());
            int kezdo = rand() % 2 + 1;
            printf("Kezd: %i \n", kezdo);
            if(kezdo == 1){
                kill(child_1, SIGTERM);
                kill(child_2, SIGUSR1);
            }else{
                kill(child_2, SIGTERM);
                kill(child_1, SIGUSR1);
            }
            
            int j;
            for(j = 0; j < 2 * atoi(argv[1]); j++){
                struct uzenet uz;
                int status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0);
                if(status < 0){
                    perror("msgsend");
                    return 1;
                }else{
                    if(j % 2 == 0){
                        idok[kezdo-1] += atoi(uz.mtext);
                        printf("%i. jatekos ido: %s \n", kezdo, uz.mtext);
                    }else{
                        idok[2 - kezdo] += atoi(uz.mtext);
                        printf("%i. jatekos ido: %s \n", 2 - kezdo + 1, uz.mtext);                        
                    }
                }
            }
            
            if(idok[0] < idok[1]){
                printf("Nyertes: 1 \n");
                *oszt_mem_ptr = child_1;
            }else if(idok[1] < idok[0]){
                printf("Nyertes: 2 \n");
                *oszt_mem_ptr = child_2;
            }else{
                printf("Dontetlen\n");
                *oszt_mem_ptr = -1;
            }
            kill(child_1, SIGRTMIN);
            kill(child_2, SIGRTMIN);
            
            close(pipefd_1_2[0]);
            close(pipefd_1_2[1]);
            close(pipefd_2_1[0]);
            close(pipefd_2_1[1]);
            fflush(stdout);
            
            int status = msgctl(uzenetsor, IPC_RMID, NULL);
            if(status < 0){
                perror("msgctl");
                return 1;
            }
            waitpid(-1, NULL, 0);
        }else
        if(child_2 == 0){//child_2 process         
            struct sigaction sigact1;
            sigact1.sa_handler=child_2_handler;
            sigemptyset(&sigact1.sa_mask);  
            sigact1.sa_flags=0;
            sigaction(SIGTERM,&sigact1,NULL);
            
            struct sigaction sigact2;
            sigact2.sa_handler=handler;
            sigemptyset(&sigact2.sa_mask);  
            sigact2.sa_flags=0;
            sigaction(SIGUSR1,&sigact2,NULL);
            
            struct sigaction sigact3;
            sigact3.sa_handler=child_sigrt;
            sigemptyset(&sigact3.sa_mask);  
            sigact3.sa_flags=0;
            sigaction(SIGRTMIN,&sigact3,NULL);
            
            
            
            if(kill(getppid(), SIGRTMIN) == -1){
                perror("kill");
                exit(1);
            }; 
            while(i <= 0);        
            close(pipefd_1_2[1]);
            close(pipefd_2_1[0]);              
                          
            srand(getpid());
                          
            if(kezdek){
                int j;
                for(j = 0; j < atoi(argv[1]); j++){
                    int sleeptime = rand()%2 + 2;
                    sleep(sleeptime);
                    int c1 = rand()%8;
                    int c2 = rand()%8;
                    int c3 = rand()%8;
                    int c4 = rand()%8;
                    
                    char msg[4];
                    msg[0] = 65 + c1;
                    msg[1] = 48 + c2 + 1;
                    msg[2] = 65 + c3;
                    msg[3] = 48 + c4 + 1;
                    
                    struct uzenet uz;
                    uz.mtype = getppid();
                    
                    char buffer[10];
                    snprintf(buffer, 10, "%i", sleeptime);
                    
                    strcpy(uz.mtext, buffer);
                    
                    int status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
                    
                    if(status < 0){
                        perror("msgsnd");
                        return 1;
                    }
                    
                    printf("2 kuldi: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                    
                    write(pipefd_2_1[1], msg, sizeof(msg));
                    
                    read(pipefd_1_2[0], msg, sizeof(msg));
                    printf("2 fogadja: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                }
                
            }else{
                int j;
                for(j = 0; j < atoi(argv[1]); j++){
                    char msg[4];
                    read(pipefd_1_2[0], msg, sizeof(msg));
                    printf("2 fogadja: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                    
                    int sleeptime = rand()%2 + 2;
                    sleep(sleeptime);
                    int c1 = rand()%8;
                    int c2 = rand()%8;
                    int c3 = rand()%8;
                    int c4 = rand()%8;
                    
                    msg[0] = 65 + c1;
                    msg[1] = 48 + c2 + 1;
                    msg[2] = 65 + c3;
                    msg[3] = 48 + c4 + 1;
                    
                    struct uzenet uz;
                    uz.mtype = getppid();
                    
                    char buffer[10];
                    snprintf(buffer, 10, "%i", sleeptime);
                    
                    strcpy(uz.mtext, buffer);
                    
                    int status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
                    
                    printf("2 kuldi: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                    
                    write(pipefd_2_1[1], msg, sizeof(msg));
                }
            }         
            while(vege <= 0){usleep(100);};
            if(*oszt_mem_ptr == getpid()){
                printf("2: nyertem \n");
            }else{
                printf("2: nem nyertem \n");
            }
                          
            close(pipefd_1_2[0]);
            close(pipefd_2_1[1]);              
            fflush(stdout);
            
            exit(0);
        }
    }else
    if(child_1 == 0){//child_1 process
        struct sigaction sigact1;
        sigact1.sa_handler=child_1_handler;
        sigemptyset(&sigact1.sa_mask);  
        sigact1.sa_flags=0;
        sigaction(SIGTERM,&sigact1,NULL);
        
        struct sigaction sigact2;
        sigact2.sa_handler=handler;
        sigemptyset(&sigact2.sa_mask);  
        sigact2.sa_flags=0;
        sigaction(SIGUSR1,&sigact2,NULL);
        
                
        struct sigaction sigact3;
        sigact3.sa_handler=child_sigrt;
        sigemptyset(&sigact3.sa_mask);  
        sigact3.sa_flags=0;
        sigaction(SIGRTMIN,&sigact3,NULL);
        
        if(kill(getppid(), SIGRTMIN) == -1){
            perror("kill");
            exit(1);
        };
        while(i <= 0);    
        
        close(pipefd_1_2[0]);
        close(pipefd_2_1[1]);              
                        
        srand(getpid());
                        
        if(kezdek){
            int j;
            for(j = 0; j < atoi(argv[1]);j++){
                int sleeptime = rand()%2 + 2;
                
                sleep(sleeptime);            
                int c1 = rand()%8;
                int c2 = rand()%8;
                int c3 = rand()%8;
                int c4 = rand()%8;
                
                char msg[4];
                msg[0] = 65 + c1;
                msg[1] = 48 + c2 + 1;
                msg[2] = 65 + c3;
                msg[3] = 48 + c4 + 1;
                
                
                struct uzenet uz;
                uz.mtype = getppid();
                
                char buffer[10];
                snprintf(buffer, 10, "%i", sleeptime);
                
                strcpy(uz.mtext, buffer);
                
                int status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
                
                if(status < 0){
                    perror("msgsnd");
                    return 1;
                }
                
                
                printf("1 kuldi: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                write(pipefd_1_2[1], msg, sizeof(msg));
                
                read(pipefd_2_1[0], msg, sizeof(msg));
                printf("1 fogadja: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
            }
        }else{
            int j;
            for(j = 0; j < atoi(argv[1]); j++){
                char msg[4];
                read(pipefd_2_1[0], msg, sizeof(msg));
                printf("1 fogadja: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                
                int sleeptime = rand()%2 + 2;
                sleep(sleeptime);            
                
                int c1 = rand()%8;
                int c2 = rand()%8;
                int c3 = rand()%8;
                int c4 = rand()%8;
                
                msg[0] = 65 + c1;
                msg[1] = 48 + c2 + 1;
                msg[2] = 65 + c3;
                msg[3] = 48 + c4 + 1;
                
                struct uzenet uz;
                uz.mtype = getppid();
                
                char buffer[10];
                snprintf(buffer, 10, "%i", sleeptime);
                
                strcpy(uz.mtext, buffer);
                
                int status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
                
                printf("1 kuldi: %c%c -> %c%c \n", msg[0], msg[1], msg[2], msg[3]);
                write(pipefd_1_2[1], msg, sizeof(msg));
            }
        }     
        
        while(vege <= 0){usleep(100);};
        if(*oszt_mem_ptr == getpid()){
            printf("1: nyertem \n");
        }else{
            printf("1: nem nyertem \n");
        }
                        
        close(pipefd_1_2[1]);
        close(pipefd_2_1[0]); 
        fflush(stdout);
                                  
        exit(0);
    }
}
