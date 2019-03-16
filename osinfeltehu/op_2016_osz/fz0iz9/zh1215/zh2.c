#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

int fd[2];
int fd2[2];
char buffer[40];
int int_buffer; 

void handler() {
    printf("Jelzes erkezett!\n");
}

void jatekos() {
    int  siker;
    
    close(fd[1]);
    close(fd2[0]);
    printf("JATEKOS: Jatekos keszenall a jatekra\n");
    kill(getppid(), SIGUSR1); 
    read(fd[0], &buffer, sizeof(buffer));
    printf("JATEKOS: A feladatom: %s\n", buffer);
    read(fd[0], &int_buffer, sizeof(int));
    read(fd[0], &buffer, sizeof(buffer));
    printf("JATEKOS: nevem es sorszamom: %s, %d\n", buffer, int_buffer);

    siker = rand() % 2;
    printf("JATEKOS: A feladat %s\n", siker ? "sikerult!!" : "nem sikerult :(");
    write(fd2[1], &siker, sizeof(int));
    close(fd[0]);
    close(fd2[1]);
    kill(getppid(), SIGUSR1); 
}

int main (int argc, char* argv[]) {
    pid_t child;
    char* name;

    srand(time(NULL));
	signal(SIGUSR1, handler);

	if (argc < 2) {
		printf("Hianyzo arg.\n");
		exit(EXIT_FAILURE);
	}

    if (pipe(fd) < 0 || pipe(fd2) < 0) {
        printf("Cso letrehozasa nem sikerult.\n");
        exit(EXIT_FAILURE);
    }

    child = fork();

    if (child < 0) {
        printf("Fork nem sikerult.\n");
        exit(EXIT_FAILURE);
    }
    else if (child > 0) {
       int i;
       pause();
       name = argv[1];
       i=1;
       close(fd[0]);
       close(fd2[1]);
       write(fd[1],"kedvenc feladatom", sizeof(buffer));
       write(fd[1],&i,sizeof(int));
       write(fd[1],name,sizeof(buffer));
       close(fd[1]);
       pause();
       read(fd2[0], &int_buffer, sizeof(int));
       printf("Trener: %s %s.\n", name, int_buffer ? "megoldotta a feladatot" : "nem oldotta meg a feladatot"); 
       close(fd2[0]);
       wait(NULL);
    }
    else {
       //GYERMEK
       jatekos();
    }

    return 0;
}
