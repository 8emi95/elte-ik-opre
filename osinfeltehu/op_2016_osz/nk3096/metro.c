//Pintér Arianna (NK3096) Operációs rendszerek gépes zh

//#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define KATASZTROFA_ESELY 10 //10-nel vagy az alatt siklik ki a metro

struct Data {
	long mtype;
	int szerelvenyek;
	int hanyadik;
};

struct Report {
	long mtype;
	short sikeres;
	char esemeny[100];
};

int getRandomNumber(int max) {
	return rand() % max + 1;
}

int mqSend(int mq, struct Data d) { 
    int status;
    status = msgsnd( mq, &d, sizeof(d) , 0 );
    if ( status < 0 ) perror("Message send error");
    return 0;
} 

int mqReceive(int mq, struct Data* d) { 
    int status; 
    status = msgrcv(mq, d, sizeof(*d), 0, 0 );
    if ( status < 0 ) perror("Message receive error"); 
	return 0;
}

int mqSendReport(int mq, struct Report r) {
	int status;
    status = msgsnd( mq, &r, sizeof(r) , 0 );
    if ( status < 0 ) perror("Message send error");
    return 0;
}

int mqReceiveReport(int mq, struct Report* r) { 
    int status; 
    status = msgrcv(mq, r, sizeof(*r), 0, 0 );
    if ( status < 0 ) perror("Message receive error"); 
	return 0;
}

void handler(int signumber){
	printf("[METRO] Megerkeztem a vegallomasra, kuldom az adatokat!\n");
}

int main(int argc, char** argv) {
	srand(time(NULL));

	if(argc > 1) {
		char* p = argv[1];

	    while (*p != '\0') {
		    if (*p < '0' || *p > '9') {
		        printf("Nem megfelelo parameter!\n");
		        return 0;
		    }
		    p++;
	    }
	}
	else { printf("Kerem adjon meg egy szamot parameterkent!\n"); return 0; }

	////////

	int messageQueue, status;
	key_t key;
	key = ftok(argv[0], 2);
	messageQueue = msgget( key, 0600 | IPC_CREAT );
	if(messageQueue < 0) { perror("MSGGET error"); return 1; }
	pid_t pid;
	signal(SIGUSR1, handler);
	int fd[2]; //pipe

	pid = fork();

	if(pid < 0) { perror("Hiba tortent a fork kozben!"); return 1; }
	else if(pid > 0) {
//szulo
		int szerelvenyek = atoi(argv[1]);
		int hanyadik = getRandomNumber(szerelvenyek);

		struct Data d = { 1, szerelvenyek, hanyadik };
		printf("[BMF] Kuldom az adatokat!\n");
		mqSend(messageQueue, d);

		pause(); //megvarjuk a gyerek jelet

		struct Report r;
		mqReceiveReport(messageQueue, &r);
		printf("[BMF] A kovetkezot kaptam: ");
		if(r.sikeres == 1) printf("az ut sikeres volt!\n");
		else if(strcmp(r.esemeny,"banan")==0) printf(" a metro elcsuszott egy bananhejon.\n");
		else if(strcmp(r.esemeny,"fagy")==0) printf(" a metro lefagyott a zord telben.\n");
		else printf(" varatlan hiba tortent.\n");

		close(fd[1]);

		//megvarjuk, amig befejezodik a gyerek
		int status;
		waitpid(pid, &status, 0);
		//message queue bezarasa
		status = msgctl( messageQueue, IPC_RMID, NULL ); 
		if ( status < 0 ) perror("msgctl"); 

	} else {

//gyerek
		struct Data d;
		mqReceive(messageQueue, &d);
		if(d.hanyadik != d.szerelvenyek)
			printf("[METRO] En a(z) %d. szerelveny vagyok ma, meg %d db jon utanam!\n", d.hanyadik, d.szerelvenyek-d.hanyadik);
		else printf("[METRO] En a(z) %d. szerelveny vagyok ma, en vagyok az utolso!\n", d.hanyadik);

		short sikeres = 1;
		char esemeny[10] = "";

		int katasztrofa = getRandomNumber(101);
		if(katasztrofa <= KATASZTROFA_ESELY) {
			
			printf("[METRO] Valami baj tortent! \n");
			sikeres = 0;
			if(katasztrofa % 2 == 0) {
				strcpy(esemeny, "banan");
			} else strcpy(esemeny, "fagy");

		} else printf("[METRO] Minden rendben!\n");

		struct Report r = { 2, sikeres, "" };
		strcpy(r.esemeny, esemeny);

		kill(getppid(), SIGUSR1);
		mqSendReport(messageQueue, r);

		close(fd[0]);
		close(fd[1]);
	}

	return 0;
}










