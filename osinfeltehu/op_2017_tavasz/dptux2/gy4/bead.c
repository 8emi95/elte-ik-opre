#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>
#include <sys/types.h>
int szamlalo = 0;

void handler(int signumber){
	printf("Signal with number %i has arrived\n",signumber);
	szamlalo++;
}


int pecset(){
	printf("pecsetelo ready!\n");
	kill(getppid(),SIGUSR1);
};
int elnok(){
	printf("Elnok ready!\n");
};
int ellenorzo(){
	printf("Ellenorzo ready!\n");
	kill(getppid(),SIGUSR1);
};


 int main(int argc, char *argv[]){
	srand(time(NULL));
	signal(SIGUSR1,handler);
	 int fd1[2];
	 int fd2[2];
	
	if( argc != 2 ){
		perror("egy argumentum..\n");
		exit(1);
	}
	
	int szavazokszama = atoi( argv[1] );

	if (pipe(fd1)<0 && pipe(fd2)<0 ) {
		perror("Hibas a cso letrehozas\n");
		exit(1);
	}

	pid_t ellen = fork();
	
	if(ellen>0)
	{
		pid_t pecsetelo=fork();
		if(pecsetelo > 0){
			while(szamlalo != 2){usleep(100);}
			elnok();
			int i  = 0;
			
			close(fd1[0]);
			close(fd2[1]);
			for(;i<szavazokszama;i++){
				 int r=rand()%100;
				  write(fd1[1],&r,sizeof(int));
				  read(fd2[0],NULL,sizeof(int));
			}
			close(fd2[0]);
			close(fd1[1]);
			
			wait(NULL);wait(NULL);
		}else{
			pecset();
		}
	}else{
		close(fd2[0]);
		close(fd1[1]);
		ellenorzo();
		int n;
		while(read(fd1[0],&n,sizeof(int))){
			printf("Erkezes: %i \n",n);
			write(fd1[1],NULL,sizeof(int));
		}
		close(fd2[1]);
		close(fd1[0]);
	}
	return 0;
}