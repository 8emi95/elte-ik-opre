#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


//Somodi Gábor Soma
//JJ9VIJ
//King of Stands beadandó - Operációs rendszerek
//2016.11.07.

void handler(int signumber) { }

//struct to store guest data
struct Guests {
	char name[50];
	char mail[50];
	char event_id[10];
	time_t time;
} guest;

//guest functions
void add_guest();
void mod_guest();
void del_guest();
void list_guests();

//event functions
void new_event();
void del_event();
void perform_event();

//menu
void run_menu();

//implementations

//guest
void add_guest() {
	FILE * fp;
	printf("Enter name: \n");
	scanf("%50s", guest.name);
	printf("Enter e-mail: \n");
	scanf("%50s", guest.mail);
	printf("Enter event ID: \n");
	scanf("%10s", guest.event_id);
	time(&guest.time);
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	int num = 1;
	char temp[512];
	
	fp = fopen(buf, "r");
	while(fgets(temp, 512, fp) != NULL) {
			num++;
	}
	fp = fopen(buf, "a");
	fprintf(fp, "%s %s %s %li\n", guest.name, guest.mail, guest.event_id, guest.time);
	
	printf("%s is the %i. guest in the event.\n", guest.name, num);
	
	fclose(fp);
}

void mod_guest() {
	FILE * fp;
	FILE * tf;
	tf = fopen("tmp.txt","w");
	printf("Enter event ID: \n");
	scanf("%10s", guest.event_id);
	
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	fp = fopen(buf, "r+");
	
	if((fp = fopen(buf, "r+")) == NULL) {
		printf("\nFile does not exist.\n");
		exit(1);
	}
	
	char old_name[50];
	printf("Enter name to modify: \n");
	scanf("%50s", old_name);
	
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	int opt;

	while(fgets(temp, 512, fp) != NULL) {
		sscanf(temp,"%s %s %s %li", guest.name, guest.mail, guest.event_id, &guest.time);
		if((strstr(guest.name, old_name)) != NULL) {
			printf("A match found on line: %d\n", line_num);
			printf("\n%s\n", temp);
			find_result++;
			
			printf("\nModify: \n[1]Name, [2]mail, [3]cancel or remove(any other key)\n");
			scanf("%d", &opt);
			switch (opt) {
			case 1:
				scanf("%50s", guest.name);
				fprintf(tf, "%s %s %s %li\n", guest.name, guest.mail, guest.event_id,
				guest.time);
				break;
			case 2:
				scanf("%50s", guest.mail);
				fprintf(tf, "%s %s %s %li\n", guest.name, guest.mail, guest.event_id,
				guest.time);
				break;
			case 3: 
				fputs(temp,tf);
				break;
			default:
				printf("Wrong input, nothing changed.\n");
			}
		}
		else {
			fputs(temp,tf);
		}
		line_num++;
	}

	if(find_result == 0) {
		printf("\nCouldn't find a match.\n");
	}
	
	fclose(fp);
	fclose(tf);
	remove(buf);
	rename("tmp.txt", buf);
}

void del_guest() {
	FILE * fp;
	FILE * tf;
	tf = fopen("tmp.txt","w");
	printf("Enter event ID: \n");
	scanf("%10s", guest.event_id);
	
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	fp = fopen(buf, "r+");
	
	if((fp = fopen(buf, "r+")) == NULL) {
		printf("\nFile does not exist.\n");
		exit(1);
	}
	
	char old_name[50];
	printf("Enter name to delete: \n");
	scanf("%50s", old_name);
	
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	int opt;

	while(fgets(temp, 512, fp) != NULL) {
		sscanf(temp,"%s %s %s %li", guest.name, guest.mail, guest.event_id, &guest.time);
		if((strstr(guest.name, old_name)) != NULL) {
			printf("A match found on line: %d\n", line_num);
			printf("\n%s\n", temp);
			find_result++;
			
			printf("\n[1]Delete or search next(any other key)\n");
			scanf("%d", &opt);
			switch (opt) {
			case 1:
				break;
			default:
				fputs(temp,tf);
			}
		}
		else {
			fputs(temp,tf);
		}
		line_num++;
	}

	if(find_result == 0) {
		printf("\nCouldn't find a match.\n");
	}
	
	fclose(fp);
	fclose(tf);
	remove(buf);
	rename("tmp.txt", buf);
}

void list_guests() {
	FILE * fp;
	printf("Enter event ID to list guests: \n");
	scanf("%10s", guest.event_id);
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	fp = fopen(buf, "r");
	
	if((fp = fopen(buf, "r")) == NULL) {
		printf("\nFile does not exist.\n");
		exit(1);
	}
	
	int line_num = 1;
	char temp[512];

	printf("\nList of guests for %s event:\n", guest.event_id);
	while(fgets(temp, 512, fp) != NULL) {
		sscanf(temp,"%s %s %s %li", guest.name, guest.mail, guest.event_id, &guest.time);
		if((strstr(guest.name, "")) != 0) {
			printf("Line %i: Name: %s, E-mail: %s, Registration time: %s", line_num, guest.name, guest.mail, ctime(&guest.time));
		}
		line_num++;
	}

	fclose(fp);
}

