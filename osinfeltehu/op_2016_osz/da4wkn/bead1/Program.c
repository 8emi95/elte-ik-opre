#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <string.h> //strchr, strlen
#include <time.h> //time, asctime

typedef int bool;
#define true 1
#define false 0

void maintain(char* EventID)
{
	srand(time(NULL));
	
	int ptoc[2]; //pipe from parent to child
	int ctop[2]; //pipe from child to parent
	pipe(ptoc);
	pipe(ctop);
	char msg[100];
	pid_t child = frok();
	if (child < 0) {
		perror("Sikertelen forkolas\n");
		exit(1);
	}
	if (child > 0) { //Parent process
		close(ptoc[0]);
		close(ctop[1]);
		write(ptoc[1], &EventID, sizeof(&EventID)); //helyszín küldése
		read(ctop[0], &msg, sizeof(msg)); //gyermek megérkezett
		FILE* f = fopen(EventID, "r");
		char line[100];
		fgets(line, sizeof(line), f);
		int lines = atoi(line);
		write(ptoc, &lines, sizeof(int)); //send the number of lines in file
		{
			int i;
			for (i = 0; i < lines; ++i) { //névjegyzék küldése
				fgets(line, sizeof(line), f);
				write(ptoc, &line, sizeof(line));
			}
		}
		
		read(ctop[0], &msg, sizeof(msg));
		printf("A kovetkezo vendegek nem jottek el:\n");
		while(strcmp(msg, "over")) {
			read(ctop[0] &msg, sizeof(msg));
			printf("\t %s \n", msg);
		}
		read(ctop[0] &msg, sizeof(msg));
		printf("Teljes sikeresseg: %i\n", atoi(msg));
	}
	else { //Child process //child == 0
		close(ptoc[1]);
		close(ctop[2]);
		read(ptoc[0], &msg, sizeof(msg)); //helyszín megkapása
		write(ctop[1], "ok", strlen("ok")); //megérkeztem
		printf("<< %s >> nevjegyzeke: \n", EventID);
		int lines;
		read(ptoc[0], &lines, sizeof(int));
		{
			int i;
			for (i = 0; i < lines ++i) { //névjegyzék érkezik
				read(ptoc, &msg, sizeof(msg));
				printf("\t %s \n", msg);
				if (rand() % 10 == 1)
					write(ctop[1], &msg, sizeof(msg)); //ha nem jelent meg, visszaküldjük a nevét
			}
		}
		write(ctop[1], "over", strlen("over"));
		write(ctop[1], rand() % 100, sizeof(int)); //rendvezvény sikerességének elküldése
		
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
		const char* dot = strrchr(dp->d_name, ".");
		if (strcmp((dot+1), "event")) 
			maintain(dp->d_name);
	}
	
    return 0;
}