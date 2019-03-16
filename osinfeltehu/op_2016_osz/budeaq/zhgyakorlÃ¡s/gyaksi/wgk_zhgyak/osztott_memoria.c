#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

struct Struktura {
	int a;
	char op;
	int b;
	int result;
};

int main (int argc,char* argv[]) {
    key_t kulcs = ftok(argv[0],1); // Kulcs generálása, amely az osztott memóriához fog adni egy egyedi kulcsot, amit az OS fog kezelni
    int oszt_mem_id = shmget(kulcs, 10 * sizeof(struct Struktura), IPC_CREAT|S_IRUSR|S_IWUSR); // Osztott memória foglalása
	struct Struktura *s = shmat(oszt_mem_id, NULL, 0); // Osztott memóriához való csatlakozás
    
    int child = fork();
    if(child > 0){    
		// SZÜLÕ
		struct Struktura uzenet = {1, '+', 3, 4};
		memcpy(s, &uzenet, sizeof(uzenet));
		waitpid(child, NULL, 0); 
		printf("A gyerek ezt olvasta: %d %c %d = %d\n", s->a, s->op, s->b, s->result);
		shmdt(s); // Lecsatlakozik az osztott memóriáról
		shmctl(oszt_mem_id, IPC_RMID, NULL); // Töröljük az osztott memóriát
    } else {
		// GYEREK
		sleep(1);	              
        printf("A gyerek ezt olvasta: %d %c %d = %d\n", s->a, s->op, s->b, s->result);
        s->result = 10;
		shmdt(s); // Lecsatlakozik az osztott memóriáról
		exit(0);
	}

   return 0;
}
