#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <wait.h>
 
struct uzenet {
    long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    char mtext [ 1024 ];
};
 
void handler(int signumber){
    printf("Jelzes kuldese megtortent\n",signumber);
}
 
// uzenetkuldes
int kuld( int uzenetsor ) {
    const char *jokivansagok[] = {"\n\n Vigyazzatok magatokra!\n\n", "\n\n Mindorokke Kiraly!\n\n", "\n\n Hurra!\n\n", "\n\n Zsiraf!\n\n"};
    //sleep(1);
    int random = rand()%4;
    struct uzenet uz = { 5, "Hajra Fradi!" };
    strcpy(uz.mtext, jokivansagok[random]);
 
    int status;
    // uzenetsor
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );
    // a 3. param ilyen is lehet: sizeof(uz.mtext)
    // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
    if ( status < 0 ) {
        perror("msgsnd");
    }
    return 0;
}
 
// uzenet fogadas
int fogad( int uzenetsor ) {
    struct uzenet uz;
    int status;
    // az utolso parameter(0) az uzenet azonositoszama
    // ha az 0, akkor a sor elso uzenetet vesszuk ki
    // ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
    // vesszuk ki a sorbol
    status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
     
    if (status < 0) {
        perror("msgsnd");
    }
    else
        printf( "A kapott kivansag:  %s\n", uz.mtext );
    return 0;
}
 
int main(int argc, char* argv[]) {
    srand(time(NULL));
    int uzenetsor, status;
    key_t kulcs;
   
    kulcs = ftok(argv[0],1);
    printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if (uzenetsor < 0) {
        perror("msgget");
        return 1;
    }
   
    signal(SIGTERM, handler);
 
    pid_t child = fork();
    if (child > 0) {
        pid_t child2 = fork();
        if(child2 == 0) {
            printf("A 2. jarorauto vegzett a reggelivel es jelzest kuldott a kozpontnak\n");
            kill(getppid(), SIGTERM);
            sleep(1);
            fogad(uzenetsor);
            printf("A 2. jarorauto befejezte a napi rutint\n");
        }
 
        else {
            pause();
            pause();
            printf("A jarorautok tudattak hogy keszen allnak a munkara\n");
               
            // Uzenetsor kuldese
            kuld(uzenetsor);
            kuld(uzenetsor);
            wait(NULL);
            status = msgctl( uzenetsor, IPC_RMID, NULL );
            if ( status < 0 ) perror("msgctl");
 
            int status;
            wait(&status);
            printf("A kozpont befejezte a mukodeset, kezdodhet a kaosz\n");
        }
    }
 
    else {
        int i;
        printf("Az 1. jarorauto megreggelizett es jelzest kuldott a kozpontnak\n");
        sleep(2);
        kill(getppid(), SIGTERM);
        fogad(uzenetsor);
        printf("Az 1. jarorauto befejezte a napi rutint\n");
    }
    return 0;
}