//event functions
void new_event() {
	FILE * fp;
	printf("\nAdd new event: \n");
	scanf("%10s", guest.event_id);
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	fp = fopen(buf, "a+");

	fclose(fp);
	printf("\nEvent created.\n");
}

void del_event() {
	FILE * fp;
	printf("\nAdd event name to delete: \n");
	scanf("%10s", guest.event_id);
	char buf[14];
	snprintf(buf, sizeof(buf), "%s.txt", guest.event_id);
	
	remove(buf);
	printf("\nEvent deleted.\n");
}

//works with example file.
void perform_event() {
	int status;
	int pipefd[2];
	char message[256];
	char temp[512];
	char names[50][50];//event partner names
	char fn[14]; // filename
	int i = 0;
	int j = 0;
	FILE* fp;
	printf("\nPerform event: \n");
	scanf("%10s", guest.event_id);
	snprintf(fn, sizeof(fn), "%s.txt", guest.event_id);
	signal(SIGTERM,handler);
	status = pipe(pipefd);
	if(status == -1){
		perror("Error \n");
		exit(1);
	}
	pid_t child = fork();
	if (child < 0)
	{
		perror("Error: child < 0\n");
		exit(1);
	}
	if (child > 0)
	{
		close(pipefd[0]);
		printf("Parent process...\n");
		//send event name and wait for answer
		write(pipefd[1], &fn, sizeof(fn));
		sleep(3);
		//pause();
		
		printf("Parent after pause...\n");
		//read: child at the place
		read(pipefd[1], &status, sizeof(status));
		printf("Parent after read...status: %d\n", status);
		
		if(status == 0){
			// Read to buffer 
			if((fp = fopen(fn, "r")) == NULL) {
				printf("\nFile does not exist.\n");
				exit(1);
			}
		
			int line_num = 1;
			while(fgets(temp, 512, fp) != NULL) {
				sscanf(temp,"%s %s %s %li", guest.name, guest.mail, guest.event_id, &guest.time);
				names[i][50] = guest.name[50];
				i++;	
				line_num++;
			}
	
			fclose(fp);
			
			write(pipefd[1], names, sizeof(names));
			//pause();
			//wait();
			sleep(3);
			//read names 
			read(pipefd[1], &fn, sizeof(fn));
			if((fp = fopen(fn, "r")) == NULL) {
				printf("\nFile does not exist.\n");
				exit(1);
			}
			
			printf("List of names, who didnt come: \n");
			while(fgets(temp, 512, fp) != NULL) {
				printf("%s", temp);
			}
		} 
		else 
		{
			perror("Error \n");
		}
		
		waitpid(child, &status, 0);
		printf("\nParent: child process ended\n");
		close(pipefd[1]);
		close(pipefd[0]);
	}
	else 
	{
		close(pipefd[1]);
		printf("Child process...\n");
		read(pipefd[0], fn, sizeof(fn));
		sleep(1);
		printf("\n Arrived to event place.\n");
		status = 0;
		write(pipefd[0], &status, sizeof(int));
		
		//kill(getppid(),SIGTERM);
		
		//Read names..
		read(pipefd[0],names,sizeof(names));
		char file_n[] = "didnotcome.txt";
		FILE* fp = fopen(file_n, "w"); 
		while (i > 0){
			if(rand()%10 == 0){
				printf("%s was not on the event\n", names[i][50]);
				fprintf(fp, "%s\n", names[i][50]);
			}
			else{
				printf("%s was on the event\n", names[i][50]);
			}
			i--;
		}
		int rnd = rand()%100;
		fprintf(fp, "The event was %d \% successfull.\n", rnd);
		fclose(fp);
		//write
		write(pipefd[0], file_n, sizeof(file_n));
		//kill(getppid(),SIGTERM);
	}
	
	
}

//menu
void run_menu() {
	int opt = 7;
	while(opt != 0){
	printf("\nKing of Stands menu: \n");
	printf("[1] - add guest\n");
	printf("[2] - modify guest data\n");
	printf("[3] - delete guest data\n");
	printf("[4] - list guests\n");
	printf("[5] - add new event\n");
	printf("[6] - delete event\n");
	printf("[7] - perform event\n");
	printf("[0] - exit\n");

	scanf("%d", &opt);

	switch (opt) {
	case 0:
		break;
	case 1:
		add_guest();
		break;
	case 2:
		mod_guest();
		break;
	case 3:
		del_guest();
		break;
	case 4:
		list_guests();
		break;
	case 5:
		new_event();
		break;
	case 6:
		del_event();
		break;
	case 7:
		perform_event();
		break;
	default:
		printf("Invalid option.\n");
	}
	}
}

int main(int argc, char ** argv) {
	//signal(SIGRTMIN, handler);
	run_menu();
	return 0;
}

