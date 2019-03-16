 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    int status;
     key_t key = ftok("omem.c", 3);  // <-- ebben volt a hiba
    //key_t key = 8537; // -1 ellenõrzése, az a hiba, bead fontos, zh nem
    int id = shmget(key, 100, IPC_CREAT|0600);
    printf("%d\n", key);
    pid_t child = fork();
    if (child > 0) {
        char* t = shmat(id, NULL, 0);
        wait(&status); //bevárja a gyerek befejezését
        printf("%s\n", t);
        shmdt(t);
        shmctl(id, IPC_RMID, 0);
    } else {
        char* s = shmat(id, NULL, 0);
        strcpy(s, "Ez az uzenet");
        shmdt(s);
    }
    return 0;
}
