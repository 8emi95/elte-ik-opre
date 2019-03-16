
// 1. Operációs Rendszerek Beadandó
// Készítette: Csabai Tamás (HYRZR6)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h> 
#include <signal.h>

void handler(int signumber){
  printf("KESZ JELZES!\n");
}

int main(int argc, char** argv ){
	
	if(argc != 2){
		printf("NEM MEGFELELO SZAMU ARGUMENTUM!\n");
		exit(EXIT_FAILURE);
	}
	
	int i;
	
	signal(SIGTERM,handler);
	
	int pid,fd;
	printf("START!\n");
	int fid=mkfifo("pipe.VAL", 0666 );
	if (fid == -1){
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
	}
	printf("FORK!\n\n");
	pid = fork();
	
	if(pid>0){   
		
		int pid2;
		pid2 = fork();
		if(pid2>0){ //elnök
			printf("Elnok: Varom a tagok jelzeset!\n");
			pause();
			printf("Elnok: Elso jelzes megerkezett!\n");
			pause();
			printf("Elnok: Masodik jelzes megerkezett!\n");
			
			fd = open("pipe.VAL", O_WRONLY);
			int szavazoSzam = atoi(argv[1]);
			printf("Elnok: Irok a csobe!\n");
			write(fd,&szavazoSzam,sizeof(szavazoSzam));
			printf("Elnok: Beirtam!\n");
			
			for(i = 0;i < szavazoSzam;++i){
				int rrr = rand()%1000;
				write(fd,&rrr,sizeof(rrr));
			}
		
			wait(NULL);
			close(fd);
			unlink("pipe.VAL");
			
		}else{ //ellenőr
			//sleep(1);
			printf("Ellenor: Kuldom a jelzest!\n");
			kill(getppid(),SIGUSR1);
			
			int szavazoSzam;
			fd = open("pipe.VAL", O_RDONLY);
			read(fd,&szavazoSzam,sizeof(szavazoSzam));
			printf("Ellenor: Ezt olvastam: %d\n", szavazoSzam);
			
			for(i = 0;i < szavazoSzam;++i){
				int index = i;
				index++;
				int rrr;
				read(fd,&rrr,sizeof(rrr));
				printf("Ellenor: %d. valaszto: %d\n", index, rrr);	
			}
			
		}
	
	}else{	//pecsételő

		//sleep(2);
		printf("Pecsetelo: Kuldom a jelzest!\n");
		kill(getppid(),SIGUSR1);
		
	}

	return 0;
}