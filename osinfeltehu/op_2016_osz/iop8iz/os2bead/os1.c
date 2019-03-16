#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>  //fork, pipe
#include <sys/wait.h> //waitpid
#include <sys/time.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <wait.h>
#include <dirent.h>
#include <linux/limits.h>


struct jelent{
	char name[50];
	char email[50];
	time_t date;
};

struct valasz{
	int sikeresseg;
	struct jelent nem_jelent_meg[50];
	char number_nem_jelent_meg;
};

void createEvent(char* eventName);
void openEvent(char* eventName);
void deleteEvent(char* eventName);
void listName(char* eventName);
void runEvent(char* fname);
char parentFunc(int pipefd[2], int queue_id, char* fname);
void childFunc(int pipefd[2], int queue_id);
char getNextEvent(struct jelent jel[50], char* fname, char delete_filename[PATH_MAX]);

int main(int argc, char* argv[]){
	char event;
	char eventName[100];
	mkdir("test", 0777);
	printf("1 esemény létrehozása\n2 esemény megnyitása\n3 esemény törlése\n4 esemény futtatása\n");
	scanf("%c", &event);
	if(event == '1'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		char filepath[PATH_MAX];
		strcpy(filepath, "./test/");
		strcpy(filepath+7, eventName);
		createEvent(filepath);
	}
	else if(event == '2'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		char filepath[PATH_MAX];
		strcpy(filepath, "./test/");
		strcpy(filepath+7, eventName);
		openEvent(filepath);
	}
	else if(event == '3'){
		printf("Adja meg az esemény nevét:\n");
		scanf("%s", eventName);
		char filepath[PATH_MAX];
		strcpy(filepath, "./test/");
		strcpy(filepath+7, eventName);
		deleteEvent(filepath);
	}
	else if(event == '4'){
		runEvent(argv[0]);
	} else {
        	printf("Nem jó karakter!\n");
	}
}

void runEvent(char* fname){
	char siker = 1;
	while(siker > 0){
		int pipefd[2];
		key_t kulcs;
	
		int queue_id;
		kulcs = ftok(fname,1); 
	  	queue_id = msgget( kulcs, 0600 | IPC_CREAT ); 
	  	if ( queue_id < 0 )
	  	{ 
	  		perror("msgget");
	  	}

		signal(SIGTERM,SIG_IGN);

	  	if (pipe(pipefd) == -1) 
	  	{
	    		perror("Pipe opening error");
	    		exit(EXIT_FAILURE);
	  	}
		pid_t  child=fork();
	  	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
		if (child>0){
			siker = parentFunc(pipefd,queue_id, fname);
			kill(child,SIGTERM);
		} else {
			childFunc(pipefd,queue_id);
		}
	}
}

char getNextEvent(struct jelent jel[50], char* fname, char delete_filename[PATH_MAX]){
	int file;
	DIR           *d;
  	struct dirent *dir;
  	d = opendir("./test");
	char run = 0;
  	if (d)
  	{
  	  while ((dir = readdir(d)) != NULL)
  	  {
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, fname) == 0){
		continue;		
		}
		char filepath[PATH_MAX];
		strcpy(filepath, "./test/");
		strcpy(filepath+7, dir->d_name);
		file = open(filepath, O_RDONLY);
		run = 1;
		strcpy(delete_filename, filepath);
  	  }

  	  closedir(d);
  	} else {
		printf("Nem tudom megnyitni");
	}
	if(!run){
		return -1;
	}
	char buff;
	read(file, &buff, 1);
	for(int i = 0; i < buff; ++i){
		struct jelent tmp;
		read(file, &tmp, sizeof(struct jelent));
		jel[i] = tmp;
	}
	close(file);
	return buff;
}

int generateRandomBetween1And10()
{
	int file = open("/dev/random", O_RDONLY);
	char buff;
	read(file, &buff, 1);
	close(file);
  	return abs(buff)%10+1;
}

struct valasz lebonyolit(struct jelent jel[50], char number){
	int sikeresseg = generateRandomBetween1And10();
	struct valasz lista;
	lista.sikeresseg = sikeresseg;
	int valasz_number = 0;
	for(int i = 0; i < number; ++i){
		int rand = generateRandomBetween1And10();
		if(rand == 1){
			lista.nem_jelent_meg[valasz_number] = jel[i];
			++valasz_number;
		}
	}
	lista.number_nem_jelent_meg = valasz_number;
	return lista;
}

struct uzenet { 
     long mtype;
     struct valasz mtext; 
};

char parentFunc(int pipefd[2], int queue_id, char* fname){
	close(pipefd[0]);
	struct jelent emberek[50];
	char delete_filename[PATH_MAX];
	char jelentNumber = getNextEvent(emberek, fname, delete_filename);
	if(jelentNumber < 0){
		return -1;
	}
	for(int i = 0; i < jelentNumber; ++i){
		char name[30];
		write(pipefd[1], &emberek[i], sizeof(struct jelent));
	}
	fflush(stdout);
	close(pipefd[1]);
	struct uzenet valasz;
	int status; 
     	status = msgrcv(queue_id, &valasz, sizeof(valasz.mtext), 5, 0 ); 
     	if ( status < 0 ) 
          perror("msgsnd");
	printf("Sikeresség: %i\n", valasz.mtext.sikeresseg);
	if(valasz.mtext.number_nem_jelent_meg != 0){
		printf("Akik nem jelentek meg:\n");
	}
	for(int i = 0; i < valasz.mtext.number_nem_jelent_meg; ++i){
		printf("Név: %s , E-mail: %s\n", valasz.mtext.nem_jelent_meg[i].name, valasz.mtext.nem_jelent_meg[i].email); 
	}
	remove(delete_filename);
	sleep(1);
	return 1;
}


void childFunc(int pipefd[2], int queue_id){
	srand(time(NULL));
	int jelentNumber = 0;
	struct jelent names[50];
	close(pipefd[1]);
	while(read(pipefd[0], &names[jelentNumber], sizeof(struct jelent))){
		++jelentNumber;
	}
	fflush(stdout);
	close(pipefd[0]);
	for(int i = 0; i < jelentNumber; ++i){
		printf("Név: %s, E-mail: %s\n", names[i].name, names[i].email);
	}
	fflush(stdout);
	struct uzenet uz;
	uz.mtype = 5;
	uz.mtext = lebonyolit(names, jelentNumber);

	int status; 
     
     	status = msgsnd( queue_id, &uz, sizeof(uz.mtext) , 0 ); 
     	if ( status < 0 ) 
        	perror("msgsnd");
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
	printf("A sorszáma: %d\n", buff);
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
		printf("Név: %s, E-mail: %s\n", tmp.name, tmp.email);
	}
}

void deleteEvent(char* eventName){
    if(remove(eventName) == -1)
    {
        perror("File handling error\n");
    }
}

