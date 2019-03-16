#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h> //waitpid

#include "helper_functions.h"

void signalUser1Handler(int signumber){
  printf("Partner has arrived to destination!\n");
}

void signalUser2Handler(int signumber, siginfo_t* info, void* nonused){
  printf("Current event ended!\n");
  switch (info->si_code){ 
    case SI_QUEUE: 
		printf("It was sent by a sigqueue, sending process (PID %i)\n",info->si_pid);
		
		EventResult* result=(EventResult*)info->si_value.sival_ptr;                     
		printf("Success rate: %i%%\n",result->successRate);
		
		if (result->numberOfMissingGuests > 0) {
			printf("Guests did not attend:\n");
			printf("%s\n", result->missingGuests);
			/*
			int ind;
			for (ind = 0; ind < result->numberOfMissingGuests; ind = ind + 1) {
				printf("%s\n", result->missingGuests[ind]);
			}
			*/
		} else {
			printf("All guests attended!\n");
		}
		
		break;
		
    default: printf("It was sent by something else \n");
  } 
}

void collect_event_list(char *fileName, EventList* event_list) {
	FILE *f;
	f = fopen(fileName, "rb");
	if (f==NULL){perror("Event list file not found!\n"); return;}
		
	fseek(f, sizeof(int), SEEK_SET);
	while (!feof(f)){
		EventListElement *current = malloc(sizeof(EventListElement));
		current->next = NULL;
		current->data = malloc(sizeof(Event));
		if (fread(current->data, sizeof(Event), 1, f)) {
			if (event_list->head == NULL) {
				event_list->head = current;
			}
			if (event_list->tail != NULL) {
				event_list->tail->next = current;
			}
			event_list->tail = current;
			
			//printf("Debug event: %d - %s (%s) | %s\n", event_list->head->data->id, event_list->head->data->name, event_list->head->data->email, event_list->head->data->time);
			//printf("Debug event: %d - %s (%s) | %s\n", event_list->tail->data->id, event_list->tail->data->name, event_list->tail->data->email, event_list->tail->data->time);
		} else {
			break;
		}
	}
	
	fclose(f);	
}

void collect_guest_list(Event *event, GuestList* guest_list) {
	FILE *f;
	char file_name[sizeof(event->name) + sizeof(event->id) + 2];
	sprintf(file_name, "%d_%s", event->id, event->name);
	f = fopen(file_name, "rb");
	if (f==NULL){perror("Event file not found!\n"); return;}
		
	fseek(f, sizeof(int), SEEK_SET);
	int numberOfGuests = 0;
	while (!feof(f)){
		GuestListElement *current = malloc(sizeof(GuestListElement));
		current->next = NULL;
		current->data = malloc(sizeof(Guest));
		if (fread(current->data, sizeof(Guest), 1, f)) {
			if (guest_list->head == NULL) {
				guest_list->head = current;
			}
			if (guest_list->tail != NULL) {
				guest_list->tail->next = current;
			}
			guest_list->tail = current;
			
			numberOfGuests = numberOfGuests + 1;
			
			//printf("Debug guest: %d - %s (%s) | %s\n", guest_list->head->data->id, guest_list->head->data->name, guest_list->head->data->email, guest_list->head->data->time);
			//printf("Debug guest: %d - %s (%s) | %s\n", guest_list->tail->data->id, guest_list->tail->data->name, guest_list->tail->data->email, guest_list->tail->data->time);
		} else {
			break;
		}
	}
	guest_list->numberOfGuests = numberOfGuests;
	
	fclose(f);	
}

void send_guest_list(int pipe_write, GuestList *guest_list) {
	write(pipe_write, &guest_list->numberOfGuests, sizeof(guest_list->numberOfGuests));
	int safetyNet2 = 0;
	while(safetyNet2 < 10 && guest_list != NULL && guest_list->head != NULL) {
		GuestListElement *current_guest = guest_list->head;
		write(pipe_write, current_guest->data->name, sizeof(current_guest->data->name));
		
		guest_list->head = current_guest->next;
		free(current_guest->data);
		free(current_guest);
		
		safetyNet2 = safetyNet2 + 1;
	}
}

void travel_to_destination(int pipe_read) {
	char destination[50];
	read(pipe_read,destination, sizeof(destination));
	printf("Received destination: %s\n", destination);
	
	printf("Partner is traveling...\n",SIGUSR1);
	sleep(3);
	kill(getppid(),SIGUSR1);
}

void read_guest_list(int pipe_read) {
	
}

