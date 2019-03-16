#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

//*************************************
///////Declarations
//*************************************
char semail[50];
char sname[30];
int find;
int count;
int editMenu;
struct person {
	char fname[30];
	char lname[30];
	char address [60];
	char email[50];
	char tel[20];
	char prio[4];
	char date[23];
};
struct person *input;
int goodroute = 0;
time_t rawtime;
struct tm * timeinfo;
int menu = 89;


//*************************************
///////Function definitions
//*************************************


void add(){
	input = malloc(sizeof(struct person));
	printf("Scan\n"); 	
	printf("Add first name: ");
	scanf("%s", input->fname);
	printf("Add last name: ");
	scanf("%s", input->lname);
	printf("Add your address(City example street 23): ");
	size_t temp_buf_size = 512;
	char* temp_buf = (char*)malloc(temp_buf_size * sizeof(char));
	getline(&temp_buf, &temp_buf_size, stdin);
	fgets (input->address, sizeof(input->address), stdin);
	input->address[strcspn(input->address, "\n")] = 0;
	printf("Add email: ");
	scanf("%s", input->email);
	printf("Add telephone number: ");
	scanf("%s", input->tel);
	printf("Is it an emergency mission?(YES/NO) ");
	scanf("%s", input->prio);
	if(strcmp(input->prio,"NO") == 0 || strcmp(input->prio,"YES") == 0)
		goodroute = 1;
	while(goodroute != 1){
		printf("Is it an emergency mission? !!!(YES/NO)!!! ");
		scanf("%s", input->prio);
		if(strcmp(input->prio,"NO") == 0 || strcmp(input->prio,"YES") == 0)
		goodroute = 1;
	}
	goodroute = 0;
	sprintf(input->date,"%d,%d,%d,%d:%d:%d",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	FILE *fptr;
	fptr = (fopen("routes.txt", "a"));
	if(fptr == NULL)
	{
		printf("Error!");	
	}
	fwrite(input, sizeof(struct person), 1, fptr);
	fclose(fptr);
}

//*************************************

void edit(){
	find = 0;
	printf("Edit\n"); 
	printf("Add your email: ");
	scanf("%s", &semail);
	
	input = malloc(sizeof(struct person));
	
	FILE *wfile;
	fclose(fopen("routesplus.txt", "w"));
	wfile = fopen("routesplus.txt", "w");
	
	FILE *fptr2;
	fptr2 = (fopen("routes.txt", "r"));
	if (fptr2) {
		while(fread(input, sizeof(struct person), 1, fptr2)){
			
			if(strcmp(input->email,semail) == 0){ 
				printf ("Name:%s %s | Address:%s | E-mail:%s | Tel:%s | Emergency:%s | Add date:%s \n", input->fname,input->lname, input->address, input->email, input->tel, input->prio, input->date);
				printf("what would you like to change?\n");
				printf("1 - Name\n");
				printf("2 - Address\n");
				printf("3 - Email\n");
				printf("4 - Telephone number\n");
				printf("5 - Importance \n");
				printf("0 - Nothing\n");
				printf("Switch a point:");
				editMenu=56;
				scanf("%d", &editMenu);
				if( !(menu == 0 || menu == 1 || menu == 2 || menu == 3 || menu == 4 || menu == 5))
				{
					printf("error\n");
					editMenu = 0;
				}
				switch(editMenu){
					case 1 :
						printf("Add first name: ");
						scanf("%s", input->fname);
						printf("Add last name: ");
						scanf("%s", input->lname);
						break;
					case 2 :
						printf("Add your address(City example street 23): ");
						size_t temp_buf_size = 512;
						char* temp_buf = (char*)malloc(temp_buf_size * sizeof(char));
						getline(&temp_buf, &temp_buf_size, stdin);
						fgets (input->address, sizeof(input->address), stdin);
						input->address[strcspn(input->address, "\n")] = 0;
						break;
					case 3 :
						printf("Add email: ");
						scanf("%s", input->email);
						break;
					case 4 :
						printf("Add telephone number: ");
						scanf("%s", input->tel);
						break;
					case 5 :
						printf("Is it an emergency mission?(YES/NO) ");
						scanf("%s", input->prio);
						if(strcmp(input->prio,"NO") == 0 || strcmp(input->prio,"YES") == 0)
							goodroute = 1;
						while(goodroute != 1){
							printf("Is it an emergency mission? !!!(YES/NO)!!! ");
							scanf("%s", input->prio);
							if(strcmp(input->prio,"NO") == 0 || strcmp(input->prio,"YES") == 0)
							goodroute = 1;
						}
						goodroute = 0;
						break;
				}

				sprintf(input->date,"%d,%d,%d,%d:%d:%d",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				find = 1;
				fwrite(input, sizeof(struct person), 1, wfile);
			}else{
				fwrite(input, sizeof(struct person), 1, wfile);
			}
			input = malloc(sizeof(struct person));
				
		}
	}
	fclose(fptr2);
	fclose(wfile);
	
	if(find == 0){
		printf("There are no such emails in the system\n");
	}
	else{
		FILE *newfile;
		fclose(fopen("routes.txt", "w"));
		newfile = fopen("routes.txt", "a");
		if(newfile == NULL)
		{
			printf("Error!");
		}
		FILE *rfile;
		rfile = (fopen("routesplus.txt", "r"));
		if (rfile) {
			input = malloc(sizeof(struct person));
			while(fread(input, sizeof(struct person), 1, rfile)){
				fwrite(input, sizeof(struct person), 1, newfile);
			}
		}
		
		fclose(newfile);
		fclose(rfile);
	}
}

//*************************************

void delete(char semail[]){
	printf("Delete\n");
	find = 0;
	
	input = malloc(sizeof(struct person));
	
	FILE *wfile2;
	fclose(fopen("routesplus.txt", "w"));
	wfile2 = fopen("routesplus.txt", "w");

	FILE *fptr3;
	fptr3 = (fopen("routes.txt", "r"));
	if (fptr3) {
		while(fread(input, sizeof(struct person), 1, fptr3)){
			
			if(strcmp(input->email,semail) == 0){
				find = 1;
			}else{
				fwrite(input, sizeof(struct person), 1, wfile2);
				input = malloc(sizeof(struct person));
			}
				
		}
	}
	fclose(fptr3);
	fclose(wfile2);
	
	if(find == 0){
		printf("There are no such emails in the system\n");
	}
	else{
		FILE *newfile2;
		fclose(fopen("routes.txt", "w"));
		newfile2 = fopen("routes.txt", "a");

		FILE *rfile2;
		rfile2 = (fopen("routesplus.txt", "r"));
		if (rfile2) {
			input = malloc(sizeof(struct person));
			while(fread(input, sizeof(struct person), 1, rfile2)){
				fwrite(input, sizeof(struct person), 1, newfile2);
			}
		}
		printf("Your mission deleted\n");
		fclose(newfile2);
		fclose(rfile2);
	}
}

//*************************************

void all_mission(){
	printf("List all mission\n"); 				
	FILE *fptr4;
	fptr4 = (fopen("routes.txt", "r"));
	input = malloc(sizeof(struct person));
	if (fptr4) {
		while(fread(input, sizeof(struct person), 1, fptr4))
			printf ("Name:%s %s | Address:%s | E-mail:%s | Tel:%s | Emergency:%s | Add date:%s \n\n", input->fname,input->lname, input->address,input->email, input->tel, input->prio, input->date);
		fclose(fptr4);
	}
}

//*************************************

void list_importance(){
	
	input = malloc(sizeof(struct person));
	
	if(strcmp(semail,"NO") == 0 || strcmp(semail,"YES") == 0)
		goodroute = 1;
	while(goodroute != 1){
		printf("Priority or not priority!!!(YES/NO)!!!: ");
		scanf("%s", &semail);
		if(strcmp(semail,"NO") == 0 || strcmp(semail,"YES") == 0)
			goodroute = 1;
	}
	goodroute = 0;
	
	FILE *fptr5;
	fptr5 = (fopen("routes.txt", "r"));
	if (fptr5) {
		while(fread(input, sizeof(struct person), 1, fptr5)){
			if(strcmp(input->prio,semail) == 0)
				printf ("Name:%s %s | Address:%s | E-mail:%s | Tel:%s | Emergency:%s | Add date:%s \n\n", input->fname,input->lname, input->address,input->email, input->tel, input->prio, input->date);
		}
		fclose(fptr5);
	}
}

//*************************************

struct person add_next_mission(){
	
	input = malloc(sizeof(struct person));
	struct person firstNo;
	struct person firstYes;
	//firstNo = malloc(sizeof(struct person));
	
	int count = 0; 
	char imp[3] = "NO";
	FILE *fptr5;
	fptr5 = (fopen("routes.txt", "r"));
	if (fptr5) {
		while(strcmp(imp,"YES") != 0 && fread(input, sizeof(struct person), 1, fptr5)){
			if(count == 0 && input->prio!="YES"){
				memcpy(&firstNo,input,sizeof(firstNo));
			}
			strcpy(imp,input->prio);
			count++;
		}
		if(count > 0){
			if(strcmp(imp,"YES") == 0)
			{
				memcpy(&firstYes,input,sizeof(firstYes));
				return firstYes;
			}
			else{
				return firstNo;
			}
		}
		fclose(fptr5);
	}
	firstNo = (struct person) {0};
	return firstNo;
}

//*************************************

void list_name(){
	printf("Listing by name\n"); 
	printf("Add the first name: ");
	scanf("%s",  &semail);
	printf("Add the last name: ");
	scanf("%s", &sname);
	
	input = malloc(sizeof(struct person));
	FILE * fptr6;
	fptr6 = (fopen("routes.txt", "r"));
	if (fptr6) {
		count = 0;
		while(fread(input, sizeof(struct person), 1, fptr6)){
			if(strcmp(input->fname,semail) == 0 && strcmp(input->lname,sname) == 0){
				count++;
				printf ("Name:%s %s | Address:%s | E-mail:%s | Tel:%s | Emergency:%s | Last modified date:%s \n\n", input->fname,input->lname, input->address, input->email, input->tel, input->prio, input->date);
			}
		}
		fclose(fptr6);
		if(count == 0)
			printf("Searching name: %s %s\nThere are no such name in the system \n",semail,sname);
	}
}


//*************************************

void handler(int signumber){}

//*************************************

void mypipe(/*char route[],struct customer list[10],int partner, int pcs*/){

	pid_t pid;
	char sz[100];  // char array for reading from 
	int parentToChild[2];       // [0]parent------------child[1]
	int childToParent[2];		// [0]child------------parent[1]
	
	signal(SIGUSR1,handler);
	
	int i;
	
	if (pipe(parentToChild) < 0 || pipe(childToParent) < 0) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork();	// creating parent-child processes
	if (pid == -1) 
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) 
	{		    	
		// idomár process
		close(parentToChild[1]);    
		close(childToParent[0]);
		printf("The trainer is getting ready\n");
		sleep(2);
		printf("The trainer is ready...\n");
		sleep(1);
		kill(getppid(), SIGUSR1);
		int count=0;
		struct person next_mission;
		read(parentToChild[0],&next_mission,sizeof(struct person));
		while(strcmp(next_mission.date,"") != 0 )
		{
			printf("The trainer get the informations\n");
			printf ("Name:%s %s | Address:%s | E-mail:%s | Tel:%s | Emergency:%s | Add date:%s \n", next_mission.fname,next_mission.lname, next_mission.address,next_mission.email, next_mission.tel, next_mission.prio, next_mission.date);
			printf("The trainer let go of the bird... \n");
			srand(time(NULL));
			int r=rand()%4+1;
			sleep(r);
			printf("The capture succeeded in %d seconds\n",r);
			delete(next_mission.email);
			write(childToParent[1],&r,sizeof(int));
			printf("\nThe trainer require the next mission. \n");
			kill(getppid(), SIGUSR1);
			read(parentToChild[0],&next_mission,sizeof(struct person));
			count++;
		}
		if(count>0){
			printf("Trainer going to rest.\n");
		}
		
		
	} 
	else 
	{    // Ne-Sas-Old Kft process 
		close(parentToChild[0]);
		close(childToParent[1]);
		printf("The Kft. is waiting for the signal\n");
		pause();
		printf("The Kft. send the mission\n");
		FILE *billfile;
		billfile = fopen("bills.txt", "a");
		struct person next_mission = add_next_mission();
		while(strcmp(next_mission.date,"") != 0 ){
			write(parentToChild[1],&next_mission,sizeof(struct person));
			pause();
			int r;
			read(childToParent[0],&r,sizeof(int));
			//----make_bills
			char date[23];
			sprintf(date,"%d,%d,%d,%d:%d:%d",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			if(billfile == NULL)
			{
				printf("Error!");
					
			}else{
				fprintf (billfile, "Name: %s %s | Email: %s | Captured time: %d | Captured date:%s \n", next_mission.fname, next_mission.lname, next_mission.email, r, date);
					
			}
			next_mission = add_next_mission();
		}
		fclose(billfile);
		
		write(parentToChild[1],&next_mission,sizeof(struct person));
		sleep(2);
		printf("Thats all!\n");
		printf("Sikeres lefutás:\n");
	}
	
	
	exit(EXIT_SUCCESS);// force exit, not necessary
}

void mission_start(){
	mypipe();
}

//----------------------------------------
///////Main program
//----------------------------------------

int main()
{   
	while( 1==1 ){
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		printf("1 - Add a new mission\n");
		printf("2 - Edit a mission\n");
		printf("3 - Delete a mission\n");
		printf("4 - List all mission\n");
		printf("5 - Listing by importance \n");
		printf("6 - Listing by name \n");
		printf("7 - Missions start \n");
		printf("0 - Exit\n");
		printf("Switch a menu point:");
		
		
		scanf("%d", &menu);

		if( !(menu == 0 || menu == 1 || menu == 2 || menu == 3 || menu == 4 || menu == 5 || menu == 6 || menu == 7))
		{
			printf("error\n");
			menu = 0;
		}
		switch(menu){
			case 1 :
				add();
				break;
			case 2 :
				edit();
				break;
			case 3 : 
				printf("Add your email: ");
				scanf("%s", &semail);
				delete(semail);
				break;
			case 4 :
				all_mission();
				break;
			case 5 :
				list_importance();	
				break;	
			case 6 :
				list_name();
				break;
			case 7 :
				mission_start();
				break;
			case 0 : return 0;
		}
	}
}
