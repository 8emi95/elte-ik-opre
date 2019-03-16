#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void handlerUtas(int signum)
{
    printf("Utas (PID: %i) bejelentkezett.\n", signum);
}

void handlerKocsi(int signum)
{
    printf("Kocsi (PID: %i) hozzarendelve az utashoz.\n", signum);
}

int main(int argc, char** argv)
{
    if(argc != 2) { printf("A program egy darab lakcimet var argumentumkent (szokoz nelkul, pl alsovonallal_tagolva)!\n"); exit(1); }
    signal(SIGUSR1, handlerUtas);
    signal(SIGUSR2, handlerKocsi);

    int pipefd[2];
    if(pipe(pipefd) == -1) { printf("Nem sikerult a cso letrehozasa.\n"); exit(1); }
    
    pid_t utas;
    pid_t kocsi;

    utas = fork();
    if(utas < 0) { printf("Nem sikerult a gyerekfolyamat (utas) letrehozasa.\n"); exit(1); }
   
    if(utas == 0) // gyerek folyamat (utas)
    {
        sleep(1);
        kill(getppid(), SIGUSR1);
        write(pipefd[1], argv[1], 100);
    }else
    {
        kocsi = fork();
        if(kocsi < 0) { printf("Nem sikerult a gyerekfolyamat (kocsi) letrehozasa.\n"); exit(1); }
        if(kocsi == 0) // gyerek folyamat (kocsi)
        {
            sleep(2);
            kill(getppid(), SIGUSR2);
            
        }else // szulo folyamat (kozpont)
        {
             printf("A kozpont keszen all az utas fogadasara.\n");
             pause();
             pause();
             
             sleep(1);
             char address[100];
             read(pipefd[0], &address, 100);

             printf("A kozpont kiolvasta az utas lakcimet: %s\n", address);

             while(wait(NULL) > 0);
             printf("A kozpont befejezte az utas fogadasat.\n");
        }
    }
 
    remove("taxi.fifo");
    return 0;
}
