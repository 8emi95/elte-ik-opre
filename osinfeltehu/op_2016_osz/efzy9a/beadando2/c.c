//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}


struct Guest{
	char registration_number[50];
	char name[50];//07-11-16_00.00;
	char email_address[50];
	char event_id[50];
	char subscription_date[50];
};

struct Event{
	char event_id[50];
	char event_name[50];
	char event_starts[50];
	char event_ends[50];
};


int num_of_guests;
struct Guest* guests;
int num_of_events;
struct Event* events;

int loadDatabaseStatus(const char* event_list_file_name, const char* guest_list_file_name);


int loadDatabaseStatus(const char* event_list_file_name, const char* guest_list_file_name){
	printf("Loading database..\n");
	// Reading the database from the files(Guests, Events)
	FILE* event_file = fopen(event_list_file_name, "r");
	FILE* guest_file = fopen(guest_list_file_name, "r");
	if(event_file == NULL || guest_file == NULL){
		printf("One of the files of the two of the files  does/do not exist.\n");
		// Or another way is that the program creates the missing files.
		return 1;
	}
	else{
		// Reading event datas
		num_of_events = 0;
		fscanf(event_file, "%i\n", &num_of_events);
		// Event(event_id; event_name; event_starts; event_ends)
		if(num_of_events > 0){
			int i;
			events = (struct Event*)realloc(events, (sizeof(struct Event) *
			num_of_events));
			for(i = 0; i < num_of_events; ++i){
				fscanf(event_file, "%s %s %s %s\n", events[i].event_id,
				events[i].event_name, events[i].event_starts,
				events[i].event_ends);
			}
		}
		else{
			printf("The event list is empty(in this case the guest list too, mutually).\n");
		}
		
		// Reading guest datas
		num_of_guests = 0;
		fscanf(guest_file, "%i\n", &num_of_guests);
		// Guest(registration_number; name; email_address; event_id, subscription_date)
		if(num_of_guests > 0){
			int i;
			guests = (struct Guest*)realloc(guests, (sizeof(struct Guest) *
			num_of_guests));
			for(i = 0; i < num_of_guests; ++i){
				fscanf(guest_file, "%s %s %s %s %s\n",
				guests[i].registration_number, guests[i].name,
				guests[i].email_address, guests[i].event_id,
				guests[i].subscription_date);
			}
		}
		else{
			printf("The guest list is empty.\n");
			return 1;
		}
	}
	fclose(event_file);
	fclose(guest_file);
	return 0;
}


char** strSplit(char* text_string, const char delimiter)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = text_string;
    char* last = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    // Certain amount of element will be separated
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count++;
            last = tmp;
        }
        tmp++;
    }

    // Add space for trailing token
    count += last < (text_string + strlen(text_string) - 1);

    // Add space for terminating null string so caller
    // knows where the list of returned strings ends
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t index  = 0;
        char* token = strtok(text_string, delim);

        while (token)
        {
            assert(index < count);
            *(result + index++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(index == count - 1);
        *(result + index) = 0;
    }

    return result;
}

