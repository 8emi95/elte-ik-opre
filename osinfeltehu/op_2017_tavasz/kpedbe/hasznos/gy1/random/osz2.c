#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

// 3. Osszunk meg egy structot (egy int es egy char[80] adattag) osztott memorian 
//	keresztul. Egyik folyamat irjon, masik olvassa ki.

struct pentek
{
    int a;
    char b[80];
};

int main(int argc, char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;
    struct pentek *s;
    
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs = ftok(argv[0], 1);
    
	// osztott memoria letrehozasa, irasra olvasasra, structnyi bajt mrettel
    oszt_mem_id = shmget(kulcs, sizeof(struct pentek), IPC_CREAT | S_IRUSR | S_IWUSR);

	// csatlakozunk az osztott memoriahoz, 
    s = shmat(oszt_mem_id, NULL, 0);

    pid = fork();
    if (pid > 0) {

        char buffer[] = "Hajra Fradi! \n";
        // beirunk a memoriaba 
        (*s).a = 80;
        strcpy((*s).b, "AAA");
        // elengedjuk az osztott memoriat
        shmdt(s);
        //	s[0]='B';  //ez segmentation fault hibat ad
        wait(NULL);
        shmctl(oszt_mem_id, IPC_RMID, NULL);
    }
    else
        if (pid == 0)
        {
            sleep(1);
            printf("A gyerek ezt olvasta az osztott memoriabol: %i %s", (*s).a, (*s).b);
            // gyerek is elengedi
            shmdt(s);
        }

    return 0;
}
