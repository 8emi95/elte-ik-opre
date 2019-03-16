#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <wait.h>

struct utazas{
    char cel[30];
    int ferohely; 
};

int i = 0;
void handler(int signumber){

    printf("%i \n", getpid());
    i++;
}

int main(int argc, char** argv){
    char* sem_nev = "/PJL2E7";
    
    struct utazas utazasok[3];
    
    strcpy(utazasok[0].cel, "Kenya");
    utazasok[0].ferohely = 10;

    strcpy(utazasok[1].cel, "Tanzania");    
    utazasok[1].ferohely = 10;
    
    strcpy(utazasok[2].cel, "Zanzibar");    
    utazasok[2].ferohely = 10;

    sem_t* semid;
    semid = sem_open(sem_nev, O_CREAT,S_IRUSR|S_IWUSR,0);
    if(semid == SEM_FAILED){
        perror("sem_open");
        exit(1);
    };
    
    struct sigaction sigact;
    sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
    sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
    //now only the arriving signal, SIGTERM will be blocked
    sigact.sa_flags=0; //nothing special behaviour
    sigaction(SIGTERM,&sigact,NULL);
    
    int oszt_mem_id = shmget(IPC_PRIVATE, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    struct utazas* oszt_mem_ptr;
    oszt_mem_ptr = shmat(oszt_mem_id, NULL, 0);
    
    int pipefd_ozd[2];
    if(pipe(pipefd_ozd) == -1){
        perror("pipe ozd nyitas");
        exit(0);
    }
    
    int pipefd_komlo[2];
    if(pipe(pipefd_komlo) == -1){
        perror("pipe komlo nyitas");
        exit(0);
    }
    
    pid_t child_ozd = fork();
    if(child_ozd < 0){
        perror("gyerek letrehozasa");
        exit(0);
    }
    
    if(child_ozd > 0){//parent process
        pid_t child_komlo = fork();
        if(child_komlo < 0){
            perror("masodik gyerek letrehozasa");
            exit(0);
        }
        
        if(child_komlo > 0){//parent process
            while(i <= 1){
                usleep(100);
            }
            //close(pipefd_ozd[0]);//reading end
            //close(pipefd_komlo[0]);//reading end
            
            memcpy(oszt_mem_ptr, utazasok, sizeof(utazasok));
            
            sem_post(semid);
            time_t t_1 = time(0);
            struct tm t_2 = *localtime(&t_1);
            t_2.tm_mday += 1;
            char buff[20];
            strftime(buff, 20, "%Y:%m:%d\0", &t_2);
            write(pipefd_ozd[1], buff, sizeof(buff));
            
            while(i <= 2)usleep(100);
            
            
            write(pipefd_komlo[1], buff, sizeof(buff));
            
            while(i <= 3)usleep(100);
            
            int ozd_jelentkezes, komlo_jelentkezes;

            if(read(pipefd_komlo[0], &komlo_jelentkezes, sizeof(komlo_jelentkezes)) > 0){
                if(komlo_jelentkezes >= 0){
                    printf("Komlon erre jelentkezett: %s \n", oszt_mem_ptr[komlo_jelentkezes].cel);
                    char valasz[20];
                                               
                    sem_wait(semid);
                    if(oszt_mem_ptr[komlo_jelentkezes].ferohely > 0){
                        oszt_mem_ptr[komlo_jelentkezes].ferohely--;
                        strcpy(valasz, "sikeres");
                    }else{
                        strcpy(valasz, "sikertelen");
                    }
                    sem_post(semid);
                    printf("%i\n", write(pipefd_komlo[1], valasz, sizeof(valasz)));
                    kill(child_komlo, SIGTERM);
                    
                }else{
                    char valasz[20];
                    strcpy(valasz, "sikertelen");
                    write(pipefd_komlo[1], valasz, sizeof(valasz));
                    kill(child_komlo, SIGTERM);
                }
            }else{
                printf("komlo keves\n");
            }
            
            if(read(pipefd_ozd[0], &ozd_jelentkezes, sizeof(ozd_jelentkezes)) > 0){
                if(ozd_jelentkezes >= 0){
                    printf("Ozdon erre jelentkezett: %s \n", oszt_mem_ptr[ozd_jelentkezes].cel);                                
                   // sem_wait(semid);
                    char valasz[20];                    
                    if(oszt_mem_ptr[ozd_jelentkezes].ferohely > 0){
                        oszt_mem_ptr[ozd_jelentkezes].ferohely--;
                        strcpy(valasz, "sikeres");
                    }else{
                        strcpy(valasz, "sikertelen");
                    }
                 //   sem_post(semid);
                    write(pipefd_ozd[1], valasz, sizeof(valasz));
                    kill(child_ozd, SIGTERM);
                    
                }else{
                    char valasz[20];
                    strcpy(valasz, "sikertelen");
                    write(pipefd_ozd[1], valasz, sizeof(valasz));
                    kill(child_ozd, SIGTERM);
                    
                }
            }else{
                printf("ozd keves\n");
            }

            
            
            close(pipefd_ozd[1]);
            close(pipefd_komlo[1]);
            close(pipefd_ozd[0]);
            close(pipefd_komlo[0]);
            
            printf("szulo vege\n");
            waitpid(child_ozd, NULL, 0);
            waitpid(child_komlo, NULL, 0);
            
            shmdt(oszt_mem_ptr);
            
            shmctl(oszt_mem_id, IPC_RMID, NULL);
            
            sem_unlink(sem_nev);
            fflush(stdout);
        }else
        if(child_komlo == 0){//child_komlo process
            //close(pipefd_komlo[1]);//writing end
        printf("%i\n", i);
            
            close(pipefd_ozd[0]);
            close(pipefd_ozd[1]);
            kill(getppid(), SIGTERM);
            
            char buff[20];
            read(pipefd_komlo[0], buff, sizeof(buff));
            
            printf("Komlo nyitas: %s \n", buff);

            srand(getpid());
            int waittime = rand() % 2 + 2;
            //sleep(waittime);
            
            int valasztott_ut = rand() % 3;
         //   sem_wait(semid);
            
            printf("Komlo: %s \n", oszt_mem_ptr[valasztott_ut].cel);
            if(oszt_mem_ptr[valasztott_ut].ferohely > 0){
                write(pipefd_komlo[1], &valasztott_ut, sizeof(valasztott_ut));
            }else{
                valasztott_ut = -1;
                write(pipefd_komlo[1], &valasztott_ut, sizeof(valasztott_ut));                
            }

     //       sem_post(semid);
            kill(getppid(), SIGTERM);
            
            while(i <= 0)usleep(100);
            
            char valasz[20];
            read(pipefd_komlo[0], valasz, sizeof(valasz));
            printf("Komlo: %s jelentkezes\n", valasz);
            
            close(pipefd_komlo[1]);
            close(pipefd_komlo[0]);
            
            fflush(stdout);
            shmdt(oszt_mem_ptr);
            
            
            exit(0);
        }
    }else
    if(child_ozd == 0){//child_ozd process
        //close(pipefd_ozd[1]);//writing end
        printf("%i\n", i);
        close(pipefd_komlo[0]);
        close(pipefd_komlo[1]);
        kill(getppid(), SIGTERM);
        
        char buff[20];
        
        read(pipefd_ozd[0], buff, sizeof(buff));
        
        printf("Ozd nyitas: %s \n", buff);
        
        srand(getpid());
        int waittime = rand() % 2 + 2;
        //sleep(waittime);
        
        int valasztott_ut = rand() % 3;
        
       // sem_wait(semid);
        
        printf("Ozd: %s \n", oszt_mem_ptr[valasztott_ut].cel);
        if(oszt_mem_ptr[valasztott_ut].ferohely > 0){
            write(pipefd_ozd[1], &valasztott_ut, sizeof(valasztott_ut));
        }else{
            valasztott_ut = -1;
            write(pipefd_ozd[1], &valasztott_ut, sizeof(valasztott_ut));
            
        }

      //  sem_post(semid);
        
        kill(getppid(), SIGTERM);
        
        
        while(i <= 0)usleep(100);
        char valasz[20];
        read(pipefd_ozd[0], valasz, sizeof(valasz));
        printf("Ozd: %s jelentkezes\n", valasz);
        
        close(pipefd_ozd[0]);
        close(pipefd_ozd[1]);
        
        fflush(stdout);
        shmdt(oszt_mem_ptr);
        
        
        exit(0);
    }
}
