#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <string.h> //strchr, strlen
#include <time.h> //time, asctime
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir

typedef int bool;
#define true 1
#define false 0

void maintain(char* EventID)
{
	
	int ptoc[2]; //pipe from parent to child
	int ctop[2]; //pipe from child to parent
	pipe(ptoc);
	pipe(ctop);
	char msg[100];
	pid_t child = fork();
	if (child < 0) {
		perror("Sikertelen forkolas\n");
		exit(1);
	}
	if (child > 0) { //Parent process
		close(ptoc[0]);
		close(ctop[1]);
		write(ptoc[1], EventID, sizeof(msg)); //helyszín küldése
		printf("Esemeny azonosito elkuldve.\n");
		read(ctop[0], msg, sizeof(msg)); //gyermek megérkezett
		FILE* f = fopen(EventID, "r");
		char line[100];
		fgets(line, sizeof(line), f);
		int lines = atoi(line);
		write(ptoc[1], line, sizeof(msg)); //send the number of lines in file
		{
			int i;
			for (i = 0; i < lines; ++i) { //névjegyzék küldése
				fgets(line, sizeof(line), f);
				write(ptoc[1], line, sizeof(msg));
			}
		}
		
		read(ctop[0], msg, sizeof(msg));
		printf("A kovetkezo vendegek nem jottek el:\n");
		while(strcmp(msg, "over")) {
			printf("(*) %s", msg);
			read(ctop[0], msg, sizeof(msg));
		}
		printf("\n");
		read(ctop[0], msg, sizeof(msg));
		printf("Teljes sikeresseg: %i \%\n", atoi(msg));
		
		
		close(ptoc[1]);
		close(ctop[0]);
	}
	else { //Child process //child == 0
		close(ptoc[1]);
		close(ctop[0]);
		read(ptoc[0], msg, sizeof(msg)); //helyszín megkapása
		write(ctop[1], "ok", sizeof(msg)); //megérkeztem
		printf("\tMegerkeztem a %s helyszinre. ", msg);
		read(ptoc[0], msg, sizeof(msg));
		int lines = atoi(msg);
		printf("Nevjegyzeke: (%i fo)\n", lines);
		{
			int i;
			for (i = 0; i < lines; ++i) { //névjegyzék érkezik
				read(ptoc[0], msg, sizeof(msg));
				printf("\t (*) %s", msg);
				if (rand() % 10 == 1)
					write(ctop[1], msg, sizeof(msg)); //ha nem jelent meg, visszaküldjük a nevét
			}
			printf("\n");
		}
		write(ctop[1], "over", sizeof(msg));
		int rnd = rand() % 100;
		sprintf(msg, "%ld", rnd);
		write(ctop[1], msg, sizeof(msg)); //rendvezvény sikerességének elküldése
		
		close(ptoc[0]);
		close(ctop[1]);
		exit(EXIT_SUCCESS);
	}
	
}

int main()
{	
	
	struct dirent* dp;
	DIR* d = opendir(".");
	if (d < 0) {
		perror("Nem sikerult megnyitni a mappat\n");
		exit(1);
	}
	while(dp = readdir(d)) {
		char* point;
		if((point = strrchr(dp->d_name,'.')) != NULL )
			if(strcmp(point,".event") == 0)
				maintain(dp->d_name);
	}
	
    return 0;
}