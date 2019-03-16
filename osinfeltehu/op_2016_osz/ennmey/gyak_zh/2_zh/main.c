#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define READ_END 0
#define WRITE_END 1
// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink
// -lrt -vel fordit!


int compare_strings(char a[], char b[])
{
   int c = 0;
 
   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
 
   if (a[c] == '\0' && b[c] == '\0')
      return 1;
   else
      return 0;
}

void arrivalAction(int signumber){
    printf("Jelzes megkapva.\n");/*
    if(10==signumber)
    {
        printf("Sikeresen beert a szerelveny.\n");
    }
    else
    {
        printf("Be lett vontatva.\n");
    }*/
}

int main (int argc, char* argv[]) { 

    int k;
    int feladat[argc];
    int elhasalt[argc];
    pid_t init_pid = getpid();
    pid_t child,ujchild; 
    
    int szereti=0;
    int elsore=0;
    int masodikra=0;
    
    int elso_kor=1;
    char string[30];
    char str[30];
    
    typedef struct
    {
        char name[30];
        char siker[30];
    } val_t;
    
    int shmid;
    val_t   *array;
    int count = 5;
    int i;
    int SizeMem;
    key_t key;
    
    key=ftok(argv[0],1);
    
    //SizeMem = sizeof(*array)*count;
    
    val_t   *val;
    int size = sizeof(val_t) * argc;
    //shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666)
    
    shmid = shmget(key,size,IPC_CREAT|S_IRUSR|S_IWUSR);
    
    //oszt_mem_id=
    array = (val_t *)shmat(shmid, 0, 0);
    
    struct sigaction partnerArrive;
    partnerArrive.sa_handler = arrivalAction;
    sigemptyset(&partnerArrive.sa_mask);
    partnerArrive.sa_flags = 0;
    sigaction(SIGUSR1,&partnerArrive,NULL);
    
    for(k=1; k<argc; ++k)
    {
        feladat[k] = (rand()%argc+1);
        elhasalt[k] = 0;        
    }
    
    for(k=1; k<argc; ++k)
    {
        if(elso_kor || elhasalt[k])
        {
            int pipefd[2];
            int back_pipefd[2];
            
            if (pipe(pipefd) == -1)
            {
                perror("Hiba a pipe nyitaskor!");
                exit(EXIT_FAILURE);
            }
            
            if (pipe(back_pipefd) == -1)
            {
                perror("Hiba a pipe nyitaskor!");
                exit(EXIT_FAILURE);
            }
            
            if(init_pid==getpid())
            {
            child = fork();
            srand(time(NULL) ^ (getpid()<<16));
            if((child)<0) {
                    printf("forking error");
                    exit(1);
                }
            } 
            if ( init_pid==getpid() ) { 
                close(pipefd[READ_END]);
                close(back_pipefd[WRITE_END]);
                
                if(elso_kor){
                    
                    sigset_t sigset;
                    sigfillset(&sigset);
                    sigdelset(&sigset,SIGUSR1);
                    sigdelset(&sigset,SIGINT);
                    
                    sprintf(string,"%d",feladat[k]); //feladat generalasa
                    write(pipefd[1], string,sizeof(string));
                    
                    write(pipefd[1], argv[k],sizeof(string));
                    
                    sprintf(str,"%d",k);
                    write(pipefd[1], str,sizeof(str));
                    
                    
                    sigsuspend(&sigset);
                    
                    //visszakap eredmenyt
                    read(back_pipefd[READ_END],string,sizeof(string)); // reading max 100 chars
                    if(compare_strings(string,"nem sikerult\n"))
                    {
                        elhasalt[k] = 1;
                    }
                }
                else
                {
                    sprintf(string,"%d",feladat[k]); //feladat generalasa
                    write(pipefd[1], string,sizeof(string));
                    read(back_pipefd[READ_END],string,sizeof(string)); // reading max 100 chars
                    if(compare_strings(string,"sikerult\n"))
                    {
                        elhasalt[k] = 0;
                    }
                }
                close(pipefd[WRITE_END]);
                close(back_pipefd[WRITE_END]);
                int status;
                wait(&status);
                //ujraindit potfeladatokkal
                if(elso_kor==1 && k==argc-1)
                {
                    k=0;
                    elso_kor=0;
                }
            }
            else
            {
                close(pipefd[WRITE_END]);
                close(back_pipefd[READ_END]);
                int siker = rand()%10;
                if(elso_kor)
                {
                    read(pipefd[READ_END],string,sizeof(string)); // reading max 100 chars
                    printf("A feladatom a(z) %s. sorszamu, ", string);
                    
                    read(pipefd[READ_END],string,sizeof(string)); // reading max 100 chars
                    printf("nevem %s ", string);
                    strcpy(array[k-1].name,string);
                    
                    
                    read(pipefd[READ_END],str,sizeof(str)); // reading max 100 chars
                    printf("es en a(z) %s. jatekos vagyok\n", str);
                    
                    if(siker>3)
                    {
                        char prob_1[30] = "1";
                        strcpy(array[k-1].siker,prob_1);
                        printf("Sikerult megoldanom a feladatot!\n");
                        write(back_pipefd[WRITE_END], "sikerult\n",sizeof(string));
                    }
                    else
                    {
                        char prob_2[30] = "2";
                        strcpy(array[k-1].siker,prob_2);
                        printf("Nem sikerult megoldanom a feladatot!\n");
                        write(back_pipefd[WRITE_END], "nem sikerult\n",sizeof(string));
                    }
                    
                    kill(getppid(),SIGUSR1);
                }
                else
                {
                    siker = rand()%10;
                    printf("Potlas inditasa!");
                    read(pipefd[READ_END],string,sizeof(string)); // reading max 100 chars
                    printf("A potfeladatom a(z) %s. sorszamu, ", string);
                    printf("nevem: %s\n",argv[k]);
                    if(siker<8)
                    {
                        char prob_2[30] = "2";
                        strcpy(array[k-1].siker,prob_2);
                        printf("Sikerult megoldanom a feladatot!\n");
                        write(back_pipefd[WRITE_END], "sikerult\n",sizeof(string));
                    }
                    else
                    {
                        
                        char prob_2[30] = "Szeretek ittmaradni.";
                        strcpy(array[k-1].siker,prob_2);
                        printf("Nem sikerult megoldanom a feladatot!\n");
                        write(back_pipefd[WRITE_END], "nem sikerult\n",sizeof(string));
                    }
                }
                close(pipefd[READ_END]);
                close(back_pipefd[WRITE_END]);
                shmdt(array);
                exit(0);
            }
        }
    }
    //osszesit
    for(i=0; i<argc-1; ++i)
    {
        printf("%s-nek ", array[i].name);
        if(elhasalt[i])
        {
            printf("%s sikere: ", array[i].siker);
        }
        printf(" sikerult megoldania feladatot.\n");
        shmdt(array);
        shmctl(shmid,IPC_RMID,NULL);
    }
    return 0; 
} 