int event_id = 0;
void executeEvent() {
    signal(SIGTERM,handler);

    int parentToChild[2], childToParent[2], n, status;
    char bufParent[200];
    char bufChild[2000];

    pid_t pid;

    pipe(parentToChild);
    pipe(childToParent);

    // Creating child process for managing the event
    if((pid=fork()) < 0) {
        abort();
    } else if(pid > 0) {// Parent
        close(parentToChild[0]);

        // Sending event
        write(parentToChild[1], events[event_id].event_name,
			strlen(events[event_id].event_name));

        // Waiting for child arrival
        pause();
        printf("Parent: Child arrived at the event.\n",SIGTERM);
        
        // Sending guests
        char str2[200];
        int i;
        for(i=0; i<num_of_guests - 1; ++i) {
            strcpy(str2, guests[i].name);
            strcat(str2, ",");
            write(parentToChild[1], str2, strlen(str2));
        }
        strcpy(str2, guests[i].name);
        strcat(str2, ",");
        write(parentToChild[1], str2, strlen(str2+1));

        close(parentToChild[1]);
        close(childToParent[1]);

        // Waiting for child messages
        int n;
        read(childToParent[0], &n, sizeof(n));
        printf("Probability of event: %d\n", n);

        while (n = read(childToParent[0], bufParent, sizeof(bufParent)) > 0) {
            printf("Who was not at the event:\n");
            printf(bufParent);
        }
        if (n < 0) {
            perror("Error reading from pipe.");
        }
		
        wait(NULL);
    }
    else
    { // Child
        // Getting event
        close(parentToChild[1]);

        read(parentToChild[0], bufChild, sizeof(bufChild));

        // Sending signal to the parent because of successful arrival
        printf("Child: Event: %s\n", bufChild);
        
		
		
		sleep(2);
        kill(getppid(),SIGTERM);

        // Getting guests
        read(parentToChild[0], bufChild, sizeof(bufChild));
        printf("Child: Guests:\n");
        char** tokens;
        tokens = strSplit(bufChild, ',');
        if(tokens) {
            int i;
            for (i = 0; *(tokens + i); i++) {
                printf("%s\n", *(tokens + i));
            }
            printf("\n");
        }

        close(parentToChild[0]);
        close(childToParent[0]);

        // Sending probability of the event
        /*
		int skipped = 0;
		time_t t;
		srand((unsigned) time(&t));
		
		if(*available_cases == (atoi(num_of_items) - (item_index) - 1)){
			skipped = 0;
			// froze the decreasing of the available_cases
			// otherwise this case would be equal to that case
			// when the given N true values are adjusted in advance 
			// before the last N item
		}
		else if(*available_cases > 0){
			skipped = rand() % 2;
			--(*available_cases);
		}else{
			// do nothing
		}
		skipped = ( rand() % 2);
		*/
		
		
		char snum[3];
        srand(time(NULL));
        int rndnum = (rand()%100)+1;
        write(childToParent[1], &rndnum, sizeof(rndnum));

        // Sending 10 % of the guests, those who were not on the event
        char notBeenThere[2000] = "";
        int i;
        for (i = 0; *(tokens + i); i++) {
            if(0==rand()%10) {
                strcat(notBeenThere, *(tokens + i));
                strcat(notBeenThere, "\n");
            }
        }
        write(childToParent[1], notBeenThere, strlen(notBeenThere)+1);
        close(childToParent[1]);

        // Freeing allocated memory
        for (i = 0; *(tokens + i); i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }
}

void guestListing(int a){
	printf("Guest listing\n");
	if(num_of_guests == 0){
		printf("The guest list is empty. Type 1 for recording new gurest..\n");
	}
	else{		
		// Guest(registration_number; name; email_address; event_id, subscription_date)
		printf("reg_number, name, email, event id, subscription date\n");
		int i;
		for(i = 0; i < num_of_guests; ++i){
			printf("%s %s %s %s %s\n", guests[i].registration_number,
				guests[i].name, guests[i].email_address,
				guests[i].event_id, guests[i].subscription_date);
		}
	}
}

int main() {
	printf("King of Stands\n"); 
	
	if(0 == loadDatabaseStatus("event_list.txt", "guest_list.txt")){
		
		guestListing(1);
		// Counting the number of guests for all events
		/*int m;
		for(m = 0; m < num_of_events; ++m){
			int n;
			num_of_event_guests_c[m] = 0;
			for(n = 0; n < num_of_guests; ++n){
				++num_of_event_guests_c[m];
			}
		}*/
		
		executeEvent(0);
		/*int i;
		for(i =0; i < num_of_events; ++i){
			printf("%i.", i);
			c_num_of_events = 0;
			printf("\n\n");
		}*/		
	}	
	else{
		printf("Could not load the database.\n");
	}	
	return 0;
	
	/*
    int again=1;
    do {
        
        printf("\n***\n1 Rendezvenyek listazasa\n2 Rendezveny resztvevoinek listazasa\n\n3 Rendezveny lebonyolitasa\n0 kilepes\n***\n\nKivalasztott: ");
        char input = getchar();
        

        
        char input='3';
        
        switch(input) {
            case '1':
                list_programs(); break;
            case '2':
                list_attendants(); break;
            case '3':
                do_program();
            case '0':
                again = 0; break;
            default:
                puts("Nincs ilyen parancs!");
                break;
        }
    } while (1==again);
	*/
}

