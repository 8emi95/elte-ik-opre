#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> 
#include <string.h>
#include <fcntl.h>
#include <time.h>

typedef struct visitordata {
	char name[80];
	char email[80];
	int id;
  char reg_time[9];
}visitordata;

struct visitordata readinput() {
	struct visitordata toreturn;
	int n = 0;
  char firstname[80], lastname[80], email[80], eventid[80], timeString[9], c;
	struct tm * time_info;
	
	time_t secs = time(0);
	time_t current_time;
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(toreturn.reg_time, sizeof(timeString), "%H:%M:%S", time_info);

	printf("Enter first and last name: ");
  scanf("%s %s", firstname, lastname);
	strcat(firstname, " ");
	strcat(firstname, lastname);
	strcpy(toreturn.name, firstname);
	//fgets(toreturn.name, 25, stdin);
	while ((c = getchar()) != EOF && c != '\n');

	printf("Enter email address: ");
  scanf("%s", toreturn.email);
	while ((c = getchar()) != EOF && c != '\n');

  printf("Enter event id: ");
  while (scanf("%d", &toreturn.id) != 1)
  {
    while (getchar() != '\n');
    printf ("Try again: ");
  }  
	return toreturn;
}

void readtomodify(FILE* f, const size_t length, struct visitordata* V){
	FILE *  my_file;
	my_file = fopen("visitors.txt", "r");

	if (!my_file)
  {
		fprintf(stderr, "\n--------\nError opening data file:  visitors.txt\n--------\n");
  }
	char str[80], email[80], reg_time[9], firstname[40], lastname[40], c;		
	int id;
	V = (visitordata*)malloc(length * sizeof(visitordata));
	int i = 0;
		
	printf("\nPlease enter the email address of the attendee that you wish to modify: ");
	scanf("%s", email);
	
	while(!feof(my_file) && i < length)
	{			

		fscanf (my_file, " %80[^\t] %s %i %s", V[i].name, V[i].email, &V[i].id, V[i].reg_time);

				if(strcmp(email, V[i].email) == 0) {
			printf("\nPlease enter the new firstname, lastname, email address and event id for the user divided by Space characters:\n");
		  scanf("%s %s %s %i", firstname, lastname, V[i].email, &V[i].id);
			strcat(firstname, " ");
			strcat(firstname, lastname);
			strcpy(V[i].name, firstname);
			while ((c = getchar()) != EOF && c != '\n');
		}
		++i;
	}
	fclose(my_file);
	FILE* overwrt;
	overwrt = fopen("visitors.txt", "w+");	
	for (i=0; i < length; ++i){
		fprintf(overwrt, "%s%s %i %s\n",strcat(V[i].name, "\t"), V[i].email, V[i].id, V[i].reg_time);
		// while ((c = getchar()) != EOF && c != '\n');
	}
	fclose(overwrt);
}

void readtodelete(FILE* f, const size_t length, struct visitordata* V){
	FILE *  my_file;
	my_file = fopen("visitors.txt", "r");

	if (!my_file)
  {
		fprintf(stderr, "\n--------\nError opening data file:  visitors.txt\n--------\n");
  }
	char str[80], email[80], reg_time[9], firstname[40], lastname[40], c;		
	int id;
	V = (visitordata*)malloc(length * sizeof(visitordata));
	int i = 0;
		
	printf("\nPlease enter the email address of the attendee that you wish to remove from the list: ");
	scanf("%s", email);
	while(!feof(my_file) && i < length)
	{			
		fscanf (my_file, " %80[^\t] %s %i %s", V[i].name, V[i].email, &V[i].id, V[i].reg_time);
		++i;
	}
	rewind(my_file);
	i = 0;
	int found = 0;
  while(!feof(my_file) && i < length && found == 0)
	{			
		if(strcmp(email, V[i].email) == 0) {
			found = 1;
		}
		else{
			++i;
		}
	}
	printf("i a while előtt %i:", i);
	while(i < length-1){
		printf("whileban %s %s:", V[i].name, V[i+1].name);
		strcpy(V[i].name, V[i+1].name);
		printf("name, %s", V[i].name);
		strcpy(V[i].email, V[i+1].email);
		printf("email, %s", V[i].email);
		V[i].id = V[i+1].id;
		printf("id, %i", V[i].id);
  	strcpy(V[i].reg_time, V[i+1].reg_time);
		printf("regtime, %s", V[i].reg_time);
		++i;
	}
	V = (visitordata*)realloc(V, (length-1) * sizeof(visitordata));
	fclose(my_file);
	FILE* overwrt;
	overwrt = fopen("visitors.txt", "w+");	
	for (i=0; i < length - 1; ++i){
		fprintf(overwrt, "%s%s %i %s\n",strcat(V[i].name, "\t"), V[i].email, V[i].id, V[i].reg_time);
		// while ((c = getchar()) != EOF && c != '\n');
	}
	fclose(overwrt);
}