int main(int argc,char** argv){
	srand(time(NULL));
	
	EventResult *event_result = malloc(sizeof(EventResult));
	event_result->numberOfMissingGuests = 0;
	
	EventList *event_list = malloc(sizeof(EventList));
	event_list->head = NULL;
	event_list->tail = NULL;
	
	if (argc!=2) {perror("Give a filename of the event list as a command line argument\n");exit(1);}
	if (!access(argv[1],F_OK)==0){perror("The file is not exist!\n"); exit(1);}
	collect_event_list(argv[1], event_list);
	
	if (event_list == NULL || event_list->head == NULL) {
		printf("No events where registered!\n");
		return 0;
	}
	
	printf("\n\nLet's get started!\n\n");
	signal(SIGUSR1,signalUser1Handler);
	
	struct sigaction sigact;
	sigact.sa_sigaction=signalUser2Handler; //instead of sa_handler, we use the 3 parameter version
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=SA_SIGINFO; //we need to set the siginfo flag 
	sigaction(SIGUSR2,&sigact,NULL);
	
	int pipefd[2];
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid_t child=fork(); //forks make a copy of variables
	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (child>0) 
	{
		// Write edstination
		close(pipefd[0]); 
		
		int safetyNet1 = 0;
		while(safetyNet1 < 10 && event_list->head != NULL) {
			EventListElement *current_event = event_list->head;
			
			separator();
			write(pipefd[1], current_event->data->name, sizeof(current_event->data->name));
		
			// Wait for arival
			pause();
			
			GuestList *guest_list = malloc(sizeof(GuestList));
			guest_list->head = NULL;
			guest_list->tail = NULL;
			collect_guest_list(current_event->data, guest_list);
		
			send_guest_list(pipefd[1], guest_list);
			
			// Wait for end of current event
			pause();
			
			int hasMore = 0;
			if (current_event->next != NULL) {
				hasMore = 1;
			}
			write(pipefd[1], &hasMore, sizeof(hasMore));
			
			// Free current head data, go to next
			event_list->head = current_event->next;
			free(current_event->data);
			free(current_event);
			
			safetyNet1 = safetyNet1 + 1;
		}
		
		close(pipefd[1]);
		fflush(NULL);
		
		separator();
		printf("Ferwell!\n\n");
		
		getchar();
	}
	else //child process
	{
		int hasMore = 1;
		close(pipefd[1]); 
		char *missingGuestNames = NULL;
		while(hasMore) { 
			travel_to_destination(pipefd[0]);
			
			// Receive guest list
			read_guest_list(pipefd[0]);
			
			printf("Reseting variables!!!\n");
			event_result->numberOfMissingGuests = 0;
			if (event_result->missingGuests != NULL) {
				free(event_result->missingGuests);
				event_result->missingGuests = NULL;
			}
			if (missingGuestNames != NULL) {
				free(missingGuestNames);
				missingGuestNames = NULL;
			}
			
			int numberOfGuests;
			read(pipefd[0],&numberOfGuests, sizeof(numberOfGuests));
			printf("Number of guests: %i\n", numberOfGuests);
			if (numberOfGuests > 0) {
				//collect guests
				int ind = 0;
				while(ind < numberOfGuests) {
					char guestName[50];
					read(pipefd[0],guestName, sizeof(guestName));
					printf("%s\n", guestName);
					
					int attended = getRandomValue(99);
					if(attended < 50) {
						event_result->numberOfMissingGuests = event_result->numberOfMissingGuests + 1;
						printf("Guest number %i, missed the event! Value: %i\n", ind, attended);
						
						if (missingGuestNames == NULL) {
							missingGuestNames = (char*) malloc(51 * sizeof(char));
							strcpy(missingGuestNames, guestName);
						} else {
							missingGuestNames = (char*) realloc(missingGuestNames, (event_result->numberOfMissingGuests * 50 + 1) * sizeof(char));
							strcat(missingGuestNames, "\n");
							strcat(missingGuestNames, guestName);
						}
					}
					
					ind = ind + 1;
				}
				
				//evaluate event
				if (event_result->numberOfMissingGuests > 0) {
					event_result->missingGuests = (char*) malloc((event_result->numberOfMissingGuests * 50 + 1) * sizeof(char));
					strcpy(event_result->missingGuests, missingGuestNames);
				}
			} else {
				printf("Guest list is empty!\n");
			}
			event_result->successRate = getRandomValue(100);
			
			printf("\nEvent ended!\n");
			union sigval s_value_ptr;
			s_value_ptr.sival_ptr=event_result;
			sigqueue(getppid(),SIGUSR2,s_value_ptr);
			
			read(pipefd[0],&hasMore, sizeof(hasMore));
		}
		
		close(pipefd[0]); // finally we close the used read end
		printf("Child process ended\n");
	}

	return 0;
}