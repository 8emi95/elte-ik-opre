#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "visitor_list.h"
#include "event_list.h"

void handler(int signumber) {}
void organize_events(struct event_node* events, struct visitor_node* visitors, int pipefd[], pid_t pid);
void send_participants(int event_id, struct visitor_node* visitors, int pipefd[]);
int count_participants(int event_id, struct visitor_node* visitors);

int main()
{
	int pipefd[2];
	
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a cső nyitásakor.\n");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, handler);
	
	pid_t pid = fork();
	
	if (pid != 0) // parent, organizer
	{
		struct visitor_node visitors;
		visitors.next = NULL;
		const char visitor_file[] = "visitors_data.bin";
		load_visitors(&visitors, visitor_file);
		
		struct event_node events;
		events.next = NULL;
		const char event_file[] = "events_data.bin";
		load_events(&events, event_file);
		
		organize_events(events.next, visitors.next, pipefd, pid);
	}
	else // child, partner
	{
		char buffer[100];		
		srand(time(NULL));
		for (read(pipefd[0], buffer, 50); strcmp(buffer, "Vége") != 0; read(pipefd[0], buffer, 50))
		{
			printf("[Pnr] Megyek a rendezvény helyszínére: %s.\n", buffer);
			
			sleep(1);
			kill(getppid(), SIGUSR1);
			
			int headcount;
			read(pipefd[0], &headcount, sizeof(headcount));
			char blacklist[headcount][100];
			int not_appeared = 0;
			int i;
			puts("\n[Pnr] Ezeket a résztvevőket kell várnom:");
			for (i = 0; i < headcount; ++i)
			{
				read(pipefd[0], buffer, 100);
				puts(buffer);
				
				if (rand() % 10 == 0)
				{
					// blacklist[not_appeared] = (char*)malloc(100);
					strcpy(blacklist[not_appeared++], buffer);
				}
			}
			
			putchar('\n');
						
			int success = rand() % 21 * 5;
			write(pipefd[1], &success, sizeof(success));
			
			write(pipefd[1], &not_appeared, sizeof(not_appeared));
			
			for (i = 0; i < not_appeared; ++i)
				write(pipefd[1], blacklist[i], sizeof(blacklist[i]));
				
			// pause();
			sleep(1);
			kill(getppid(), SIGUSR1);

			pause();
		}
		
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void organize_events(struct event_node* events, struct visitor_node* visitors, int pipefd[], pid_t pid)
{
	for (; events != NULL; events = events->next)
	{
		write(pipefd[1], events->data.location, 50);
		
		sleep(1);
		kill(pid, SIGUSR1);
		
		pause();
		puts("[KoS] Jól van, a helyeden vagy.");
		
		send_participants(events->data.id, visitors, pipefd);
				
		// sleep(1);
		// kill(pid, SIGUSR1);
		
		pause();
		
		int success;
		read(pipefd[0], &success, sizeof(success));
		printf("[KoS] Azt a visszajelzést kaptam, hogy a rendezvény %i%%-ban volt sikeres, és ezek a jelentkezettek nem jelentek meg:\n", success);
		
		int not_appeared;
		read(pipefd[0], &not_appeared, sizeof(not_appeared));
		
		int i;
		for (i = 0; i < not_appeared; ++i)
		{
			char name[100];
			read(pipefd[0], name, 100);
			puts(name);
		}
		
		putchar('\n');
		
		// sleep(1);
		// kill(pid, SIGUSR1);
		// pause();
	}
	
	write(pipefd[1], "Vége", 100);
	close(pipefd[0]);
	close(pipefd[1]);
}

void send_participants(int event_id, struct visitor_node* visitors, int pipefd[])
{
	int headcount = count_participants(event_id, visitors);
	write(pipefd[1], &headcount, sizeof(headcount));
	
	for (; visitors != NULL; visitors = visitors->next)
		if (visitors->data.event_id == event_id)
			write(pipefd[1], visitors->data.name, 100);
}

int count_participants(int event_id, struct visitor_node* visitors)
{
	int count = 0;
	for (; visitors != NULL; visitors = visitors->next)
		if (visitors->data.event_id == event_id)
			++count;
	
	return count;
}
