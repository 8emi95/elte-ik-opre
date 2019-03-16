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
    int  siker, random;
    int potvizsga;
    printf("JATEKOS: Jatekos keszenall a jatekra\n");
    kill(getppid(), SIGUSR1); 
    
    close(fd[1]);
    close(fd2[0]);
    while(read(fd[0], &buffer, sizeof(buffer))) {
        printf("JATEKOS: A feladatom: %s\n", buffer);
        potvizsga = strcmp(buffer, "potfeladat");
        read(fd[0], &int_buffer, sizeof(int));
        read(fd[0], &buffer, sizeof(buffer));
        printf("JATEKOS: nevem es sorszamom: %s, %d\n", buffer, int_buffer);
        if (potvizsga) {
           random = rand() % 10;
           siker = random == 1 || random == 2 ? 0 : 1;
        } 
        else {
           siker = rand() %2;
        }
        printf("JATEKOS: A feladat %s\n", siker ? "sikerult!!" : "nem sikerult :(");
        write(fd2[1], &siker, sizeof(int));
        kill(getppid(), SIGUSR1); 
    }
    close(fd[0]);
    close(fd2[1]);
}

int main (int argc, char* argv[]) {
    pid_t child;
    char* name;
    int elhasaltak[argc-1];
    int elhasaltak_n;

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
       close(fd[0]);
       close(fd2[1]);
       for (i=1; i<argc; ++i) {
           name = argv[i];
           write(fd[1],"kedvenc feladatom", sizeof(buffer));
           write(fd[1],&i,sizeof(int));
           write(fd[1],name,sizeof(buffer));
           pause();
           read(fd2[0], &int_buffer, sizeof(int));
           printf("Trener: %s %s.\n", name, int_buffer ? "megoldotta a feladatot" : "nem oldotta meg a feladatot"); 
           if (!int_buffer) {
               elhasaltak[elhasaltak_n++]=i;
           } 
       }
       int potvizsga = 0;
       int index;
       printf("\n\nPOTVIZSGA!!!\n");
       for (i=0; i<elhasaltak_n; ++i) {
           index = elhasaltak[i];
           name = argv[index+1];
           write(fd[1],"potfeladat", sizeof(buffer));
           write(fd[1],&i,sizeof(int));
           write(fd[1],name,sizeof(buffer));
           pause();
           read(fd2[0], &int_buffer, sizeof(int));
           printf("Trener: %s %s.\n", name, int_buffer ? "megoldotta a feladatot" : "nem oldotta meg a feladatot"); 
           if (int_buffer) {
               potvizsga++;
           }
       }
       
       printf("Osszesitett eredmeny: elsore atment: %d, masodjara: %d\n", argc-1-elhasaltak_n, potvizsga);
       close(fd[1]);
       close(fd2[0]);
       wait(NULL);
    }
    else {
       //GYERMEK
       jatekos();
    }

    return 0;
}
