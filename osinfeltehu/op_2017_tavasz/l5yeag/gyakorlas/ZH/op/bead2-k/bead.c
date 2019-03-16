#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/time.h>

int pipefd[2];
int msgQueue;
char childID[31];

char names[10][31] = {"Egyiptomi ugroeger", "Sisakos kazuar", "Rucskosfarku disznodelfin", "Sujtasos futyulolud", "Papucscsoru golya", "Csikos pampamacska", "Leveles tengeri sarkany",
						"Pisztolyos rak", "Feketemellu kanyaszsas", "Negyzethalos ajakoshal"
					 };

struct RecordMsg { 
	long mtype;
	long nyulDb;
	long nyulKor;
	long megfigyelesiIdo;
	char megfigyeloNev[31];
	char telepulesNev[31];
}; 

struct Record { 
	char megfigyeloNev[31];
	char telepulesNev[31];
	int nyulDb;
	int nyulKor;
	int megfigyelesiIdo;
}; 


int commitToFile(struct Record* records, char* filename, int size) {
    FILE* file = fopen( filename, "w" );
    if (file == 0 ) {
        return 0;
    }
    fwrite((const void*) & size,sizeof(int),1,file);
	fwrite(records, sizeof(struct Record), size, file);
    fclose(file);
    return 1;
}

void handleralarm(int signalnumber){
	const struct RecordMsg uz = { 1,rand() % 10,1,time(NULL), "telepules1",childID};
	strcpy(uz.megfigyeloNev, childID);
	strcpy(uz.telepulesNev, "telep1");
	msgsnd( msgQueue, &uz, 62*sizeof(char)+sizeof(long)*3 , 0 );

	const struct RecordMsg uz2 = { 1,rand() % 10,2,time(NULL), "telepules2",childID};
	strcpy(uz2.megfigyeloNev, childID);
	strcpy(uz2.telepulesNev, "telep2");
	msgsnd( msgQueue, &uz2, 62*sizeof(char)+sizeof(long)*3 , 0 );

	const struct RecordMsg uz3 = { 1,rand() % 10,2,time(NULL), "telepules3",childID};
	strcpy(uz3.megfigyeloNev, childID);
	strcpy(uz3.telepulesNev, "telep3");
	msgsnd( msgQueue, &uz3, 62*sizeof(char)+sizeof(long)*3 , 0 );

	kill(getppid(), SIGRTMIN);
}

void inchild() {
	
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR1);
	int val;
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	/* sigrtmin kuldese : megerkezett, majd varakozunk sigusr1-re, hog ymegjott a pipera a nev */
	kill(getppid(), SIGRTMIN);
    sigwait(&sigset, &val);

    char buff[31];
	close(pipefd[1]);
    read(pipefd[0],buff,sizeof(buff));
    printf("Azonosito megjott: %s\n", buff);
    close(pipefd[0]); 
    strcpy(childID, buff);

    /* nyugtazas */
    kill(getppid(), SIGUSR1);

    /* elkezd dolgozni, csak sigterm blokkolasa (sigterm : vege a processnek, ezt vagy alarmtol kapja meg vagy a szulotol) */
    sigdelset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGTERM);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	srand(getpid());
	signal(SIGALRM,handleralarm);
	alarm(rand() % 4+2);
	sigwait(&sigset, &val);

    printf("Megfigyelo visszahivva\n");
}

void handleEnd(int signalnumber){
	kill(getpid(), SIGTERM);
}

