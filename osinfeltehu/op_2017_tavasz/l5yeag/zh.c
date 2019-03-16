#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time
#include <signal.h> //sigval
	
void handler(int sign){
}

int main(int argc, char* argv[]){
	
	if(argc<2){
		printf("Add meg a lakcimet\n");
		exit(1);
	}
	
	int i;
	char address[512];
	strcpy(address,"");
	for (i=1; i<argc;++i){
		strcat(address,argv[i]);
		strcat(address," ");
	}
	pid_t parentID = getpid();
	pid_t pid;
	pid_t car_pid;
	int pp[2];
	char str[512];
	signal(SIGUSR1,handler);
	
	if (pipe(pp) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(1);
	}
	pid=fork();
	if(pid > 0) { // kozpont
		printf("A kozpont keszen all!\n");
		
		pause();
		//bejelentkezett az utas
		car_pid=fork();
		if(car_pid>0){//kozpont
			close(pp[1]);  //Usually we close the unused write end
			read(pp[0],str,sizeof(str)); // reading 
			printf("A kozpont olvasta a lakcimet: %s \n",str);
			close(pp[0]);  
			
		   int status;
		   waitpid(pid, &status, 0); //megvarjuk meg az utas es az auto vegez
		   waitpid(car_pid, &status, 0); 
			
			printf("A kozpont vegzett\n");
		}else{//auto
			printf("Az autot hozzarendelem\n");
		}
	}
	else { // utas
		sleep(3);
		printf("Itt az utas\n");
		kill(getppid(),SIGUSR1);
		
		close(pp[0]); //Usually we close unused read end
		write(pp[1],address,sizeof(address));
		close(pp[1]); // Closing write descriptor 
		
		printf("Az utas beirta a lakcimet\n");
		printf("Az utas vegzett (%d)\n",getpid());
	}
	
	return 0;
}