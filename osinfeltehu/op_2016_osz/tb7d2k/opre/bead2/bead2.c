#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

struct Guest
{
	int id;
	char name[100];
	char email[100];
	int eventid;
	time_t signup;
};

struct Guest guests[100];
int guestid = 0;
int guestindex = 0;

void readGuests()
{
	guestindex = 0;
	FILE* file;
	file = fopen("guests", "r");
	if (file)
	{
		while (!feof(file))
		{
			if (fread(&(guests[guestindex]), sizeof(struct Guest), 1, file))
			{
				if (guests[guestindex].id >= guestid)
				{
					guestid = guests[guestindex].id+1;
				}
				++guestindex;
			}
		}
		fclose(file);
	}
}

void writeGuests(int beg, int end)
{
	FILE* file;
	file = fopen("guests", "w");
	for (int i = beg; i < end; ++i)
	{
		fwrite(&guests[i], sizeof(struct Guest), 1, file);
	}
	fclose(file);
}

void appendGuest(struct Guest guest)
{
	FILE* file;
	file = fopen("guests", "a");
	fwrite(&guest, sizeof(struct Guest), 1, file);
	fclose(file);
}

struct Event
{
	int id;
	char name[100];
	int count;
};

struct Event events[1000];
int eventid = 0;
int eventindex = 0;

void readEvents()
{
	eventindex = 0;
	FILE* file;
	file = fopen("events", "r");
	if (file)
	{
		while (!feof(file))
		{
			if (fread(&(events[eventindex]), sizeof(struct Event), 1, file))
			{
				if (events[eventindex].id >= eventid)
				{
					eventid = events[eventindex].id+1;
				}
				++eventindex;
			}
		}
		fclose(file);
	}
}

void writeEvents(int beg, int end)
{
	FILE* file;
	file = fopen("events", "w");
	for (int i = beg; i < end; ++i)
	{
		fwrite(&events[i], sizeof(struct Event), 1, file);
	}
	fclose(file);
}

void appendEvent(struct Event event)
{
	FILE* file;
	file = fopen("events", "a");
	fwrite(&event, sizeof(struct Event), 1, file);
	fclose(file);
}

bool isValidEventId(int id)
{
	readEvents();
	for (int i = 0; i < eventindex; ++i)
	{
		if (events[i].id == id)
		{
			return true;
		}
	}
	return false;
}

void listEvents()
{
	readEvents();
	printf("\nID\tName\tNumber of guests\n");
	for (int i = 0; i < eventindex; ++i)
	{
		printf("%d\t%s\t%d\n", events[i].id, events[i].name, events[i].count);
	}
}

void addEvent()
{
	readEvents();
	struct Event event;
	char name[100];
	printf("\nEvent name: ");
	scanf(" %[^\n]%*c", name);
	strcpy(event.name, name);
	event.id = eventid;
	event.count = 0;
	appendEvent(event);
}

void deleteEvent()
{
	readEvents();
	int id;
	printf("\nDelete event by ID: ");
	scanf("%d", &id);
	int position = -1;
	for (int i = 0; i < eventindex; ++i)
	{
		if (id == events[i].id)
		{
			position = i;
			break;
		}
	}
	if (position == -1)
	{
		printf("\nEvent ID not found\n");
		return;
	}
	for (int i = position; i < eventindex; ++i)
	{
		events[i] = events[i+1];
	}
	writeEvents(0, eventindex-1);
}

void listGuests()
{
	readGuests();
	printf("\nID\tName\tEmail\tEvent ID\tRegister time\n");
	for (int i = 0; i < guestindex; ++i)
	{
		printf("%d\t%s\t%s\t%d\t%s", guests[i].id, guests[i].name, guests[i].email, guests[i].eventid, ctime(&guests[i].signup));
	}
}

void addGuest()
{
	readEvents();
	readGuests();
	struct Guest guest;
	guest.signup = time(NULL);
	guest.id = guestid;
	char name[100];
	printf("\nName: ");
	scanf (" %[^\n]%*c", name);
	strcpy(guest.name, name);
	char email[100];
	printf("\nEmail address: ");
	scanf (" %[^\n]%*c", email);
	strcpy(guest.email, email);
	int eventid;
	printf("\nEvent ID: ");
	scanf (" %d", &eventid);
	bool isValidEventId = false;
	for (int i = 0; i < eventindex; ++i)
	{
		if (events[i].id == eventid)
		{
			events[i].count += 1;
			isValidEventId = true;
			break;
		}
	}
	if (!isValidEventId)
	{
		printf("\nEvent ID not found\n");
		return;
	}
	guest.eventid = eventid;
	writeEvents(0, eventindex);
	appendGuest(guest);
	printf("\nGuest added");
}

