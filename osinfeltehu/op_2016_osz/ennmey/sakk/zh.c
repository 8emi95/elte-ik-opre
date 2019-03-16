#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time

void handler(int sign){
	printf("");
	//printf("Signal '%d' is handled.\n",sign);
}

int main(int argc,char*argv[]){
	//int n = atoi(argv[1]);
	int i;
	char str[64];
	char lepes[64];
	
	pid_t parentID = getpid();
	pid_t pid;
	int pp[2];
	int lepes_pipe[2];
	
	if(pipe(pp)==-1){
		perror("pipe error\n");
		exit(1);
	}
	
	for(i=0;i<2;++i){
		if(parentID == getpid()){
             printf("fork elott vagyok ezzel a piddel: %d\n",getpid());
			if((pid=fork())<0) {
				printf("forking error (%d)",i);
				exit(1);
			}
            else
            {
                printf("fork utan vagyok ezzel a piddel: %d\n",getpid());
            }
		}
	}
	if(pid > 0) { // Parent
		srand(time(NULL)^(getpid()));
		printf("Szulo itt van (id: %d)\n",getpid());
		
		int c;
		int r = rand()%2;
		printf("Random szam: %i\n", r);
		sprintf(str, "%d", r);		
		for(c=1;c<=2;++c) {
			if(c == 2){
				r = r + 1;
				r = r % 2;
				sprintf(str, "%d", r);
				signal(SIGUSR1,handler);sleep(1);
			}else{
				signal(SIGUSR2,handler);sleep(1);
			}
			
			close(pp[0]);
			write(pp[1],str,sizeof(str));
		}
		int st;
		wait(&st);
		
		printf("Szulo vegzett (%d)\n",getpid());
	}
	else { // Child
		printf("  | Gyerek el es virul (id: %d)\n\n",getpid());
		close(pp[1]);
		read(pp[0],str,sizeof(str));
		int r2 = 2 + rand()%3;
		printf("Gyerek olvasta: %s ", str);
		if(str[0] == '0'){
			printf(" Gondolkodok... %i masodpercig. ", r2);
			sleep(r2);
			printf("Ez a gyerek kezd!\n", SIGUSR1);
			lepes[0] = 'B';		
			lepes[1] = '4';
			lepes[2] = 'C';
			lepes[3] = '4';
			
			close(lepes_pipe[0]);
			write(lepes_pipe[1],lepes,4);
			kill(getppid(),SIGUSR1);
		}else{
			printf("Ez a gyerek NEM KEZD!\n", SIGUSR2);sleep(1);
			close(lepes_pipe[1]);
			read(lepes_pipe[0],lepes,4);
	
			lepes[0] = 'D';		
			lepes[1] = '7';
			lepes[2] = 'E';
			lepes[3] = '4';	
			close(lepes_pipe[0]);
			write(lepes_pipe[1],lepes,4);
			kill(getppid(),SIGUSR2);
		}
		
		printf("  | Gyerek voltam (id: %d)\n\n",getpid());
	}
	
	return 0;
}