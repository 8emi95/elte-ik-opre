#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

struct Jelentkezo {
    char nev[80];
    char email[80];
    time_t jelentkezesiIdo;
};

void getEvents () {
    DIR* directory = opendir ("adatok");
    if (directory == NULL) {
	printf("Hiba az adatok mappa megnyitása során!!! %s\n", strerror(errno));
	exit(1);
    }
    errno = 0;
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
	    continue;
	}
	printf("%s\n", entry->d_name);
    }
    if ( errno != 0 ) {
	printf("Hiba az adatok olvasása során!!! %s\n", strerror(errno));
	exit(1);
    }
    closedir(directory);
}

void eleresKeszito(char * eleres, const char * rendezvenyAzon) {
    strcpy(eleres, "adatok/");
    strcat(eleres, rendezvenyAzon);
}

int letezikERendezveny(const char * eleres){
    return access(eleres, F_OK);
}

off_t fileMeret(const char * eleres) {
    struct stat st;
    
    if (stat(eleres, &st) == 0) {
	return st.st_size;
    } else {
	return -1;
    }
}

void vendegekOlvasasa(struct Jelentkezo jelentkezok[], char * helyszin, const char * eleres){
    int fd = open(eleres, O_RDONLY);
    int i = 0;
    struct tm * timeinfo;
    do
    {
		read(fd, jelentkezok + i, sizeof(struct Jelentkezo));
		timeinfo = localtime(&jelentkezok[i].jelentkezesiIdo);
		printf("%d %s %s %s\n", (i+1), jelentkezok[i].nev, jelentkezok[i].email, asctime(timeinfo));
		i++;
    } while (i < (fileMeret(eleres) / sizeof(struct Jelentkezo)));
    close(fd);
}

main () { 
    printf("Válaszható rendezvények azonosítók szerint:\n");
    getEvents();
    printf("Melyik rendezvény következik?\n");
    char rendezvenyAzon[80];
    char helyszin[80];
    scanf("%s", rendezvenyAzon);
    char eleres[90];
    eleresKeszito(eleres, rendezvenyAzon);
    if (letezikERendezveny(eleres) == -1) {
	printf("Nem létezik ilyen azonosítójú rendezvény!!!\n");
	exit(1);
    }
    id_t pid;
    int pipefd[2];
    if(pipe(pipefd) == -1) {
	printf("pipe hiba");
	exit(1);
    }
    if ((pid = fork()) == -1) {
	printf("fork hiba");
	exit(1);
    } else if(pid == 0) {
	int i = 1;
	struct Jelentkezo * jelentkezok = malloc(sizeof(struct Jelentkezo));
	struct Jelentkezo ember;
	while(read(pipefd[0], &ember, sizeof(struct Jelentkezo)) != 0) {
	   // printf("%d\n", i);
	    i++;
	    jelentkezok = realloc(jelentkezok, sizeof(struct Jelentkezo) * i);
	    //printf("%d %s %s ", (i+1), jelentkezok[i].nev, jelentkezok[i].email);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	_exit(0);
    } else {
	struct Jelentkezo jelentkezok[fileMeret(eleres) / sizeof(struct Jelentkezo)];
	vendegekOlvasasa(jelentkezok, helyszin, eleres);
	write(pipefd[1], &jelentkezok, sizeof(jelentkezok));
	close(pipefd[0]);
	close(pipefd[1]);
	wait(NULL);
	exit(0);
    }
    exit(0);
}