#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "types.h"

int fd[2];
int fd2[2];
char buffer[40];

void handler() {
	printf("Jelzes erkezett!\n");
}

void partner() {
	int guest_count, i, buffint;

	close(fd[1]);
	close(fd2[0]);
	guest_count = 0;
	printf("PARTNER: Partner vagyok, keszen allok!\n");
	kill(getppid(), SIGUSR1);
	while(read(fd[0], &buffer, sizeof(buffer)) && buffer) {
		printf("PARTNER: Megerkezem %s-ba, kuldok egy signalt a fonoknek!\n", buffer);
		kill(getppid(), SIGUSR1);
		read(fd[0], &guest_count, sizeof(int));
		for(i=0; i<guest_count; ++i) {
			read(fd[0], &buffer, sizeof(buffer));
		 	printf("PARTNER: %d. vendeg: %s\n", i+1, buffer);
		}
		printf("PARTNER: megjottek a vendegek, megrendezem a rendezvenyt\n");
		sleep(1);
		printf("PARTNER: Kuldom a jelentest fonok\n");
		buffint = rand() % 100;
		write(fd2[1], &buffint, sizeof(int));
		for(i=0; i<guest_count; ++i) {
			buffint = rand() % 10 == 0 ? 0 : 1;
			write(fd2[1], &buffint, sizeof(int));
		}

	}
	close(fd2[1]);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("A program hasznalata: ./kofs [adatallomany]\n");
        exit(1);
    }

	signal(SIGUSR1, handler);
	if (pipe(fd) < 0) { printf("Nem sikerult a forkolas!");exit(EXIT_FAILURE);} 
	if (pipe(fd2) < 0) { printf("Nem sikerult a forkolas!");exit(EXIT_FAILURE);} 
	
	printf("\n\nRENDEZVENYSZERVEZO:Hivom a partneremet\n");

	pid_t child=fork();

	if(child < 0) { printf("Nem sikerult a forkolas!\n");exit(EXIT_FAILURE);}

	if (child > 0) {
	    vector events;
	    FILE* f;
	    char line[160];
	    Event* tempevent;
	    Guest* tempguest;
	    events = vector_init();
	    char linetype;
    	pause();

    	close(fd[0]);
    	close(fd2[1]);
	    printf("RENDEZVENYSZERVEZO:Betoltom a %s file-bol az adatokat!\n", argv[1]);
	    f = fopen(argv[1], "r");
	    if (f == NULL) {printf("A file nem erheto el.\n\n");exit(EXIT_FAILURE);}
	
	    while(fgets(line, sizeof(line), f)) {
	            sscanf(line, "%c *[^\n]", &linetype);
	            switch (linetype) {
	                case 'E':
	                    tempevent = event_init();
	                    sscanf(line,"%*c %s %s", tempevent->name, tempevent->location);
	                    vector_add(&events, tempevent);
	                    break;
	                case 'G':
	                    tempguest = guest_init();
	                    sscanf(line,"%*c %s %s %[^\t\n]", tempguest->name, tempguest->mail,
	                            tempguest->entry_date);
	                    vector_add(&tempevent->guests, tempguest);
	                    break;
	            }
	        }
	    fclose(f);

		printf("RENDEZVENYSZERVEZO: Kuldom a cimeket, kollega\n");
	

		int i,j, buffint;
		for (i=0; i<events.count; ++i) {	
			tempevent = vector_get(&events,i);
			printf("------------------------------------------------\n"
				"RENDEZVENYSZERVEZO: Kuldok egy cimet, tartsd meg a %s rendezvenyt\n", tempevent->name);
			write(fd[1], tempevent->location, sizeof(buffer));
			pause();
			printf("RENDEZVENYSZERVEZO: Latom megerkeztel, kuldom a vendeglistat.\n");
			write(fd[1], &tempevent->guests.count, sizeof(int));
	        for (j=0; j<tempevent->guests.count; ++j) {
				tempguest = vector_get(&tempevent->guests, j);
				write(fd[1], tempguest->name, sizeof(buffer));
			}

			read(fd2[0], &buffint, sizeof(int));
			printf("RENDEZVENYSZERVEZO: Lassuk a jelentest\nRENDEZVENYSZERVEZO: Siker: %d%% ", buffint);
			printf("%s\n", buffint > 50 ? "Nem rossz, kollega" : "Ez bizony eleg alacsony..." );
	        for (j=0; j<tempevent->guests.count; ++j) {
				read(fd2[0], &buffint, sizeof(int));
				if (!buffint) {
					tempguest = vector_get(&tempevent->guests, j);
					printf("RENDEZVENYSZERVEZO: Nem jelent meg: %s\n", tempguest->name);	
				}
	        }

		}
		close(fd[1]);
		wait(NULL);

	    for (i=0; i<events.count; i++) {
       		tempevent = vector_get(&events, i);
	      	vector_empty(&tempevent->guests);
	    }

	    vector_empty(&events);
		return 0;
	} 
	else {
		//GYERMEK
		partner();


	}

}
