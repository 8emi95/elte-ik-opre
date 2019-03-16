#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

const static char* FIFO = "fifo.tmp";
const static int LINE = 81;
const static int MSG_LEN = 1024;
const static char* mkfifoerr = "MKFIFO ERROR";
const static char* forkerr = "FORK ERROR";

struct uzenet {
	long _kat;
	char _uz[1024];
};

void kozpont(const int, const pid_t);

void taxis(const int);

void utas(const char*);

void main(int argc, char* argv[]) {
	if(argc > 1) {
		if(access(FIFO, F_OK) != -1)
			unlink(FIFO);
		
		int f = mkfifo(FIFO, S_IRUSR | S_IWUSR);
		
		key_t kulcs = ftok(argv[0], 1);
		
		if(f != -1) {
			pid_t u = fork();
			if(u > 0) {
				int msgid = msgget(kulcs, 0600 | IPC_CREAT);
				pid_t t = fork();
				if(t > 0) {
					kozpont(msgid, u);
					waitpid(t, NULL, 0);
					waitpid(u, NULL, 0);
					unlink(FIFO);
					msgctl(msgid, IPC_RMID, NULL);
					//printf("KOZPONT VEGE\n");
				} else if(t == 0) {
					taxis(msgid);
					//printf("TAXIS VEGE\n");
				} else {
					printf("%s TAXIS\n", forkerr);
					exit(1);
				}
			} else if(u == 0) {
				utas(argv[1]);
				//printf("UTAS VEGE\n");
			} else {
				printf("%s UTAS\n", forkerr);
				exit(1);
			}
		} else {
			printf("%s\n", mkfifoerr);
		}
	}
}

void kozpont(const int msgid, const pid_t u) {
	int f;
	char lakcim[LINE];
	
	//lakcim olvas
	f = open(FIFO, O_RDONLY);
	read(f, lakcim, LINE);
	printf("KOZPONT: %s\n", lakcim);
	close(f);
	
	//lakcim tovabbkuld
	struct uzenet uz = { 1, "" };
	char u_str[10];
	strcpy(uz._uz, lakcim);
	strcat(uz._uz, " ");
	sprintf(u_str, "%d", u);
	strcat(uz._uz, u_str);
	msgsnd(msgid, &uz, strlen(uz._uz) + 1, 0);
	
	//visszaigazolas olvas
	msgrcv(msgid, &uz, MSG_LEN, 0, 0);
	printf("KOZPONT: \"TAXIS: %s\"\n", uz._uz);
}

void taxis(const int msgid) {
	struct uzenet uz;
	msgrcv(msgid, &uz, MSG_LEN, 0, 0);
	printf("TAXIS: %s\n", uz._uz);
	
	strcpy(uz._uz, "VETTEM");
	msgsnd(msgid, &uz, strlen(uz._uz) + 1, 0);
}

void utas(const char* lakcim) {
	int f;
	
	//lakcim elkuld
	f = open(FIFO, O_WRONLY);
	write(f, lakcim, strlen(lakcim) + 1);
	close(f);
}