int getfilelength(int handle){
		int line = 0;
		char buff[1];
		lseek(handle, 0, SEEK_SET);
	
		while (0 != read(handle, buff, 1)) {
			if(*buff == '\n') {
				line++;
			}
		}
		return line;
}

int main(int argc, char** argv)
{
	char choice[1] = "1";
	struct visitordata mydata;
	char name[80];
	printf("\nWelcome!\n");
	while(atoi(choice) > 0 && atoi(choice) < 6) {
		printf("You have the following options:\n");
		printf("1 Enter a new attendee\n2 View a list of attendees\n");
		printf("3 Remove an entry\n4 Modify data for an attendee\n5 Launch new event (delete all current event data)\nElse: Exit\n");
		printf("Please write your choice and press Enter: ");
		scanf("%s", choice);
		
		if (atoi(choice) == 1){
			mydata = readinput();
			int handle = open("./visitors.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
			if (handle < 0){
				printf("\n--------\nFile error.\n--------\n");
				return;
			}
			
		char buffer[20];
		sprintf(buffer, "%d", mydata.id);
		lseek(handle, 0, SEEK_END);
		write(handle, strcat(mydata.name, "\t"), strlen(mydata.name)+1);  
		write(handle, strcat(mydata.email, " "), strlen(mydata.email)+1);  
		write(handle, strcat((buffer), " "), strlen((buffer))+1);  
		write(handle, strcat(mydata.reg_time, "\n"), strlen(mydata.reg_time)+1);
		int line = getfilelength(handle);
		
		close(handle);
		printf("\n--------\nThe attendee named %swith email address %shas been added to the event %i as the number %i attendee.\n--------\n\n",mydata.name, mydata.email, mydata.id, line);
		}
		else if (atoi(choice) == 2) {
			int handle = open("./visitors.txt", O_RDONLY);
			if(handle == -1) {
				printf("\n--------\nError: no list exists yet.\n--------\n");			
			} 
			else {
				char buf[1];
				lseek(handle, 0, SEEK_SET);
				printf("\nThe current list of attendees is as follows:\n\n");
				while (read(handle, buf, 1) == 1) {
					putchar(buf[0]);
				}
				putchar('\n');
			}
			close(handle);
		}
		else if (atoi(choice) == 3) {
			int handle = open("./visitors.txt",  O_RDWR, S_IRUSR|S_IWUSR);
			if(handle == -1) {
				printf("\n--------\nError: no list exists yet.\n--------\n");			
			} 
			else {
				char email[80];
				int lines = getfilelength(handle);
				struct visitordata* V;
				FILE* file;
				readtodelete(file, lines, V);
			}
			close(handle);
		}
		else if (atoi(choice) == 4) {
			int handle = open("./visitors.txt",  O_RDWR, S_IRUSR|S_IWUSR);
			if(handle == -1) {
				printf("\n--------\nError: no list exists yet.\n--------\n");			
			} 
			else {
				char email[80];
				int lines = getfilelength(handle);
				struct visitordata* V;
				FILE* file;
				readtomodify(file, lines, V);
			}
			close(handle);
		}
		else if (atoi(choice) == 5) {
			int ret = remove("./visitors.txt");
			if(ret == 0){
				printf("\n--------\nFile deleted successfully!\n--------\n");
			}
			else {
				printf("\n--------\nError: unable to delete the file!\n--------\n");
			}		
	  }
	}
  printf("\nGoodbye!\n");
	return 0;
}