int main(int argc,char ** argv) {
	if (argc == 3) {
		int n = atoi(argv[1]);
		int waitingTime = atoi(argv[2]);
		int children[n];
		pid_t child;
		int j;
		int val;

		pipe(pipefd);
		int kulcs = ftok(argv[0],1); 
		msgQueue = msgget( kulcs, 0600 | IPC_CREAT ); 

		/*sigrtmin blokkolasa, ezen fognak visszajelezni a gyerekek, hogy megerkeztek-e*/
		sigset_t sigset;
		sigemptyset(&sigset);
		sigaddset(&sigset, SIGRTMIN);
		sigprocmask(SIG_BLOCK, &sigset, NULL);

		if (n > 0 && n < 11) {
			int i = 0;
			while (i < n) {
				child = fork();
				if (child > 0) {
					children[i] = child;	
					printf("Megfigyelo kikuldve\n");
					++i;
				} else {
					inchild();
					break;
				}
			}
			int arrivedCount = 0;
			if (child > 0){ 
				while(arrivedCount < n) {
				    sigwait(&sigset, &val);
				    arrivedCount++;
				    printf("Megerkezett: %d/%d\n", arrivedCount, n);
				}
				printf("Osszegyultek a megfigyelok\n");

				
				/*sigusr1-en tortenik a nevkikuldes ertesitese es az arrol valo nyugtazas
					sigrtmin pedig azert kerul bokkolasra, mert miutan megkapta a nevet egy gyerek, mar rogton kezdhet
					megfigyelni, tehat fogadni kell azokat a signalokat is*/
				sigset_t sigset2;
				sigemptyset(&sigset2);
				sigaddset(&sigset2, SIGUSR1);
				sigaddset(&sigset2, SIGRTMIN);
				sigprocmask(SIG_BLOCK, &sigset2, NULL);

				for (j = 0; j < n;++j) {
					close(pipefd[0]);
              		write(pipefd[1], names[j], strlen(names[j])+1);

					kill(children[j], SIGUSR1);

					sigwait(&sigset2, &val);
					
				}
				printf("Azonositok kikuldve\n");
				
				/*kivesszuk a blokkolasbol a sigusr1-et, mar nincs ra szukseg*/
				sigdelset(&sigset2, SIGUSR1);
				sigprocmask(SIG_BLOCK, &sigset2, NULL);
				
				/* beallitunk alarmot, majd a sigtermet is blokkoljuk a sigrtminen kivul is, ezen keresztul kapja meg, ha lejart a megfigyeles ideje */
				signal(SIGALRM,handleEnd);
				alarm(waitingTime);
				sigaddset(&sigset2, SIGTERM);
				sigprocmask(SIG_BLOCK, &sigset2, NULL);
				int vege = 0;
				int count = 0;
				int size = 0;
				struct Record* records;
				records = malloc(0);
				while(!vege) {
				    sigwait(&sigset2, &val);
				    if (val == SIGRTMIN) {
						struct RecordMsg uz; 
						int status; 
						for (j = 0; j<3;++j) {
							msgrcv(msgQueue, &uz, 62*sizeof(char)+sizeof(long)*3, 1, 0 );
							printf("Onnek 1 uj uzenete erkezett! Felado: %s\n", uz.megfigyeloNev);
							records = realloc(records, (size+1) * sizeof(struct Record));
							strcpy(records[size].megfigyeloNev, uz.megfigyeloNev);
							strcpy(records[size].telepulesNev, uz.telepulesNev);
							records[size].nyulDb = uz.nyulDb;
							records[size].nyulKor = uz.nyulKor;
							records[size].megfigyelesiIdo = uz.megfigyelesiIdo;
							++size;
						}

						count++;
					    if (count == 1) {
						    srand(time(NULL));
							kill(children[rand() % n], SIGTERM);
						}				    	
				    } else {
				    	vege = 1;
				    }
				}
				printf("Vege, maradek visszahivasa\n");
				for (j = 0; j <n; ++j) {
					kill(children[j], SIGTERM);	
				}
				commitToFile(records, "megfigyeles", size);
				printf("Megfigyelesek fajlba kiirva!");

				int status;
    			wait(&status);
			    printf("VEGE\n");
			}
		} else { 
			printf("A megfigyelok szamanak 1 es 10 koze kell esnie!");
			return;
		}
	} else {
		printf("Hibas parameterek!");
		return;
	}
	return;
}