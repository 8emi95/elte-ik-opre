#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#define SIGERTED SIGRTMIN+1
#define SIGIGEN SIGRTMIN+2
#define SIGNEM SIGRTMIN+3

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
};


int i = 0;
void handler(int signumber){
    i++;
}

int main(int argc, char** argv){
    pid_t child_1;
    pid_t child_2;
    
    int pipefd_1_p[2];
    int pipefd_2_p[2];
    
    if(pipe(pipefd_1_p) == -1){
        perror("cso 1 p");
        exit(1);
    }
    
    
    if(pipe(pipefd_2_p) == -1){
        perror("cso 2 p");
        exit(1);
    }
    
    int uzenetsor;
    uzenetsor = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
    int sh_mem_id;
    int *sh_mem_ptr;
    
    sh_mem_id = shmget(IPC_PRIVATE, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    sh_mem_ptr = shmat(sh_mem_id,NULL,0);
    *sh_mem_ptr = 0;
    
    char* sem_name = "/pjl2e7";
    sem_unlink(sem_name);
    sem_t* semid = sem_open(sem_name, O_CREAT, S_IRUSR|S_IWUSR, 1);
    
    if(uzenetsor < 0){
        perror("msgget");
        return 1;
    }
    
    child_1 = fork();
    if(child_1 < 0){
        perror("gyerek letrehozasa");
        exit(1);
    }
    
    if(child_1 > 0){//parent proc
        pid_t child_2 = fork();
        if(child_2 < 0){
            perror("masodik gyerek letrehozasa");
            exit(1);
        }
        
        if(child_2 > 0){//parent proc
            struct sigaction sigact;
            sigact.sa_handler=handler; 
            sigemptyset(&sigact.sa_mask);     
            sigact.sa_flags=0;
            sigaction(SIGIGEN,&sigact,NULL);
            sigaction(SIGNEM,&sigact,NULL);
        
            close(pipefd_1_p[1]);
            close(pipefd_2_p[1]);
            
            struct uzenet uz;
            uz.mtype = child_1;
            strcpy(uz.mtext, "Hany db otost szeretnel?");
        
            int status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
            if(status <  0){
                perror("msgsnd");
                return 1;
            }            
            
            uz.mtype = child_2;
            status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
            if(status <  0){
                perror("msgsnd");
                return 1;
            }            
            
            int valasz;
            read(pipefd_1_p[0], &valasz, sizeof(valasz));
            printf("1. valasza: %i \n", valasz);
            
            read(pipefd_2_p[0], &valasz, sizeof(valasz));
            printf("2. valasza: %i \n", valasz);
            
            sem_wait(semid);
            
            printf("Nem erti: %i \n", *sh_mem_ptr);
            
            sem_post(semid);
            
            sem_unlink(sem_name);
            
            kill(child_1, SIGERTED);
            kill(child_2, SIGERTED);
            
            while(i <= 1){usleep(100);}
            
            waitpid(-1, NULL, 0);
            
            status = msgctl(uzenetsor, IPC_RMID, NULL);
            if(status < 0){
                perror("msgctl");
                return 1;
            }
            
            fflush(stdout);
            
            shmdt(sh_mem_ptr);
            shmctl(sh_mem_id, IPC_RMID, NULL);
            
            close(pipefd_1_p[0]);
            close(pipefd_2_p[0]);
            
            
        }else{//child_2 proc
            struct sigaction sigact;
            sigact.sa_handler=handler; 
            sigemptyset(&sigact.sa_mask);     
            sigact.sa_flags=0;
            sigaction(SIGERTED,&sigact,NULL);
        
            close(pipefd_2_p[0]);
            close(pipefd_1_p[0]);
            close(pipefd_1_p[1]);
            struct uzenet uz;
            int status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0);
            if(status < 0){
                perror("msgsend");
                return 1;
            }else{
                printf("2 kaptam: %s \n", uz.mtext);
            }
            
            srand(getpid());
            
            int ertem = rand() % 2;
            if(ertem == 0){
                sem_wait(semid);
                *sh_mem_ptr = 2;
                sem_post(semid);
            }
            
            int valasz = rand() % 4 + 1;
            write(pipefd_2_p[1], &valasz, sizeof(valasz));
        
            while(i == 0){usleep(100);}
            ertem = rand() % 2;
            if(ertem == 0){
                kill(getppid(), SIGNEM);
            }else{
                kill(getppid(), SIGIGEN);
            }
            close(pipefd_2_p[1]);
            shmdt(sh_mem_ptr);
            
            exit(0);
        }
    }else{//child_1 proc
        struct sigaction sigact;
        sigact.sa_handler=handler; 
        sigemptyset(&sigact.sa_mask);     
        sigact.sa_flags=0;
        sigaction(SIGERTED,&sigact,NULL);
    
        close(pipefd_1_p[0]);
        close(pipefd_2_p[0]);
        close(pipefd_2_p[1]);
    
        struct uzenet uz;
        int status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0);
        if(status < 0){
            perror("msgsend");
            return 1;
        }else{
            printf("1 kaptam: %s \n", uz.mtext);
        }
        
        srand(getpid());        
        
        int ertem = rand() % 2;
        if(ertem == 0){
            sem_wait(semid);
            *sh_mem_ptr = 1;
            sem_post(semid);
        }
    
        int valasz = rand() % 4 + 1;
        write(pipefd_1_p[1], &valasz, sizeof(valasz));
        
        while(i == 0){usleep(100);}
        ertem = rand() % 2;
        if(ertem == 0){
            kill(getppid(), SIGNEM);
        }else{
            kill(getppid(), SIGIGEN);
        }
        
        close(pipefd_1_p[1]);
        shmdt(sh_mem_ptr);
        
        exit(0);
    }
    
}