#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit

void check(int argc){
	if(argc!=2){
		printf("add meg a gyerekeket :v\n");
		exit(1);
	}
}

void handler(int sign){
	printf("Signal '%d' is handled.\n",sign);
}

int main(int argc,char*argv[]){
	check(argc);
	int n = atoi(argv[1]);
	int i;
	
	pid_t parentID = getpid();
	pid_t pid;
	
	for(i=0;i<n;++i){
		if(parentID == getpid()){
			if((pid=fork())<0) {
				printf("fork shit (%d)",i);
				exit(1);
			}
		}
	}
	printf("      pid -> %d\n",pid);
	if(pid > 0) { // Parent
		printf("Papa itt van (id: %d)\n",getpid());
		int c;
		for(c=1;c<=n;++c) {
			printf(" kell egy signal (%d) :v\n",c);
			//pause();
			signal(SIGUSR1,handler);sleep(1);
			printf(" Megkaptam (%d) :P\n\n",c);
		}
		int st;
		wait(&st);
		printf(" - %d. varas..\n",c);
		
		printf("Papa vegzett\n");
	}
	else { // Child
		printf("  | Itt a gyerek (id: %d, par: %d)\n",getpid(),getppid());
		printf("   - uzi papanak (%d)\n",SIGUSR1);
		kill(getppid(),SIGUSR1);
		printf("   - elkuldve (%d)\n",SIGUSR1);
		printf("  | Gyerek vege(id: %d, par: %d)\n\n",getpid(),getppid());
	}
	
	return 0;
}