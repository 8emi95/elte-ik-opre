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
	long mtype;
	char mtext [ 1024 ]; 
};

int kuld(int uzenetsor, char *address, int pidNum) {
	struct uzenet uz = { 5, "Ez egy uzenet" };
    char pidID[32];
    sprintf(pidID, "%d", pidNum);
	strcpy(uz.mtext, address);
    strcat(uz.mtext, ", ");
    strcat(uz.mtext, pidID);

	int status; 
	// uzenetsor
	status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	if ( status < 0 ) { 
		perror("msgsnd");
	}
	return 0; 
}

// uzenet fogadas
int fogad(int uzenetsor) {
	struct uzenet uz; 
	int status; 
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
	if (status < 0) {
		perror("msgsnd");
	}
	else
		printf( "A kapott cim, telefonszam: %s\n", uz.mtext ); 
	return 0; 
}

int main(int argc,char ** argv) {
    char *address = argv[1];
    printf("%s\n", address);

    char buffer[256];

//Pipe
    int child1pipe[2];
    int child2pipe[2];

    if (pipe(child1pipe) == -1) {
	   perror("Hiba a pipe nyitaskor(child1)!");
	   exit(EXIT_FAILURE);
	}

	if (pipe(child2pipe) == -1) {
	   perror("Hiba a pipe nyitaskor(child2)!");
	   exit(EXIT_FAILURE);
	}

//Üzenetküldés
    int uzenetsor, status;
	key_t kulcs;

    kulcs = ftok(argv[0],1); 
	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if (uzenetsor < 0) { 
		perror("msgget"); 
		return 1; 
	}

    pid_t child1 = fork();
    if(child1 > 0) {
        pid_t child2 = fork();
        if(child2 == 0) {
            //Child 2 - Taxi
            sleep(2);
            printf("T-Taxi bejelentkezik!\n");
            fogad(uzenetsor);
        } else {
            //Parent - Taxitársaság
            printf("T  --- Taxitarsasag varja az utast!\n");
            //utas
            //int status;
			//wait(&status);
            close(child1pipe[1]);
            read(child1pipe[0], buffer, sizeof(buffer));
            close(child1pipe[0]);
            printf("T  --- Utas cime: %s\n", buffer);

            //taxi
            printf("T  --- Adatok küldese a taxinak!\n");
            int pidNum = getpid();
            kuld(uzenetsor, address, pidNum);
            sleep(3);
            printf("A Taxitarsasag megvarta a gyerekfolyamatok veget\n");
        }
    } else {
        //Child 1 - Utas
        sleep(1);
        printf("U-Utas bejelentkezik a taxitarsasaghoz!\n");
        close(child1pipe[0]);
        write(child1pipe[1], address, sizeof(buffer));
        close(child1pipe[1]);
    }
}