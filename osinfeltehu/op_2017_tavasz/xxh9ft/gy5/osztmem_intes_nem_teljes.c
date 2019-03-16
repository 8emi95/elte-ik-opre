#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main (int argc, char* argv[]) {
    int pid;
    key_t kulcs;
    int oszt_mem_id;
    int *s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs = ftok(argv[0], 1); // IPC_CREAT-tel is lehet, akkor csak az adott program, és annak gyerekfolyamatai érik el.
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id = shmget(kulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    // shmget(kulcs, méretBájtban, jogosultság);
    // csatlakozunk az osztott memoriahoz, 
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
    s = shmat(oszt_mem_id, NULL, 0);
    //
    pid = fork();
    if(pid > 0) {
       // beirunk a memoriaba
       *s = 2222;
       // elengedjuk az osztott memoriat
       shmdt(s);
       
       wait(NULL);
       // IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
       // IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
       //  ami shmid_ds struct tipusu mutato
       shmctl(oszt_mem_id, IPC_RMID, NULL); // ez töröl. Akkor történik meg ténylegesen a törtlés, ha minden korábban felcsatlakozott folyamat lejelentkezett!
    } else {
        if(pid == 0) {
            sleep(1); // it várja, hogy beleírjon a szülő a memóriába, kell várni, különben még szemét van
            // sleep helyett jelzéssel is lehet, de igazi megolás a szemafor!

            printf("A gyerek ezt olvasta az osztott memoriabol: %d\n", *s);
            // gyerek is elengedi
            shmdt(s);
        }
    }

   return 0;
}











/** /
 shm_server.c

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    // We'll name our shared memory segment "5678".
    key = 5678;

    // Create the segment.
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Now we attach the segment to our data space.
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Now put some things into the memory for the other process to read.
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;

    // Finally, we wait until the other process changes the first character of our memory to '*', indicating that it has read what we put there.
    while (*shm != '*')
        sleep(1);

    exit(0);
}




shm_client.c

// shm-client - client program to demonstrate shared memory.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ     27

main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    // We need to get the segment named "5678", created by the server.
    key = 5678;

    // Locate the segment.
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Now we attach the segment to our data space.
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Now read what the server put in the memory.
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    // Finally, change the first character of the segment to '*', indicating we have read the segment.
    *shm = '*';

    exit(0);
}
/**/