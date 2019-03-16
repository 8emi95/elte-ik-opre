#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void handler(int signumber){}

const char* fails[3] = {"bananhej","hideg","semmi"};

typedef struct{
	char Id[32];
}S;

int main(int argc, char* argv[]){
	if(argc==1){
		printf("Add meg paramterken hany szerelveny legyen osszesen legyen!\n");
		exit(0);
	}
	
	srand(time(NULL));
	signal(SIGUSR1,handler);
	
	int pipe_p[2];
	int pipe_c[2];
	
	if(pipe(pipe_p) == -1){
		perror("Hiba a szulo pipe letrehozasa kozben!\n");
		exit(1);
	}
	if(pipe(pipe_c) == -1){
		perror("Hiba a gyerek pipe letrehozasa kozben!\n");
		exit(1);
	}

	int status;
	
	
		//szulo
		int i = 0;
		
		int max = atoi(argv[1]);
		
		while(i<max){
			pid_t child = fork();
			if(child<0){
				perror("Hiba a fork hívás közben!\n");
				exit(1);
			}
			if(child==0){
				//gyerek
				int max;
				int num;
				read(pipe_p[0], &max, sizeof(max));
				read(pipe_p[0], &num, sizeof(num));
				
				printf("Szerelveny: en vagyok a(z) %i. szerelveny, es meg %i db van hatra!\n",num,max-num);
				
				sleep(2);
				
				int r = rand()%10;
				
				// if(r<5){
					// write(pipe_c[1], "bananhej", sizeof("bananhej"));
				// }
				// else if(r>5 && r<10){
					// write(pipe_c[1], "hideg", sizeof("hideg"));
				// }
				// else{
					// write(pipe_c[1], "semmi", sizeof("semmi"));
				// }
				
				int fail;

				if(r==0){
					fail = rand()%2;
					
				}
				else{
					fail = 2;
				}
				
				kill(getppid(),SIGUSR1);
				
				write(pipe_c[1], &fail, sizeof(fail));

				exit(0);
			}
			else{
				int num = i+1;
				char fail[32];
				
				write(pipe_p[1], &max, sizeof(max));
				write(pipe_p[1], &num, sizeof(num));
				
				printf("Tarsasag: elkuldtem az adatokat, most varom, hogy megerkezzen a szerelveny!\n");
				
				pause();
				int f;
				char* fs;
				read(pipe_c[0], &f, sizeof(f));
				
				switch(f){
					case 0:
						fs = "bananhej";
						break;
					case 1:
						fs = "hideg";
						break;
					case 2:
						fs = "semmi";
						break;
				}
				
				printf("Tarsasag: megerkezett a szerelveny, ez a hiba csuszott kozbe: %s\n",fs);
				
				++i;
				
				wait(&status);
			}
			
		}

}