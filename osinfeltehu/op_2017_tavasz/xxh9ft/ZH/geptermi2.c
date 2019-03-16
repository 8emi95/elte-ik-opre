//Read and print out the file given in the parameter
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 


struct Question {
    char q[300];
    char a[300];
    char b[300];
    char c[300];
    char d[300];
    char date[18];
};

struct Question questions[100];
int q_num = 0;

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}


struct Uzenet {
     long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ];
};

// sendig a message
int kuld( int uzenetsor ) {
     const struct Uzenet uz = { 5, "Hajra Fradi!" };
     int status;
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) {
          perror("msgsnd");
     }
     return 0;
}
     
// receiving a message. 
int fogad( int uzenetsor ) {
     struct Uzenet uz;
     int status;
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0);
     
     if ( status < 0 )
          perror("msgsnd");
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
     
     return 0;
}




/** /
int main (int argc, char* argv[]) {
    int ii = 0;
    for(ii=0; ii<3; ++ii) {
        printf("%d.\n", ii+1);

        pid_t child;
        int uzenetsor, status;
        key_t kulcs;
        
        // msgget needs a key, amelyet az ftok gener�l
        //.
        kulcs = ftok(argv[0],1);
        printf ("A kulcs: %d\n",kulcs);
        uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
        if ( uzenetsor < 0 ) {
            perror("msgget");
            return 1;
        }
        
        child = fork();
        if ( child > 0 ) {
            kuld( uzenetsor );  // Parent sends a message.
            wait( NULL );
            // After terminating child process, the message queue is deleted.
            status = msgctl( uzenetsor, IPC_RMID, NULL );
            if ( status < 0 )
                perror("msgctl");
            return 0;
        } else if ( child == 0 ) {
            return fogad( uzenetsor );
            // The child process receives a message.
        } else {
            perror("fork");
            return 1;
        }
    }

    return 0;
}
/**/






/**/
int main(int argc, char** argv) {
    int pToC[2], cToP[2]; // csővezetékek létrehozása
    int n;
    pid_t pid = 0;
    char sz[300];  // char array for reading from pipe
    signal(SIGTERM, handler); //handler = SIG_IGN - ignore the signal (not SIGKILL, SIGSTOP), 

    // pipe fv hívásnak a fork előtt kell lenni
    if (pipe(pToC) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    if (pipe(cToP) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    pid = fork();	// creating parent-child processes
    if (pid == -1) {
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    // child process (utas)
    if (pid == 0) {
        printf("Utas indul.\n");
        
        //kill(getppid(), SIGTERM); // üzenet, hogy készen állok.

        close(cToP[0]);  // cToP-ből nem olvas a gyerek
        close(pToC[1]);  // Usually we close the unused write end
        // itt használhatom pToC-t
        close(pToC[0]);

        // lakcím küldése
        if(argc < 2) {
            printf("Nincs lakcím megadva!\n");
            exit(1);
        }
        write(cToP[1], argv[1], 14);

        close(cToP[1]); // finally we close the used read end

        exit(EXIT_SUCCESS);
    }
    else
    {    // szulo process 
        printf("Szulo indul!\n");

        // A szülő véletlenszerűen kiválaszt 3 kérdést, amit a csővezetéken kiküld a kérdezőbiztosnak.
        close(pToC[0]); // Usually we close unused read end
        close(cToP[1]); // gyerektőlSzülőnek csőbe nem ír a szülő

        srand(time(NULL));

        close(pToC[1]); // Closing write descriptor 

        // szülő kiolvassa a lakcímeket, stb.
        char buff[200];
        int i = 0;
        while (n = read(cToP[0], &buff, sizeof(buff)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
            printf("%d. - %s\n", i+1, buff);
            ++i;
        }
        close(cToP[0]); // cToP olvasását befejezte a szülő

        /**/
        pid_t child;
        int uzenetsor, status;
        key_t kulcs;
        
        kulcs = ftok(argv[0],1);
        printf ("A kulcs: %d\n",kulcs);
        uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
        if ( uzenetsor < 0 ) {
            perror("msgget");
            return 1;
        }
        
        child = fork();
        if ( child > 0 ) { // SZÜLŐ
            printf("taxis indul\n");
            /**/
            struct Uzenet uz = { 222, "sikerult" };
            kuld( uzenetsor );  // Parent sends a message.
            wait( NULL );
            // After terminating child process, the message queue is deleted.
            status = msgctl( uzenetsor, IPC_RMID, NULL );
            if ( status < 0 )
                perror("msgctl");
            /**/
            //return 0;
        } /**/
        else if ( child == 0 ) { // TAXIS
            printf("szulo folytatódik\n");
            fogad( uzenetsor );
            // The child process receives a message.
        } else {
            perror("fork");
            return 1;
        }
        /**/


        wait(NULL);		// waiting for child process (not necessary)
        printf("Szulo befejezte!\n");
    }

    return 0;
}
/**/