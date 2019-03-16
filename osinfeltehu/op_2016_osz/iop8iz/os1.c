#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>

struct jelent{
	char name[50];
	char email[50];
	time_t date;
};

void createEvent(char* eventName);
void openEvent(char* eventName);
void deleteEvent(char* eventName);
void listName(char* eventName);

int main(){
	char event;
	char eventName[100];
	printf("1 esemény létrehozása\n2 esemény megnyitása\n3 esemény törlése\n");
	scanf("%c", &event);
	if(event == '1'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		createEvent(eventName);
	}
	else if(event == '2'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		openEvent(eventName);
	}
	else if(event == '3'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		deleteEvent(eventName);
	} else {
        printf("Nem jó karakter!");
	}
}

void createEvent(char* eventName){
	int file = open(eventName, O_WRONLY | O_CREAT);
	chmod( eventName, 0666 );
	if (file<0){perror("File opening error\n"); exit(1);}
	write(file, "\0", 1);
	close(file);
}

void openEvent(char* eventName){
	printf("1 jelentkezés\n2 listázás\n3 módosítás\n4 törlés\n");
	char event;
	scanf("%s", &event);
	if(event == '1'){
        int file = open(eventName, O_RDWR | O_APPEND);
        chmod( eventName, 0666 );
        if (file<0){perror("File opening error\n"); exit(1);}
        struct jelent tmp;
        printf("Adja meg a nevét:\n");
        scanf("%s", tmp.name);
        printf("Adja meg az e-mail címét:\n");
        scanf("%s", tmp.email);
        time(&tmp.date);
        write(file, &tmp, sizeof(struct jelent));
        close(file);
        file = open(eventName, O_RDWR);
        char buff;
        read(file, &buff, 1);
        ++buff;
	printf("A sorszáma: %d", buff);
        lseek(file, 0, SEEK_SET);
        write(file, &buff, 1);
        close(file);
	}
	else if(event == '2'){
		listName(eventName);
	}
	else if(event == '3'){
        int file = open(eventName, O_RDWR);
        chmod( eventName, 0666 );
        if (file<0){perror("File opening error\n"); exit(1);}
        printf("Melyik jelentkezőt szeretné módosítani?\n");
        char n;
        fsync(file);
        scanf("%d", &n);
        struct jelent tmp;
        printf("Adja meg az uj nevét:\n");
        scanf("%s", tmp.name);
        printf("Adja meg az uj e-mail címét:\n");
        scanf("%s", tmp.email);
        time(&tmp.date);
        lseek(file, 1+(n*sizeof(struct jelent)), SEEK_SET);
        write(file, &tmp, sizeof(struct jelent));
        close(file);
	}
	else if(event == '4'){
        int originalFile = open(eventName, O_RDONLY);
        chmod( eventName, 0666 );
        if (originalFile<0){perror("File opening error\n"); exit(1);}
        printf("Melyik jelentkezőt szeretné törölni?\n");
        char n;
        fsync(originalFile);
        scanf("%s", &n);
        int newFile = open("temp",O_WRONLY|O_CREAT);
        if(newFile == -1)
        {
            perror("File handling error\n");
        }
        chmod( eventName, 0666 );

        int number = 0;
        struct jelent currentJelent;
        char buff;
        read(originalFile, &buff, 1);
        --buff;
        write(newFile, &buff, 1);
        while(number < n)
        {
            read(originalFile, &currentJelent, sizeof(struct jelent));
            write(newFile, &currentJelent, sizeof(struct jelent));
            ++number;
        }

        lseek(originalFile, 1+((n+1)*sizeof(struct jelent)), SEEK_SET);

        while(read(originalFile, &currentJelent, sizeof(struct jelent)))
        {
            write(newFile, &currentJelent, sizeof(struct jelent));
        }

        close(originalFile);
        close(newFile);

        if(remove(eventName) == -1)
        {
            perror("File handling error\n");
        }
        rename("temp", eventName);
	}
	else {
        printf("Nem jó karakter!");
	}
}

void listName(char* eventName){
	int file = open(eventName,O_RDONLY);
	char buff;
	read(file, &buff, 1);
	for(int i = 0; i < buff; ++i){
		struct jelent tmp;
		read(file, &tmp, sizeof(struct jelent));
		printf("Name: %s, E-mail: %s\n", tmp.name, tmp.email);
	}
}

void deleteEvent(char* eventName){
    if(remove(eventName) == -1)
    {
        perror("File handling error\n");
    }
}

