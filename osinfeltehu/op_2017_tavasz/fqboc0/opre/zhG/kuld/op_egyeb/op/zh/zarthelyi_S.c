//BPJQT6
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int starter = 100;

void signal_jatekos1(){ printf("Az 1-es jatekos keszen all!\n"); }
void signal_jatekos2(){ printf("A 2-es jatekos keszen all!\n"); }
void signal_kikezd() { printf("1-es jatekos kezd!\n"); starter = 0; }
void signal_kikezd2() { printf("2-es jatekos kezd!\n"); starter = 1;}

pid_t jatekos1;
pid_t jatekos2;

int main(int argc, char* argv[]) {

    signal(SIGUSR1,signal_jatekos1);
    signal(SIGUSR2,signal_jatekos2);
	signal(SIGPOLL, signal_kikezd);
	signal(SIGTERM, signal_kikezd2);
	
	int cso[2], cso2[2], nbytes;
	//0 olvasás, 1 írás
	
	
	char readbuffer[80];

	pipe(cso);
	pipe(cso2);
	
	

    jatekos1 = fork();
	
    if (jatekos1 < 0) { 
		perror("Fork hiba\n"); 
	}

    if (jatekos1 > 0) { // (SZÜLŐ)
	
        pause();
		
        jatekos2 = fork();

		if (jatekos2 < 0) { 
			perror("Fork fail\n"); 
		}

        if (jatekos2 > 0) { // (SZÜLŐ)
		
			pause();
			
			printf("A jatekosok elkeszultek!\n");
			
			//ki kezd
			srand(time(NULL));
			int rnd;
			int kikezd = rand() % 2;
			
			//int kikezd = 1;
			signal(SIGUSR1,signal_jatekos1);
			signal(SIGUSR2,signal_jatekos2);
			switch (kikezd){
				case 0: printf("Szulo: 1-es jatekos kezd\n", kikezd); kill(jatekos1, SIGPOLL); kill(jatekos2, SIGPOLL); break;
				case 1: printf("Szulo: 2-es jatekos kezd\n", kikezd); kill(jatekos1, SIGTERM); kill(jatekos2, SIGTERM); break;
				default: break;
			}

			
			/*
			//jatekos2 azonosito
			close(cso[0]); 
			write(cso[1],kezdoGamer,strlen(kezdoGamer)+1); 
			close(cso[1]);
			
			//jatekos1 azonosito
			close(cso2[0]); 
			write(cso2[1],kezdoGamer,strlen(kezdoGamer)+1); 
			close(cso2[1]);
			*/
			waitpid(jatekos1, NULL, 0);
			waitpid(jatekos2, NULL, 0);

        } else { // jatekos1
		
            kill(getppid(), SIGUSR1);
			pause();
			printf("JatekosX: %d",starter);
			/*
			close(cso[1]);
			nbytes = read(cso[0], &readbuffer, sizeof(readbuffer));
			close(cso[0]);
			*/
			//pause();
			//printf("Jatekos 1: a szulo azt uzeni, hogy %s kezd\n", readbuffer);
			
        }

    } else { // jatekos2
	
        kill(getppid(), SIGUSR2);
		pause();
		printf("JatekosY: %d",starter);
		/*
		close(cso2[1]);
		nbytes = read(cso2[0], &readbuffer, sizeof(readbuffer));
		close(cso2[0]);
		*/
		//pause();
		//printf("Jatekos 2: a szulo azt uzeni, hogy %s kezd\n", readbuffer);
		
		
    }

    return 0;
}
