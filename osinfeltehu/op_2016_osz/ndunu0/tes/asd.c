#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <signal.h>

struct passenger {
    char* name;
};

struct flight {
    char* name;
    char* start_time;
    struct passenger passangers[10];
    int count_passenger;
};

struct msg {
	long mtype;
	char mtext[1000];
};


int send_msg(int msg, char* msg_text) {
	int status = 0;
	struct msg message = {1, ""};
	strcat(message.mtext, msg_text);
	status = msgsnd(msg, &message, strlen(message.mtext) + 1, 0);
	if(status < 0) {
		perror("Message send error!");
	}
	return 0;
}

int recieve(int msg) {
	struct msg message;
	int status = msgrcv(msg, &message, 1000, 1, 0);
	
	if(status < 0) {
		perror("Message recieve error!");
	} else {
		printf("%s\n", message.mtext);
	}
	return 0;
}

void signal_handler(int sing_number) {
	printf("\nSignal with number %i has arrived\n", sing_number);
}

void list_flights(struct flight* f, int actual_size) {
    if(actual_size != 0) {
        int i;
        for(i = 0; i < actual_size; i++) {
            printf("\t %d. %s -- %s\n", i, f[i].name, f[i].start_time);
        }
        printf("\n");
     }
}


int main(int argc, char* argv[]) {
    int pipefd[2];
	int message, st;
	char p[100] = "";
	char marks[100];
	char select_op = ' ';
	pid_t  pid_id;
	time_t t;
	key_t key;
	srand((unsigned) time(&t));
	
	//Start trip
	if(pipe(pipefd) == -1) {
		perror("Pipe open error!");
		return 1;
	}
	
	key = ftok(argv[0], 1);
	message = msgget(key, 0666  | IPC_CREAT);
	if(message < 0) {
		perror("Message get error ");
		return 1;
	}
	
	signal(SIGTERM, signal_handler);
	pid_id = fork();
	if(pid_id == -1) {
		perror("Fork error!");
		return 1;
	}
	
	struct flight flights[3];
	//járatok
	flights[0].name = "Krakko";
    flights[0].start_time = "10:00";
    flights[1].name = "Praga";
    flights[1].start_time = "12:00";
    flights[2].name = "Becs";
    flights[2].start_time = "14:00";
    
    //utasok
    flights[0].passangers[0].name = "Gergo";
	flights[0].passangers[1].name = "Andras";
	flights[0].passangers[2].name = "Istvan";
	flights[0].count_passenger = 3;
	flights[1].passangers[0].name = "Endre";
	flights[1].passangers[1].name = "Marton";
	flights[1].count_passenger = 2;
	flights[2].passangers[0].name = "Zoltan";
	flights[2].passangers[1].name = "Laszlo";
	flights[2].passangers[2].name = "Elek";
	flights[2].count_passenger = 3;

	int actual_size = 3;
	
	if(pid_id > 0) { //parent process
		printf("Select the road!\n");
		list_flights(flights, 3);
		char selected_op;
		fflush(stdin);
		printf("Line number: ");
		scanf("%s", &selected_op);
		int selected_line = strtol(&selected_op, NULL, 10);
		if(selected_line < actual_size) {
		 	char pas_count[200];
			sprintf(pas_count, "%d", flights[selected_line].count_passenger);
			write(pipefd[1], pas_count, strlen(pas_count));
			write(pipefd[1], ", ", strlen(", "));
			int i;
			for(i = 0; i < flights[selected_line].count_passenger; i++) {
				write(pipefd[1], flights[selected_line].passangers[i].name, strlen(flights[selected_line].passangers[i].name));
				write(pipefd[1], ", ", strlen(", "));
			}
			
			close(pipefd[1]);
		} else {
			printf("Wrong input (%c) \n", select_op);
			exit(1);
		}
		printf("The journey has begun! (parent process)\n");
		//értékelések amit fogadok
		fflush(NULL);
		
		int j;
		for(j = 0; j < flights[selected_line].count_passenger; j++) {
			printf("%s: ", flights[selected_line].passangers[j].name);
			recieve(message);
		}
		
		pause();
		printf("Completed a tour guide\n");
		wait();
	} else { //child process
		//get passangers
		read(pipefd[0], &p, sizeof(p));
		printf("Trip participants: %s (child)\n", p);
		char str_count[20] = "";
		int k;
		for(k = 0; k < strlen(p); k++) {
			if(p[k] == ',') {
				break;
			}
			str_count[k] = p[k];
		}
		int pas_count = strtol(str_count, NULL, 10);
		close(pipefd[0]);
		fflush(NULL);
		
		//send_msg
		int j = 0;
		for(j; j < pas_count; j++) {
			int value = 1 + rand() % 5;
			char str_value[5];
			sprintf(str_value, "%d", value);
			send_msg(message, str_value);
		}
		
		sleep(1);
		kill(getppid(), SIGTERM); //elküldöm, hogy megáltam
	}
    return 0;
}