void editGuest()
{
	readGuests();
	int id;
	printf("\nEdit guest by ID: ");
	scanf("%d", &id);
	bool isValidGuestId = false;
	for (int i = 0; i < guestindex; ++i)
	{
		if (guests[i].id == id)
		{
			isValidGuestId = true;
			printf("\nNew name: ");
			scanf (" %[^\n]%*c", guests[i].name);
			printf("\nNew e-mail address: ");
			scanf (" %[^\n]%*c", guests[i].email);
			break;
		}
	}
	if (!isValidGuestId)
	{
		printf("\nID not found\n");
		return;
	}
	writeGuests(0, guestindex);
}

void deleteGuest()
{
	readEvents();
	readGuests();
	int id;
	printf("\nDelete guest by ID: ");
	scanf("%d", &id);
	int position = -1;
	for (int i = 0; i < guestindex; ++i)
	{
		if (guests[i].id == id)
		{
			position = i;
			for (int j = 0; j < eventindex; ++j)
			{
				if (events[j].id == guests[i].eventid)
				{
					events[j].count -= 1;
					break;
				}
			}
			break;
		}
	}
	if (position == -1)
	{
		printf("\nID not found\n");
		return;
	}
	for (int i = position; i < guestindex; ++i)
	{
		guests[i] = guests[i+1];
	}
	writeGuests(0, guestindex-1);
	writeEvents(0, eventindex);
}

static int arrived = 0;
static key_t key;

static void handleSignal(int signumber)
{
	if (signumber == SIGTERM)
	{
		arrived = 1;
	}
}

void startEvent()
{
	readGuests();
	int id;
	printf("\nStart event by ID: ");
	scanf("%d", &id);
	if (!isValidEventId(id))
	{
		printf("\nID not found\n");
		return;
	}
	struct Guest registered[100];
	int count = 0;
	for (int i = 0; i < guestindex; ++i)
	{
		if (guests[i].eventid == id)
		{
			registered[count] = guests[i];
			count++;
		}
	}
	struct message {
		long mtype;
		char mtext[1024];
	};
	int msgqueue = msgget(key, 0600 | IPC_CREAT);
	if (msgqueue < 0 ) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	int pipefd[2];
	pid_t cpid;
	if (pipe(pipefd) == -1)
	{
		perror("Pipe opening was unsuccesful\n");
		exit(EXIT_FAILURE);
	}
	signal(SIGTERM, handleSignal);
	cpid = fork();
	if (cpid < 0)
	{
		perror("Fork calling was unsuccesful\n");
		exit(EXIT_FAILURE);
	}
	else if (cpid > 0)
	{
		pause();
		if (arrived)
		{
			printf("\nOrganiser arrived\nSending guest list\n");
		}
		close(pipefd[0]);
		char buffer[100];
		for (int i = 0; i < count; ++i)
		{
			//char buffer[100];
			strcpy(buffer, registered[i].name);
			write(pipefd[1], &buffer, sizeof(buffer));
		}
		close(pipefd[1]);
		struct message m;
		msgrcv(msgqueue, &m, 1024, 1, 0);
		printf("\nEvent review: %s\n", m.mtext);
		int status;
		waitpid(cpid, &status, 0);
		printf("\nOrganiser finished\n");
	}
	else
	{
		sleep(1);
		kill(getppid(), SIGTERM);
		close(pipefd[1]);
		sleep(1);
		printf("\nEvent started\n\nEvent guests:\n");
		srand(time(NULL));
		for (int i = 0; i < count; ++i) {
			char buffer[100];
			read(pipefd[0], &buffer, sizeof(buffer));
			if (rand() / (RAND_MAX + 1.0) < 0.9)
			{
				printf("%s arrived\n", buffer);
			}
			else
			{
				printf("%s is missing\n", buffer);
				/*struct message guest = { 2, "" };
				sprintf(guest.mtext, "%s", buffer);
				msgsnd(msgqueue, &guest, strlen(guest.mtext), 0);*/
			}
		}
		sleep(1);
		printf("\nEvent ended\n");
		struct message review = { 1, "" };
		sprintf(review.mtext, "%d/10", rand() % 11);
		msgsnd(msgqueue, &review, strlen(review.mtext), 0);
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}
}

void menu()
{
	int option;
	do {
		printf("\nChoose an option: ");
		printf("\n1 - List guests\n");
		printf("2 - Add guest\n");
		printf("3 - Edit guest\n");
		printf("4 - Delete guest\n");
		printf("5 - List events\n");
		printf("6 - Add event\n");
		printf("7 - Delete event\n");
		printf("8 - Start event\n");
		printf("9 - Exit\n");
		printf("\nYour option: ");
		scanf("%d", &option);
		switch (option)
		{
			case 1: listGuests();
			break;
			case 2: addGuest();
			break;
			case 3: editGuest();
			break;
			case 4: deleteGuest();
			break;
			case 5: listEvents();
			break;
			case 6: addEvent();
			break;
			case 7: deleteEvent();
			break;
			case 8: startEvent();
			break;
		}
	} while (option != 9);
}

int main(int argc, char** argv)
{
	key = ftok(argv[0], 1);
	menu();
	return 0;
}